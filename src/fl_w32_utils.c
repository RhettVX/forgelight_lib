//----------------------------------------------------------------
// Memory functions
//----------------------------------------------------------------
fl_internal uint8 *
w32_memory_alloc(uint32 size)
    {
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
    }

fl_internal bool32
w32_memory_free(uint8 *pointer)
    {
    return HeapFree(GetProcessHeap(), 0, pointer);
    }


//----------------------------------------------------------------
// File functions
//----------------------------------------------------------------
// TODO(rhett): This only reads a 4gb file, but the pack2 format can handle larger.
fl_internal uint8 *
w32_read_entire_file(String8 file_path)
    {
    uint8 *result = 0;
    HANDLE file_handle = CreateFile(cast(char const *)file_path.content,
                                   GENERIC_READ,
                                   FILE_SHARE_READ,
                                   0,
                                   OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL,
                                   0);

    if (file_handle == INVALID_HANDLE_VALUE)
        {
        return result;
        }

    // get file size
    uint32 file_size = GetFileSize(file_handle, 0);

    // Setup buffer
    result = fl_alloc(file_size);

    // NOTE(rhett): Read entire file into result
    uint32 bytes_read = 0;
    ReadFile(file_handle, result, file_size, &bytes_read, 0);

    CloseHandle(file_handle);
    return result;
    }