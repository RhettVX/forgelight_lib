// NOTE(rhett): Headers
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "src/fl_core.h"
#define FL_STRING_IMPLEMENTATION
#include "src/fl_string.h"
#include "src/fl_w32_utils.h"
#define MINIZ_NO_ARCHIVE_WRITING_APIS
#define MINIZ_NO_ARCHIVE_APIS
#include "src/external/miniz.h"
#include "src/fl_pack2.h"

// NOTE(rhett): Implementations
#include "src/external/crc64.c"
#include "src/fl_core.c"
#include "src/fl_w32_utils.c"
#include "src/external/miniz.c"
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
    ui8 *pack2_buffer = fl_alloc(FL_PACK2_BUFFER_SIZE);
    Pack2 temp_pack = fl_load_pack2("C:\\A\\Games\\PlanetSide 2 Test\\Resources\\Assets\\data_x64_0.pack2",
                                    pack2_buffer,
                                    FL_PACK2_BUFFER_SIZE);

    ZeroMemory(pack2_buffer, FL_PACK2_BUFFER_SIZE);


    // uint64 test_hash = fl_crc64(cstring_to_string8("{NAMELIST}"));
    // printf("%llx\n", test_hash);

    // TODO(rhett): is String8 too much of a hassle here?
    // ui64 test_hash = fl_crc64("CLIENTITEMDEFINITIONS.TXT");
    // printf("%llu\n", test_hash);

    // Asset2_Data test_asset = fl_get_asset_by_hash(test_hash, temp_pack);

    Asset2 *test_asset2 = fl_get_asset2_by_name("VEHICLES.TXT", temp_pack);
    if (test_asset2 == 0)
        {
        return -1;
        }

    if (!w32_read_entire_file(temp_pack.pack_path.content, pack2_buffer, FL_PACK2_BUFFER_SIZE))
        {
        return -1;
        }

    pack2_buffer += test_asset2->data_offset;

    ui8 *asset_buffer = fl_read_asset2(test_asset2, pack2_buffer);

    // ui8 *asset_buffer = fl_alloc(test_asset2.r)
    #endif

    return 0;
    }