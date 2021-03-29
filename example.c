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

// TODO(rhett): I'm not so sure we need to use String8 so much


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