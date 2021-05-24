#ifndef FL_STRING_H
#define FL_STRING_H


//----------------------------------------------------------------
// Structures
//----------------------------------------------------------------
typedef struct String8 String8;
struct String8
    {
    u32  length;
    u8  *content;
    };


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
fl_internal u32
cstring_length(char *cstring);

fl_internal String8
cstring_to_string8(char *cstring);

// fl_internal void
// cstring_make_upper(char *cstring);


#endif // FL_STRING_H


#ifdef FL_STRING_IMPL


//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
fl_internal u32
cstring_length(char *cstring)
    {
    u32 count = 0;
    while (*(cstring++))
        {
            ++count;
        }
    return count;
    }

fl_internal String8
cstring_to_string8(char *cstring)
    {
    String8 result = {0};
    result.length = cstring_length(cstring);
    result.content = cstring;

    return result;
    }

// fl_internal void
// cstring_make_upper(char *cstring)
//     {
//     while(*cstring != 0)
//         {
//         if (*cstring >= 'a' && *cstring <= 'z')
//             {
//             *cstring ^= 0b00100000;
//             }
//         ++cstring;
//         }
//     }


#endif // FL_STRING_IMPL
