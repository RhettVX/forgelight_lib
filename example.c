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


//----------------------------------------------------------------
// Entry point
//----------------------------------------------------------------
int
main(void)
    {
    ui8 *pack2_buffer = fl_alloc(FL_PACK2_BUFFER_SIZE);
    Pack2 temp_pack = fl_load_pack2("C:\\A\\Games\\PlanetSide 2 Test\\Resources\\Assets\\data_x64_0.pack2",
                                    pack2_buffer,
                                    FL_PACK2_BUFFER_SIZE);

    ZeroMemory(pack2_buffer, FL_PACK2_BUFFER_SIZE);

    Asset2 test_asset2 = fl_get_asset2_by_name("Vehicles.txt", temp_pack);
    if (test_asset2.name_hash == 0)
        {
        printf("Asset not found.\n");
        return -1;
        }

    w32_read_entire_file(fl_cast(char *)temp_pack.pack_path.content,
                         pack2_buffer,
                         FL_PACK2_BUFFER_SIZE);

    pack2_buffer += test_asset2.data_offset;
    ui8 *asset_buffer = fl_alloc(FL_ASSET2_BUFFER_SIZE);
    test_asset2 = fl_read_asset2(test_asset2, pack2_buffer, asset_buffer, FL_ASSET2_BUFFER_SIZE);

    return 0;
    }