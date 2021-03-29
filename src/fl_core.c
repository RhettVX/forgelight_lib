//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
fl_internal void
memory_copy(uint8 *source, uint8 *destination, uint32 size)
    {
    while(size--) // NOTE(rhett): Must be postfix
        {
        *(destination + size) = *(source + size);
        }
    }

fl_internal uint32
get_uint32le(uint8 *data)
    {
    uint32 result = 0;
    for (uint32 i = 4; i > 0; --i)
        {
        result <<= 8;
        result |= *(data + (i - 1));
        }

    return result;
    }

fl_internal uint64
get_uint64le(uint8 *data)
    {
    uint64 result = 0;
    for (uint64 i = 8; i > 0; --i)
        {
        result <<= 8;
        result |= *(data + (i - 1));
        }

    return result;
    }