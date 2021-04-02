#ifndef FL_STRING_H
#define FL_STRING_H


//----------------------------------------------------------------
// Structures
//----------------------------------------------------------------
typedef struct String8 String8;
struct String8
    {
    ui32  size;    // NOTE(rhett): Includes null-terminator.
    char *content; // NOTE(rhett): Null-terminated string.
    };


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
fl_internal ui32
cstring_length(char *cstring);

fl_internal String8
cstring_to_string8(char *cstring);


#endif // FL_STRING_H