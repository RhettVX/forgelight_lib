// NOTE(rhett): Headers
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "library/external/miniz.c"
#include "library/external/crc64.c"

#define FL_BASE_IMPL
#include "library/fl_base.h"
#define FL_STRING_IMPL
#include "library/fl_string.h"
#define FL_W32_UTILS_IMPL
#include "library/fl_w32_utils.h"
#define MINIZ_NO_ARCHIVE_WRITING_APIS
#define MINIZ_NO_ARCHIVE_APIS
#include "library/external/miniz.h"
#define FL_PACK2_IMPL
#include "library/fl_pack2.h"


//----------------------------------------------------------------
// Entry point
//----------------------------------------------------------------
int
main(void)
    {
#ifdef FL_STUFF_TEST
    u8 *pack2_buffer = fl_alloc(FL_PACK2_BUFFER_SIZE);
    fl_Pack2 temp_pack = fl_load_pack2("C:\\A\\Games\\PlanetSide 2 Test\\Resources\\Assets\\data_x64_0.pack2",
                                    pack2_buffer,
                                    FL_PACK2_BUFFER_SIZE);

    ZeroMemory(pack2_buffer, FL_PACK2_BUFFER_SIZE);

    fl_Asset2 test_asset2 = fl_get_asset2_by_name("Vehicles.txt", temp_pack);
    if (test_asset2.name_hash == 0)
        {
        printf("Asset not found.\n");
        return -1;
        }

    fl_w32_read_entire_file(fl_cast(char *)temp_pack.pack_path.content,
                         pack2_buffer,
                         FL_PACK2_BUFFER_SIZE);

    pack2_buffer += test_asset2.data_offset;
    u8 *asset_buffer = fl_alloc(FL_ASSET2_BUFFER_SIZE);
    test_asset2 = fl_read_asset2(test_asset2, pack2_buffer, asset_buffer, FL_ASSET2_BUFFER_SIZE);
    fl_eval_print_u32(test_asset2.name_hash);

    b32 folder_created = fl_w32_create_folder("test folder");
    fl_eval_print_u32(folder_created);

    b32 retval = fl_w32_write_buffer_to_file("test folder\\test_file.txt",
                                             asset_buffer,
                                             test_asset2.unzipped_data_length);
    fl_eval_print_u32(retval);
    return 0;
#else
    fl_export_assets_from_pack2("C:\\A\\Games\\PlanetSide 2 Test\\Resources\\Assets\\data_x64_0.pack2",
                                "unpacked_assets");
    
    return 0;
#endif
    }
