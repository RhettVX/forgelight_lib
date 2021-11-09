#include "forgelight_lib.h"


//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
u32
strings_cstring_length(char* cstring)
    {
    u32 count = 0;
    while (*(cstring++))
        {
            ++count;
        }
    return count;
    }

String8
strings_cstring_to_string8(char* cstring)
    {
    String8 result = {0};
    result.length = strings_cstring_length(cstring);
    result.content = cstring;

    return result;
    }

// fl_internal void
// cstring_make_upper(char* cstring)
//     {
//     while(*cstring != 0)
//         {
//         if (*cstring >= 'a' &&* cstring <= 'z')
//             {
//            * cstring ^= 0b00100000;
//             }
//         ++cstring;
//         }
//     }
