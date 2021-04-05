//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
//// Helper Functions
// TODO(rhett): hash the uppercase string
fl_external ui64
fl_crc64(char *to_hash)
    {
    return ~crc64(~0, to_hash, cstring_length(to_hash));
    }

//// Main functions
fl_external void
fl_load_pack2(Pack2 *ptr_pack, char *pack_path)
    {
    printf("Loading \"%s\"...\n", pack_path);
    ptr_pack->pack_path = cstring_to_string8(pack_path);

    ui8 *buffer = w32_read_entire_file(ptr_pack->pack_path.content);
    ui8 *buffer_begin = buffer;

    // TODO(rhett): Verify magic
    ui8 magic[3] = {0};
    fl_memcpy(buffer, magic, 3);
    buffer += 3;

    ui8 version = *buffer;
    buffer += 1;

    ptr_pack->asset_count = get_ui32le(buffer);
    buffer += 4;

    ptr_pack->pack_length = get_ui64le(buffer);
    buffer += 8;

    ui64 map_offset = get_ui64le(buffer);
    buffer += 8;

    // NOTE(rhett): Skip unk0 and signature. Move right to map
    buffer = buffer_begin + map_offset;

    // NOTE(rhett): Load asset info
    // ptr_pack->name_hashes = fl_cast(ui64 *)fl_alloc(ptr_pack->asset_count * 8);
    // TODO(rhett): We should probably move allocation out of here
    ptr_pack->asset2s = fl_cast(Asset2 *)fl_alloc(ptr_pack->asset_count * sizeof(Asset2));

    Asset2 *ptr_asset2s = 0;
    for (ui32 i = 0; i < ptr_pack->asset_count; ++i)
        {
        ptr_asset2s = &ptr_pack->asset2s[i];

        ptr_asset2s->name_hash = get_ui64le(buffer);
        buffer += 8;

        ptr_asset2s->data_offset = get_ui64le(buffer);
        buffer += 8;

        ptr_asset2s->raw_data_length = get_ui64le(buffer);
        buffer += 8;

        ptr_asset2s->zip_flag = get_ui32le(buffer);
        buffer += 4;

        // NOTE(rhett): Skip checksum
        buffer += 4;
        // printf("%#018llx\n", ptr_pack->name_hashes[i]);
        }
    }

// TODO(rhett): We may be able to optimize this as long as the hashes are in order of least to greatest
fl_external Asset2 *
fl_get_asset_index_by_hash(ui64 hash, Pack2 pack)
    {
    for (ui32 i = 0; i < pack.asset_count; ++i)
        {
        if (pack.asset2s[i].name_hash == hash)
            {
            return &pack.asset2s[i];
            }
        }
    return 0;
    }

fl_external Asset2 *
fl_get_asset_index_by_name(char *name, Pack2 pack)
    {
    // TODO(rhett): Check if name hash is already cached
    ui64 hash = fl_crc64(name);
    return fl_get_asset_index_by_hash(hash, pack);
    }

fl_external b32
fl_unpack_asset2(Asset2 *asset)
    {
    switch(asset->zip_flag)
        {
        // NOTE(rhett): Asset is unzipped
        case 0x00:
        case 0x10:
            printf("UNZIPPED\n");
            break;

        // NOTE(rhett): Asset is zipped
        case 0x01:
        case 0x11:
            printf("ZIPPED\n");
            break;
        }

    return 0;
    }

