//================================================================
#ifndef FL_W32_UTILS_H
#define FL_W32_UTILS_H


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
//// Memory functions
fl_internal u8 *
fl_w32_memory_alloc(u32 size);

fl_internal b32
fl_w32_memory_free(u8 *pointer);

//// File functions
fl_internal b32
fl_w32_read_entire_file(char *file_path, u8 *buffer, u32 max_size);

fl_internal b32
fl_w32_write_buffer_to_file(char *file_path, u8 *buffer, u32 buffer_length);

fl_internal b32
fl_w32_create_folder(char *folder_path);


#endif // FL_W32_UTILS_H
//================================================================

//================================================================
#ifdef FL_W32_UTILS_IMPL


//----------------------------------------------------------------
// Memory functions
//----------------------------------------------------------------
fl_internal u8 *
fl_w32_memory_alloc(u32 size)
    {
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
    }

fl_internal b32
fl_w32_memory_free(u8 *pointer)
    {
    return HeapFree(GetProcessHeap(), 0, pointer);
    }


//----------------------------------------------------------------
// File functions
//----------------------------------------------------------------
// TODO(rhett): This only reads a 4gb file, but the pack2 format can handle larger.
// TODO(rhett): Maybe use a result variable.
fl_internal b32
fl_w32_read_entire_file(char *file_path, u8 *buffer, u32 max_size)
    {
    HANDLE file_handle = CreateFile(file_path,
                                    GENERIC_READ,
                                    FILE_SHARE_READ,
                                    0,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    0);

    if (file_handle == INVALID_HANDLE_VALUE)
        {
        return 0;
        }

    // get file size
    u32 file_size = GetFileSize(file_handle, 0);
    if (file_size > max_size)
        {
        return 0;
        }

    if (!buffer)
        {
        return 0;
        }

    // NOTE(rhett): Read entire file into buffer
    u32 bytes_read = 0;
    ReadFile(file_handle, buffer, file_size, &bytes_read, 0);

    CloseHandle(file_handle);
    return 1;
    }

fl_internal b32
fl_w32_write_buffer_to_file(char *file_path, u8 *buffer, u32 buffer_length)
    {
    HANDLE file_handle = CreateFile(file_path,
                                    GENERIC_WRITE,
                                    0,
                                    0,
                                    CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL,
                                    0);
    if (file_handle == INVALID_HANDLE_VALUE)
        {
        fl_printf("Unable to open file: %s\n", file_path);
        return 0;
        }


    if (!buffer)
        {
        fl_printf("Buffer is empty.\n");
        CloseHandle(file_handle);
        return 0;
        }

    DWORD bytes_written = 0;    
    if (!WriteFile(file_handle,
                   buffer,
                   buffer_length,
                   &bytes_written,
                   0))
        {
        fl_printf("Unable to write file \"%s\" - GetLastError(%d)\n",
                  file_path,
                  GetLastError());
        CloseHandle(file_handle);
        return 0;
        }

    CloseHandle(file_handle);
    return 1;
    }

fl_internal b32
fl_w32_create_folder(char *folder_path)
    {
    if (!CreateDirectory(folder_path, 0))
        {
        fl_printf("CreateDirectory error on \"%s\" - GetLastError(%d)\n",
                  folder_path,
                  GetLastError());
        return 0;
        }

    return 1;
    }


#endif // FL_W32_UTILS_IMPL
//================================================================
