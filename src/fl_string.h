#ifndef FL_STRING_H
#define FL_STRING_H


//----------------------------------------------------------------
// Structures
//----------------------------------------------------------------
typedef struct String8 String8;
struct String8
    {
    uint32  size;
    uint8  *content;
    };


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
fl_internal uint32
cstring_length(char *cstring);

fl_internal String8
cstring_to_string8(char *cstring);


#endif // FL_STRING_H