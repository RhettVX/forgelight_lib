#include "../external/miniz.h"

#include "../forgelight_lib.h"
#include "internal.h"
#include "os.h"


//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
//// Helper Functions
// TODO(rhett): Clean this up
// NOTE(rhett): Calculates a crc64 hash of an asset's filename as used by pack2s
u64
pack2_names_calculate_hash(String8 to_hash)
    {
    // u32 string_length = strings_cstring_length(to_hash);

    char upper_string[FL_NAMELIST_MAX_NAME_LENGTH] = {0};
    if (to_hash.length > FL_NAMELIST_MAX_NAME_LENGTH)
        {
        printf("Whoops, guess filenames can be bigger than what we have in FL_NAMELIST_MAX_NAME_LENGTH...");
        return 0;
        }

    for (u32 i = 0; i < to_hash.length; ++i)
        {
        char c = *(to_hash.content + i);
        if (c >= 'a' && c <= 'z')
            {
            c ^= 0b00100000;
            }
        upper_string[i] = c;
        }

    return ~crc64(~0, upper_string, to_hash.length);
    }

// TODO(rhett): Redo this
// Pack2_Namelist
// pack2_names_generate_namelist_from_string_list(String8* string_list, uint string_count)
//     {
//     uint capacity = FL_NAMELIST_BUFFER_SIZE / sizeof(Pack2_Namelist);
//     if (string_count > capacity)
//         {
//         printf("[!] String count exceeds namelist capacity. Increase namelist buffer or use less strings.\n");
//         Pack2_Namelist empty = {0};
//         return empty;
//         }

//     Pack2_Namelist namelist = {.count = 0,
//                                .capacity = capacity,
//                                .entries = malloc(FL_NAMELIST_BUFFER_SIZE)};

//     for (uint i = 0; i < string_count; ++i)
//         {
//         u64 hash = pack2_names_calculate_hash(string_list[i].content);
//         Pack2_NamelistEntry entry = {.hash = hash,
//                                      .name = string_list[i]};

//         namelist.entries[namelist.count] = entry;
//         ++namelist.count;
//         }

//     return namelist;
//     }

// NOTE(rhett): Assume the file contains a list of names separated by newlines
Pack2_Namelist
pack2_names_generate_namelist_from_file(char* file_path, u32 file_buffer_size)
    {
    Pack2_Namelist result = {.count = 0,
                             .capacity = FL_NAMELIST_MAX_NAME_AMOUNT,
                             .entries = {0},
                             .raw_data_ptr = 0};

    // NOTE(rhett): Load namelist file to memory
    result.raw_data_ptr = malloc(file_buffer_size);
    if (!os_load_entire_file(file_path, result.raw_data_ptr, file_buffer_size))
        {
        printf("[!] Unable to load file: \"%s\"\n", file_path);
        free(result.raw_data_ptr);
        Pack2_Namelist empty = {0};
        return empty;
        }

    // NOTE(rhett): Allocate memory for the Pack2_Namelist
    result.entries = malloc(result.capacity * sizeof(Pack2_NamelistEntry));

    // NOTE(rhett): Feed pointers of names in memory to the namelist
    uint char_count = 0;
    for (u32 i = 0; i < file_buffer_size; ++i)
        {
        // TODO(rhett): Just handling \r\n for now
        if (result.raw_data_ptr[i] == '\r' && result.raw_data_ptr[++i] == '\n')
            {
            String8 string = {.length = char_count,
                              .content = (&result.raw_data_ptr[i-1] - char_count)};

            Pack2_NamelistEntry entry = {.hash = pack2_names_calculate_hash(string), 
                                         .name = string};

            result.entries[result.count] = entry;
            ++result.count;
            char_count = 0;

            continue;
            }

        ++char_count;
        }

    return result;
    }

//// Main functions
Pack2
pack2_load_from_file(char* pack_path, u8* pack2_buffer, u32 pack2_max_size)
    {
    Pack2 result = {0};

    printf("Loading \"%s\"...\n", pack_path);
    result.pack_path = strings_cstring_to_string8(pack_path);

    if (!pack2_buffer)
        {
        return result;
        }
    
    if (!os_load_entire_file(result.pack_path.content, pack2_buffer, pack2_max_size))
        {
        return result;
        }

    u8* buffer_begin = pack2_buffer;

    // TODO(rhett): Verify magic
    u8 magic[3];
    memcpy(pack2_buffer, magic, 3);
    pack2_buffer += 3;

    u8 version = *pack2_buffer;
    pack2_buffer += 1;

    result.asset_count = endian_get_u32_le(pack2_buffer);
    pack2_buffer += 4;

    result.pack_length = endian_get_u64_le(pack2_buffer);
    pack2_buffer += 8;

    u64 map_offset = endian_get_u64_le(pack2_buffer);
    pack2_buffer += 8;

    // NOTE(rhett): Skip unk0 and signature. Move right to map
    pack2_buffer = buffer_begin + map_offset;

    // NOTE(rhett): Load asset info
    // TODO(rhett): We should probably move allocation out of here
    result.asset2s = malloc(result.asset_count * sizeof(Asset2));

    Asset2* ptr_asset2s;
    for (u32 i = 0; i < result.asset_count; ++i)
        {
        ptr_asset2s = &result.asset2s[i];

        ptr_asset2s->name_hash = endian_get_u64_le(pack2_buffer);
        pack2_buffer += 8;

        ptr_asset2s->data_offset = endian_get_u64_le(pack2_buffer);
        pack2_buffer += 8;

        ptr_asset2s->raw_data_length = endian_get_u64_le(pack2_buffer);
        pack2_buffer += 8;

        ptr_asset2s->zip_flag = endian_get_u32_le(pack2_buffer);
        pack2_buffer += 4;

        // NOTE(rhett): Skip checksum
        pack2_buffer += 4;
        }
    return result;
    }

// TODO(rhett): We may be able to optimize this as long as the hashes are in order of least to greatest
Asset2
pack2_asset2_get_by_hash(u64 hash, Pack2 pack)
    {
    for (u32 i = 0; i < pack.asset_count; ++i)
        {
        if (pack.asset2s[i].name_hash == hash)
            {
            return pack.asset2s[i];
            }
        }
    Asset2 empty = {0};
    return empty;
    }

Asset2
pack2_asset2_get_by_name(char* name, Pack2 pack)
    {
    // TODO(rhett): Check if name hash is already cached
    u64 hash = pack2_names_calculate_hash(strings_cstring_to_string8(name));
    return pack2_asset2_get_by_hash(hash, pack);
    }

Asset2
pack2_asset2_load_to_buffer(Asset2 asset, u8* pack2_buffer, u8* asset2_buffer, u32 max_asset2_size)
    {
    switch(asset.zip_flag)
        {
        // NOTE(rhett): Asset is unzipped
        case 0x00:
        case 0x10:
            // TODO(rhett): I am copying the data from the pack2_buffer to the asset2_buffer, but I don't believe that is necessary 
            memcpy(pack2_buffer, asset2_buffer, asset.raw_data_length);
            break;

        // NOTE(rhett): Asset is zipped
        case 0x01:
        case 0x11:
            // NOTE(rhett): Skip A1B2C3D4
            pack2_buffer += 4;

            // NOTE(rhett): This is big endian
            asset.unzipped_data_length = endian_get_u32_be(pack2_buffer);
            pack2_buffer += 4;

            if (asset.unzipped_data_length > max_asset2_size)
                {
                printf("Unzipped length is bigger than the max asset2 size. NOT GOOD\n");
                return asset;
                }

            mz_uncompress(asset2_buffer,
                          &asset.unzipped_data_length,
                          pack2_buffer,
                          (mz_ulong)asset.raw_data_length);
        }

    return asset;
    }

// NOTE(rhett): This will allocate 600mb
// TODO(rhett): this will allocate a bunch more, I need to clean this back up
void
pack2_export_assets_as_files(char* pack_path, char* output_folder, char* namelist_path)
    {
    u8* buffer_begin = malloc(FL_PACK2_BUFFER_SIZE + FL_ASSET2_BUFFER_SIZE);
    u8* pack_buffer  = buffer_begin;
    u8* asset_buffer = buffer_begin + FL_PACK2_BUFFER_SIZE;

    Pack2 pack = pack2_load_from_file(pack_path,
                                      pack_buffer,
                                      FL_PACK2_BUFFER_SIZE);

    Pack2_Namelist namelist = pack2_names_generate_namelist_from_file(namelist_path,
                                                                      FL_NAMELIST_FILE_BUFFER_SIZE);
    pack2_consolidate_with_namelist(&pack, namelist);

    os_create_folder(output_folder);
    for (uint i = 0; i < pack.asset_count; ++i)
        {
        Asset2* ptr_asset = &pack.asset2s[i];

        pack_buffer = buffer_begin + ptr_asset->data_offset;
        *ptr_asset = pack2_asset2_load_to_buffer(*ptr_asset,
                                                 pack_buffer,
                                                 asset_buffer,
                                                 FL_PACK2_BUFFER_SIZE);

        // TODO(rhett): Clean this up
        char output_path[OS_MAX_PATH_LENGTH];
        if (pack.asset2_names[i].hash)
            {
            String8 name = pack.asset2_names[i].name;
            char temp_buffer[FL_NAMELIST_MAX_NAME_LENGTH] = {0};
            memcpy(name.content, temp_buffer, name.length);

            sprintf_s(output_path,
                      OS_MAX_PATH_LENGTH,
                      "%s\\%s",
                      output_folder,
                      temp_buffer);
            }
        else
            {
            sprintf_s(output_path,
                      OS_MAX_PATH_LENGTH,
                      "%s\\%#018llx.bin",
                      output_folder,
                      ptr_asset->name_hash);
            }

        // TODO(rhett): maybe we should just set unzipped_data_length to raw asset size if uncompressed
        // NOTE(rhett): If unzipped_data_length is 0, then we're most likely uncompressed
        if (ptr_asset->unzipped_data_length)
            {
            os_write_buffer_to_file(output_path, asset_buffer, ptr_asset->unzipped_data_length);
            }
        else
            {
            os_write_buffer_to_file(output_path, asset_buffer, ptr_asset->raw_data_length);
            }
        }
    
    // TODO(rhett): I think I'm allocating way more than I should be
    free(buffer_begin);
    free(pack.asset2s);
    free(pack.asset2_names);
    free(namelist.entries);
    free(namelist.raw_data_ptr);
    } 

extern void
pack2_consolidate_with_namelist(Pack2* pack, Pack2_Namelist namelist)
    {
    if (pack->asset2_names)
        {
        printf("\"%s\" has already been consolidated with a namelist.", pack->pack_path.content);
        return;
        }

    pack->asset2_names = malloc(pack->asset_count * sizeof(Pack2_NamelistEntry));

    // TODO(rhett): Optimize this. Skip already used names.
    for (u32 i = 0; i < pack->asset_count; ++i)
        {
        for (u32 j = 0; j < namelist.count; ++j)
            {
            if (pack->asset2s[i].name_hash == namelist.entries[j].hash)
                {
                pack->asset2_names[i] = namelist.entries[j];
                break;
                }
            }
        }
    }

