#include <windows.h>

#include "../forgelight_lib.h"


//----------------------------------------------------------------
// Memory functions
//----------------------------------------------------------------
u8 *
win32_memory_alloc(u32 size)
    {
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
    }

b32
win32_memory_free(void *pointer)
    {
    return HeapFree(GetProcessHeap(), 0, pointer);
    }


//----------------------------------------------------------------
// File functions
//----------------------------------------------------------------
// TODO(rhett): This only reads a 4gb file, but the pack2 format can handle larger.
// TODO(rhett): Maybe use a result variable.
b32
win32_read_entire_file(char *file_path, u8 *buffer, u32 max_size)
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

b32
win32_write_buffer_to_file(char *file_path, u8 *buffer, u32 buffer_length)
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
        printf("Unable to open file: %s\n", file_path);
        return 0;
        }


    if (!buffer)
        {
        printf("Buffer is empty.\n");
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
        printf("Unable to write file \"%s\" - GetLastError(%d)\n",
                  file_path,
                  GetLastError());
        CloseHandle(file_handle);
        return 0;
        }

    CloseHandle(file_handle);
    return 1;
    }

b32
win32_create_folder(char *folder_path)
    {
    if (!CreateDirectory(folder_path, 0))
        {
        printf("CreateDirectory error on \"%s\" - GetLastError(%d)\n",
                  folder_path,
                  GetLastError());
        return 0;
        }

    return 1;
    }