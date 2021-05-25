//================================================================
#ifndef FL_PACK2_H
#define FL_PACK2_H


//----------------------------------------------------------------
// Defines
//----------------------------------------------------------------
#define FL_PACK2_BUFFER_SIZE  MB(500)
#define FL_ASSET2_BUFFER_SIZE MB(100)


//----------------------------------------------------------------
// Structures
//----------------------------------------------------------------
typedef struct fl_Asset2 fl_Asset2;
struct fl_Asset2
    {
    u64 name_hash;
    u64 data_offset;
    u64 raw_data_length;
    u32 unzipped_data_length;
    u32 zip_flag;
    };

typedef struct fl_Pack2 fl_Pack2;
struct fl_Pack2
    {
    String8    pack_path;
    u32        asset_count;
    u64        pack_length;
    fl_Asset2 *asset2s;
    };


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
//// Helper functions
fl_internal u64
fl_crc64_uppercase(char *to_hash);

//// Main functions
fl_internal fl_Pack2
fl_load_pack2(char *pack_path, u8 *pack2_buffer, u32 pack2_max_size);

fl_internal fl_Asset2
fl_get_asset2_by_hash(u64 hash, fl_Pack2 pack);

fl_internal fl_Asset2
fl_get_asset2_by_name(char *name, fl_Pack2 pack);

// NOTE(rhett): Returns a copy of asset with the unzipped asset size.
fl_internal fl_Asset2
fl_read_asset2(fl_Asset2 asset, u8 *pack2_buffer, u8 *asset2_buffer, u32 max_asset2_size);


#endif // FL_PACK2_H
//================================================================

//================================================================
#ifdef FL_PACK2_IMPL


//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
//// Helper Functions
// TODO(rhett): Clean this up
fl_internal u64
fl_crc64_uppercase(char *to_hash)
    {
    u32 string_length = cstring_length(to_hash);

    char upper_string[256] = {0};
    if (string_length > 256)
        {
        fl_printf("Whoops, guess filenames can be bigger than 256 characters...");
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
fl_internal fl_Pack2
fl_load_pack2(char *pack_path, u8 *pack2_buffer, u32 pack2_max_size)
    {
    fl_Pack2 result = {0};

    fl_printf("Loading \"%s\"...\n", pack_path);
    result.pack_path = cstring_to_string8(pack_path);

    if (!pack2_buffer)
        {
        return result;
        }
    
    if (!fl_w32_read_entire_file(result.pack_path.content, pack2_buffer, pack2_max_size))
        {
        return result;
        }

    u8 *buffer_begin = pack2_buffer;

    // TODO(rhett): Verify magic
    u8 magic[3];
    fl_memcpy(pack2_buffer, magic, 3);
    pack2_buffer += 3;

    u8 version = *pack2_buffer;
    pack2_buffer += 1;

    result.asset_count = fl_get_u32le(pack2_buffer);
    pack2_buffer += 4;

    result.pack_length = fl_get_u64le(pack2_buffer);
    pack2_buffer += 8;

    u64 map_offset = fl_get_u64le(pack2_buffer);
    pack2_buffer += 8;

    // NOTE(rhett): Skip unk0 and signature. Move right to map
    pack2_buffer = buffer_begin + map_offset;

    // NOTE(rhett): Load asset info
    // TODO(rhett): We should probably move allocation out of here
    result.asset2s = fl_cast(fl_Asset2 *)fl_alloc(result.asset_count * sizeof(fl_Asset2));

    fl_Asset2 *ptr_asset2s;
    for (u32 i = 0; i < result.asset_count; ++i)
        {
        ptr_asset2s = &result.asset2s[i];

        ptr_asset2s->name_hash = fl_get_u64le(pack2_buffer);
        pack2_buffer += 8;

        ptr_asset2s->data_offset = fl_get_u64le(pack2_buffer);
        pack2_buffer += 8;

        ptr_asset2s->raw_data_length = fl_get_u64le(pack2_buffer);
        pack2_buffer += 8;

        ptr_asset2s->zip_flag = fl_get_u32le(pack2_buffer);
        pack2_buffer += 4;

        // NOTE(rhett): Skip checksum
        pack2_buffer += 4;
        // printf("%#018llx\n", result.name_hashes[i]);
        }
    return result;
    }

// TODO(rhett): We may be able to optimize this as long as the hashes are in order of least to greatest
fl_internal fl_Asset2
fl_get_asset2_by_hash(u64 hash, fl_Pack2 pack)
    {
    for (u32 i = 0; i < pack.asset_count; ++i)
        {
        if (pack.asset2s[i].name_hash == hash)
            {
            return pack.asset2s[i];
            }
        }
    fl_Asset2 empty = {0};
    return empty;
    }

fl_internal fl_Asset2
fl_get_asset2_by_name(char *name, fl_Pack2 pack)
    {
    // TODO(rhett): Check if name hash is already cached
    u64 hash = fl_crc64_uppercase(name);
    return fl_get_asset2_by_hash(hash, pack);
    }

fl_internal fl_Asset2
fl_read_asset2(fl_Asset2 asset, u8 *pack2_buffer, u8 *asset2_buffer, u32 max_asset2_size)
    {
    switch(asset.zip_flag)
        {
        // NOTE(rhett): Asset is unzipped
        case 0x00:
        case 0x10:
            fl_printf("UNZIPPED\n");
            // TODO(rhett): 
            return asset;
            break;

        // NOTE(rhett): Asset is zipped
        case 0x01:
        case 0x11:
            // fl_printf("ZIPPED\n");

            // NOTE(rhett): Skip A1B2C3D4
            pack2_buffer += 4;

            // NOTE(rhett): This is big endian
            asset.unzipped_data_length = fl_get_u32be(pack2_buffer);
            pack2_buffer += 4;

            if (asset.unzipped_data_length > max_asset2_size)
                {
                fl_printf("Unzipped length is bigger than the max asset2 size. NOT GOOD\n");
                return asset;
                }

            mz_uncompress(asset2_buffer,
                          &asset.unzipped_data_length,
                          pack2_buffer,
                          asset.raw_data_length);
            break;
        }

    return asset;
    }

// TODO(rhett): maybe we'll take a namelist here
// NOTE(rhett): This will allocate 600mb
fl_internal void
fl_export_assets_from_pack2(char *pack_path, char *output_folder)
    {
    u8 *buffer_begin = fl_alloc(FL_PACK2_BUFFER_SIZE + FL_ASSET2_BUFFER_SIZE);
    u8 *pack_buffer = buffer_begin;
    u8 *asset_buffer = buffer_begin + FL_PACK2_BUFFER_SIZE;

    fl_Pack2 pack = fl_load_pack2(pack_path,
                                  pack_buffer,
                                  FL_PACK2_BUFFER_SIZE);

    fl_w32_create_folder(output_folder);
    for (int i = 0; i < pack.asset_count; ++i)
        {
        fl_Asset2 *ptr_asset = &pack.asset2s[i];

        pack_buffer = buffer_begin + ptr_asset->data_offset;
        *ptr_asset = fl_read_asset2(*ptr_asset,
                                    pack_buffer,
                                    asset_buffer,
                                    FL_PACK2_BUFFER_SIZE);

        if (ptr_asset->unzipped_data_length == 0)
            {
            fl_printf("Skipping unzipped asset...\n");
            continue;
            }

        char output_path[256];
        sprintf(output_path,
                "%s\\%016llx.bin",
                output_folder,
                ptr_asset->name_hash);
        fl_w32_write_buffer_to_file(output_path, asset_buffer, ptr_asset->unzipped_data_length);
        }
    } 


#endif // FL_PACK2_IMPL
