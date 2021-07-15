#include "../external/miniz.h"

#include "../forgelight_lib.h"
#include "internal.h"

//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
//// Helper Functions
// TODO(rhett): Clean this up
u64
pack2_crc64_uppercase(char* to_hash)
    {
    u32 string_length = strings_cstring_length(to_hash);

    char upper_string[256] = {0};
    if (string_length > 256)
        {
        printf("Whoops, guess filenames can be bigger than 256 characters...");
        return 0;
        }

    for (u32 i = 0; i < string_length; ++i)
        {
        char c = *(to_hash + i);
        if (c >= 'a' && c <= 'z')
            {
            c ^= 0b00100000;
            }
        upper_string[i] = c;
        }

    return ~crc64(~0, upper_string, string_length);
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
    
    if (!os_read_entire_file(result.pack_path.content, pack2_buffer, pack2_max_size))
        {
        return result;
        }

    u8* buffer_begin = pack2_buffer;

    // TODO(rhett): Verify magic
    u8 magic[3];
    memcpy(pack2_buffer, magic, 3);
    pack2_buffer += 3;

    u8 version =* pack2_buffer;
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
    result.asset2s = (Asset2* )malloc(result.asset_count*  sizeof(Asset2));

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
        // printf("%#018llx\n", result.name_hashes[i]);
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
    u64 hash = pack2_crc64_uppercase(name);
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
            printf("UNZIPPED\n");
            // TODO(rhett): 
            return asset;
            break;

        // NOTE(rhett): Asset is zipped
        case 0x01:
        case 0x11:
            // printf("ZIPPED\n");

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
            break;
        }

    return asset;
    }

// TODO(rhett): maybe we'll take a namelist here
// NOTE(rhett): This will allocate 600mb
void
pack2_export_assets_as_files(char* pack_path, char* output_folder)
    {
    u8* buffer_begin = (u8*)malloc(FL_PACK2_BUFFER_SIZE + FL_ASSET2_BUFFER_SIZE);
    u8* pack_buffer = buffer_begin;
    u8* asset_buffer = buffer_begin + FL_PACK2_BUFFER_SIZE;

    Pack2 pack = pack2_load_from_file(pack_path,
                                      pack_buffer,
                                      FL_PACK2_BUFFER_SIZE);

    os_create_folder(output_folder);
    for (uint i = 0; i < pack.asset_count; ++i)
        {
        Asset2* ptr_asset = &pack.asset2s[i];

        pack_buffer = buffer_begin + ptr_asset->data_offset;
        *ptr_asset = pack2_asset2_load_to_buffer(*ptr_asset,
                                                 pack_buffer,
                                                 asset_buffer,
                                                 FL_PACK2_BUFFER_SIZE);

        if (ptr_asset->unzipped_data_length == 0)
            {
            printf("Skipping unzipped asset...\n");
            continue;
            }

        // TODO(rhett): don't hardcode the path size
        char output_path[256];
        sprintf_s(output_path,
                256,
                "%s\\%016llx.bin",
                output_folder,
                ptr_asset->name_hash);
        os_write_buffer_to_file(output_path, asset_buffer, ptr_asset->unzipped_data_length);
        break; // TODO(rhett): 
        }
        
    free(buffer_begin);
    free(pack.asset2s);
    } 
