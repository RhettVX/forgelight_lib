#include "forgelight_lib.h"


int
main(void)
    {
    #if 1 // Let's try the real deal now

    pack2_export_assets_as_files("D:\\WindowsUsers\\Rhett\\Desktop\\forgelight-toolbox\\Backups\\07-28-21-TEST\\Resources\\Assets\\OutfitWars_x64_1.pack2",
                                 "unpacked_assets",
                                 "D:\\WindowsUsers\\Rhett\\Desktop\\forgelight-toolbox\\merged.txt");

    #endif

    #if 0 // Combining pack2 stuff with namelist stuff

    // u8* pack2_file_buffer = malloc(FL_PACK2_BUFFER_SIZE);
    // Pack2 test_pack2 = pack2_load_from_file("D:\\WindowsUsers\\Rhett\\Documents\\Projects\\forgelight_lib\\test\\example_pack2.pack2",
    //                                         pack2_file_buffer,
    //                                         FL_PACK2_BUFFER_SIZE);

    // Pack2_Namelist test_namelist = pack2_names_generate_namelist_from_file("P:\\forgelight_lib\\test\\example_namelist.txt",
    //                                                                        FL_NAMELIST_FILE_BUFFER_SIZE);

    // pack2_consolidate_with_namelist(&test_pack2, test_namelist);

    // TODO(rhett): This may work better as a different function
    pack2_export_assets_as_files("D:\\WindowsUsers\\Rhett\\Documents\\Projects\\forgelight_lib\\test\\example_pack2.pack2",
                                 "unpacked_assets",
                                 "P:\\forgelight_lib\\test\\example_namelist.txt");

    #endif

    #if 0 // Pack2 stuff
    pack2_export_assets_as_files("D:\\WindowsUsers\\Rhett\\Documents\\Projects\\forgelight_lib\\test\\example_pack2.pack2",
                                 "unpacked_assets");
    #endif

    #if 0 // Namelist stuff

    // String8 example_strings[] = {strings_cstring_to_string8("test1.txt"),
    //                              strings_cstring_to_string8("test2.txt"),
    //                              strings_cstring_to_string8("test3.txt"),
    //                              strings_cstring_to_string8("test4.txt"),
    //                              strings_cstring_to_string8("test5.txt"),
    //                              strings_cstring_to_string8("{namelist}")};

    // Pack2_Namelist example_namelist = pack2_names_generate_namelist_from_string_list(example_strings, array_count(example_strings));
    // Pack2_Namelist example_namelist = pack2_names_generate_namelist_from_file("D:\\WindowsUsers\\Rhett\\Desktop\\Python Scripts\\namelist.txt", MB(50));
    Pack2_Namelist example_namelist = pack2_names_generate_namelist_from_file("P:\\forgelight_lib\\test\\example_namelist.txt", MB(50));

    for (u32 i = 0; i < example_namelist.count; ++i)
        {
        String8 name = example_namelist.entries[i].name;
        char temp_buffer[FL_NAMELIST_MAX_NAME_LENGTH] = {0};
        memcpy(name.content, temp_buffer, name.length);
        printf(":%u: %s\n", i, temp_buffer);
        }

    free(example_namelist.entries);
    free(example_namelist.raw_data_ptr);
    #endif
    
    #ifdef FL_DEBUG
    debug_allocation_check_for_unfreed_memory();
    #endif // FL_DEBUG
    
    return 0;
    }
