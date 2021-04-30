#ifndef FL_STRING_H
#define FL_STRING_H


//----------------------------------------------------------------
// Structures
//----------------------------------------------------------------
typedef struct String8 String8;
struct String8
    {
    ui32  length;
    ui8  *content;
    };


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
fl_internal ui32
cstring_length(char *cstring);

fl_internal String8
cstring_to_string8(char *cstring);

// fl_internal void
// cstring_make_upper(char *cstring);


#endif // FL_STRING_H


#ifdef FL_STRING_IMPLEMENTATION


//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
fl_internal ui32
cstring_length(char *cstring)
    {
    ui32 count = 0;
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


#endif // FL_STRING_IMPLEMENTATION