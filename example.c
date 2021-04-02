// NOTE(rhett): Headers
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "src/fl_core.h"
#include "src/fl_string.h"
#include "src/fl_w32_utils.h"
#include "src/fl_pack2.h"

// NOTE(rhett): Implementations
#include "src/external/crc64.c"
#include "src/fl_core.c"
#include "src/fl_string.c"
#include "src/fl_w32_utils.c"
#include "src/fl_pack2.c"

// TODO(rhett): I'm not so sure we need to use String8 so much. maybe just for storing in another struct


//----------------------------------------------------------------
// Entry point
//----------------------------------------------------------------
int
main(void)
    {
    printf("hello, world\n");


    #if 1
    Pack2 temp_pack = {0};
    fl_load_pack2(&temp_pack, "C:\\A\\Games\\PlanetSide 2 Test\\Resources\\Assets\\data_x64_0.pack2");

    // uint64 test_hash = fl_crc64(cstring_to_string8("{NAMELIST}"));
    // printf("%llx\n", test_hash);

    // TODO(rhett): is String8 too much of a hassle here?
    ui64 test_hash = fl_crc64("CLIENTITEMDEFINITIONS.TXT");
    printf("%llu\n", test_hash);

    // Asset2_Data test_asset = fl_get_asset_by_hash(test_hash, temp_pack);

    i32 test_asset2 = fl_get_asset_index_by_name("VEHICLES.TXT", temp_pack);
    if (test_asset2 == FL_ASSET_NOT_FOUND)
        {
        return 1;
        }

    fl_unpack_asset_from_pack2(test_asset2, temp_pack);
    #endif

    return 0;
    }