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