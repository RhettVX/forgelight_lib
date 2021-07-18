#include "forgelight_lib.h"


int
main(void)
    {
    // pack2_export_assets_as_files("C:\\A\\Games\\PlanetSide 2 Test\\Resources\\Assets\\data_x64_0.pack2",
    //                              "unpacked_assets");

    // String8 example_strings[] = {strings_cstring_to_string8("test1.txt"),
    //                              strings_cstring_to_string8("test2.txt"),
    //                              strings_cstring_to_string8("test3.txt"),
    //                              strings_cstring_to_string8("test4.txt"),
    //                              strings_cstring_to_string8("test5.txt"),
    //                              strings_cstring_to_string8("{namelist}")};

    // Pack2_Namelist example_namelist = pack2_names_generate_namelist_from_string_list(example_strings, array_count(example_strings));
    Pack2_Namelist example_namelist = pack2_names_generate_namelist_from_file("D:\\WindowsUsers\\Rhett\\Desktop\\Python Scripts\\namelist.txt", MB(50));
    
    #ifdef FL_DEBUG
    debug_allocation_check_for_unfreed_memory();
    #endif // FL_DEBUG
    
    return 0;
    }
