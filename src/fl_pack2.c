//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
//// Helper Functions
// TODO(rhett): Clean this up
fl_external ui64
fl_crc64_uppercase(char *to_hash)
    {
    ui32 string_length = cstring_length(to_hash);

    char upper_string[255] = {0};
    if (string_length > 255)
        {
        printf("Whoops, guess filenames can be bigger than 256 characters...");
        return 0;
        }

    for (ui32 i = 0; i < string_length; ++i)
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
fl_external Pack2
fl_load_pack2(char *pack_path, ui8 *pack2_buffer, ui32 pack2_max_size)
    {
    Pack2 result = {0};

    printf("Loading \"%s\"...\n", pack_path);
    result.pack_path = cstring_to_string8(pack_path);

    if (!pack2_buffer)
        {
        return result;
        }
    
    if (!w32_read_entire_file(result.pack_path.content, pack2_buffer, pack2_max_size))
        {
        return result;
        }

    ui8 *buffer_begin = pack2_buffer;

    // TODO(rhett): Verify magic
    ui8 magic[3] = {0};
    fl_memcpy(pack2_buffer, magic, 3);
    pack2_buffer += 3;

    ui8 version = *pack2_buffer;
    pack2_buffer += 1;

    result.asset_count = get_ui32le(pack2_buffer);
    pack2_buffer += 4;

    result.pack_length = get_ui64le(pack2_buffer);
    pack2_buffer += 8;

    ui64 map_offset = get_ui64le(pack2_buffer);
    pack2_buffer += 8;

    // NOTE(rhett): Skip unk0 and signature. Move right to map
    pack2_buffer = buffer_begin + map_offset;

    // NOTE(rhett): Load asset info
    // TODO(rhett): We should probably move allocation out of here
    result.asset2s = fl_cast(Asset2 *)fl_alloc(result.asset_count * sizeof(Asset2));

    Asset2 *ptr_asset2s = 0;
    for (ui32 i = 0; i < result.asset_count; ++i)
        {
        ptr_asset2s = &result.asset2s[i];

        ptr_asset2s->name_hash = get_ui64le(pack2_buffer);
        pack2_buffer += 8;

        ptr_asset2s->data_offset = get_ui64le(pack2_buffer);
        pack2_buffer += 8;

        ptr_asset2s->raw_data_length = get_ui64le(pack2_buffer);
        pack2_buffer += 8;

        ptr_asset2s->zip_flag = get_ui32le(pack2_buffer);
        pack2_buffer += 4;

        // NOTE(rhett): Skip checksum
        pack2_buffer += 4;
        // printf("%#018llx\n", result.name_hashes[i]);
        }
    return result;
    }

// TODO(rhett): We may be able to optimize this as long as the hashes are in order of least to greatest
fl_external Asset2
fl_get_asset2_by_hash(ui64 hash, Pack2 pack)
    {
    for (ui32 i = 0; i < pack.asset_count; ++i)
        {
        if (pack.asset2s[i].name_hash == hash)
            {
            return pack.asset2s[i];
            }
        }
    Asset2 empty = {0};
    return empty;
    }

fl_external Asset2
fl_get_asset2_by_name(char *name, Pack2 pack)
    {
    // TODO(rhett): Check if name hash is already cached
    ui64 hash = fl_crc64_uppercase(name);
    return fl_get_asset2_by_hash(hash, pack);
    }

fl_external Asset2
fl_read_asset2(Asset2 asset, ui8 *pack2_buffer, ui8 *asset2_buffer, ui32 max_asset2_size)
    {
    switch(asset.zip_flag)
        {
        // NOTE(rhett): Asset is unzipped
        case 0x00:
        case 0x10:
            printf("UNZIPPED\n");


            // TODO(rhett): 
            break;

        // NOTE(rhett): Asset is zipped
        case 0x01:
        case 0x11:
            printf("ZIPPED\n");

            // NOTE(rhett): Skip A1B2C3D4
            pack2_buffer += 4;

            // NOTE(rhett): This is big endian
            asset.unzipped_data_length = get_ui32be(pack2_buffer);
            pack2_buffer += 4;

            if (asset.unzipped_data_length > max_asset2_size)
                {
                printf("Unzipped length is bigger than the max asset2 size. NOT GOOD\n");
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

