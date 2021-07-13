// NOTE(rhett): Headers
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "external/miniz.c"
#include "external/crc64.c"

#include "forgelight_lib.h"


int
main(void)
    {
    pack2_export_assets_from_pack2("C:\\A\\Games\\PlanetSide 2 Test\\Resources\\Assets\\data_x64_0.pack2",
                                   "unpacked_assets");
    
    return 0;
    }
