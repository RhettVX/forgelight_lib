// NOTE(rhett): Headers
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forgelight_lib.h"


int
main(void)
    {
    pack2_export_assets_as_files("C:\\A\\Games\\PlanetSide 2 Test\\Resources\\Assets\\data_x64_0.pack2",
                                 "unpacked_assets");
    
    return 0;
    }
