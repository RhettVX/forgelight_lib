#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "fl_core.h"
#include "fl_string.h"
#include "fl_w32_utils.h"

// #include "external/lookup2.c"
#include "external/crc64.c"
#include "fl_core.c"
#include "fl_string.c"
#include "fl_w32_utils.c"

// TODO(rhett): I'm not so sure we need to use String8 so much


//----------------------------------------------------------------
// Structures
//----------------------------------------------------------------
typedef struct Asset2_Data Asset2_Data;
struct Asset2_Data
    {
    uint64 data_offset;
    uint64 raw_data_length;
    uint32 zip_flag;
    };

typedef struct Pack2 Pack2;
struct Pack2
    {
    String8      pack_path;
    uint32       asset_count;
    uint64      *name_hashes;
    uint64       pack_length;
    Asset2_Data *asset_data;
    };


//----------------------------------------------------------------
// Helper functions
//----------------------------------------------------------------
// TODO(rhett): hash the uppercase string
fl_external uint64
fl_crc64(String8 to_hash)
    {
    return ~crc64(~0, to_hash.content, to_hash.size-1);
    }


//----------------------------------------------------------------
// Main functions
//----------------------------------------------------------------
fl_external void
fl_load_pack2(Pack2 *ptr_pack, String8 pack_path)
    {
    printf("Loading \"%s\"...\n", pack_path.content);
    ptr_pack->pack_path.size = pack_path.size;
    ptr_pack->pack_path.content = fl_alloc(pack_path.size);
    fl_memcpy(pack_path.content, ptr_pack->pack_path.content, ptr_pack->pack_path.size);

    uint8 *buffer = w32_read_entire_file(ptr_pack->pack_path);
    uint8 *buffer_begin = buffer;

    // TODO(rhett): Verify magic
    uint8 magic[3] = {0};
    fl_memcpy(buffer, magic, 3);
    buffer += 3;

    uint8 version = *cast(uint8 *)buffer;
    buffer += 1;

    ptr_pack->asset_count = get_uint32le(buffer);
    buffer += 4;

    ptr_pack->pack_length = get_uint64le(buffer);
    buffer += 8;

    uint64 map_offset = get_uint64le(buffer);
    buffer += 8;

    // NOTE(rhett): Skip unk0 and signature. Move right to map
    buffer = buffer_begin + map_offset;

    // NOTE(rhett): Load asset info
    ptr_pack->name_hashes = cast(uint64 *)fl_alloc(ptr_pack->asset_count * 8);
    ptr_pack->asset_data = cast(Asset2_Data *)fl_alloc(ptr_pack->asset_count * sizeof(Asset2_Data));

    Asset2_Data *ptr_asset_data = 0;
    for (uint32 i = 0; i < ptr_pack->asset_count; ++i)
        {
        ptr_asset_data = &ptr_pack->asset_data[i];

        ptr_pack->name_hashes[i] = get_uint64le(buffer);
        buffer += 8;

        ptr_asset_data->data_offset = get_uint64le(buffer);
        buffer += 8;

        ptr_asset_data->raw_data_length = get_uint64le(buffer);
        buffer += 8;

        ptr_asset_data->zip_flag = get_uint32le(buffer);
        buffer += 4;

        // NOTE(rhett): Skip checksum
        buffer += 4;
        // printf("%#018llx\n", ptr_pack->name_hashes[i]);
        }
    }

// TODO(rhett): Should we take a pointer to a pack2?
fl_external Asset2_Data
fl_get_asset_by_hash(uint64 hash, Pack2 pack)
    {
    for (uint32 i = 0; i < pack.asset_count; ++i)
        {
        if (pack.name_hashes[i] == hash)
            {
            return pack.asset_data[i];
            }
        }

    Asset2_Data empty = {0};
    return empty;
    }

fl_external Asset2_Data
fl_get_asset_by_name(String8 name, Pack2 pack)
    {
    uint64 hash = fl_crc64(name);
    return fl_get_asset_by_hash(hash, pack);
    }



//----------------------------------------------------------------
// Entry point
//----------------------------------------------------------------
int
main(void)
    {
    printf("hello, world\n");
    Pack2 temp_pack = {0};
    fl_load_pack2(&temp_pack, cstring_to_string8("C:\\A\\Games\\PlanetSide 2 Test\\Resources\\Assets\\data_x64_0.pack2"));

    // uint64 test_hash = fl_crc64(cstring_to_string8("{NAMELIST}"));
    // printf("%llx\n", test_hash);

    // TODO(rhett): is String8 too much of a hassle here?
    uint64 test_hash = fl_crc64(cstring_to_string8("CLIENTITEMDEFINITIONS.TXT"));
    printf("%llu\n", test_hash);

    Asset2_Data test_asset = fl_get_asset_by_hash(test_hash, temp_pack);

    Asset2_Data test_asset2 = fl_get_asset_by_name(cstring_to_string8("VEHICLES.TXT"), temp_pack);

    return 0;
    }
