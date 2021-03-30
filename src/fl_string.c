//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
fl_internal uint32
cstring_length(char *cstring)
    {
    uint32 count = 0;
    while (*(cstring++))
        {
            ++count;
        }
    return count;
    }

// TODO(rhett): Maybe don't allocate in here.
fl_internal String8
cstring_to_string8(char *cstring)
    {
    String8 result = {0};
    result.size = cstring_length(cstring)+1;
    result.content = fl_alloc(result.size);
    fl_memcpy(cstring, result.content, result.size);

    return result;
    }