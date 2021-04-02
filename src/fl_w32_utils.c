//----------------------------------------------------------------
// Memory functions
//----------------------------------------------------------------
fl_internal ui8 *
w32_memory_alloc(ui32 size)
    {
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
    }

fl_internal b32
w32_memory_free(ui8 *pointer)
    {
    return HeapFree(GetProcessHeap(), 0, pointer);
    }


//----------------------------------------------------------------
// File functions
//----------------------------------------------------------------
// TODO(rhett): This only reads a 4gb file, but the pack2 format can handle larger.
fl_internal ui8 *
w32_read_entire_file(char *file_path)
    {
    ui8 *result = 0;
    HANDLE file_handle = CreateFile(file_path,
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
    ui32 file_size = GetFileSize(file_handle, 0);

    // Setup buffer
    result = fl_alloc(file_size);

    // NOTE(rhett): Read entire file into result
    ui32 bytes_read = 0;
    ReadFile(file_handle, result, file_size, &bytes_read, 0);

    CloseHandle(file_handle);
    return result;
    }