//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
fl_internal void
memory_copy(ui8 *source, ui8 *destination, ui32 size)
    {
    while(size--) // NOTE(rhett): Must be postfix
        {
        *(destination + size) = *(source + size);
        }
    }

//// Little endian
fl_internal ui32
get_ui32le(ui8 *data)
    {
    ui32 result = 0;
    for (ui32 i = 4; i > 0; --i)
        {
        result <<= 8;
        result |= *(data + (i - 1));
        }

    return result;
    }

fl_internal ui64
get_ui64le(ui8 *data)
    {
    ui64 result = 0;
    for (ui64 i = 8; i > 0; --i)
        {
        result <<= 8;
        result |= *(data + (i - 1));
        }

    return result;
    }

//// Big endian
fl_internal ui16
get_ui16be(ui8 *data)
    {
    ui16 result = 0;
    for (ui32 i = 0; i < 2; ++i)
        {
        result <<= 8;
        result |= *(data + i);
        }

    return result;
    }

fl_internal ui32
get_ui32be(ui8 *data)
    {
    ui32 result = 0;
    for (ui32 i = 0; i < 4; ++i)
        {
        result <<= 8;
        result |= *(data + i);
        }

    return result;
    }