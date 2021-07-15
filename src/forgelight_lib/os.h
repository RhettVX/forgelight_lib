#ifndef OS_H
#define OS_H


//// Memory functions
extern void* 
os_memory_alloc(u32 size);

extern b32
os_memory_free(void* pointer);

//// File functions
extern b32
os_read_entire_file(char* file_path, u8* buffer, u32 max_size);

extern b32
os_write_buffer_to_file(char* file_path, u8* buffer, u32 buffer_length);

extern b32
os_create_folder(char* folder_path);

//================================================================
#ifdef FL_WIN32
    #ifdef FL_DEBUG
        extern void* 
        win32_memory_alloc(u32 size, uint line, char* file);

        extern b32
        win32_memory_free(void* pointer, uint line, char* file);
    #else
        extern void* 
        win32_memory_alloc(u32 size);

        extern b32
        win32_memory_free(void* pointer);
    #endif // FL_DEBUG

    extern b32
    win32_read_entire_file(char* file_path, u8* buffer, u32 max_size);

    extern b32
    win32_write_buffer_to_file(char* file_path, u8* buffer, u32 buffer_length);

    extern b32
    win32_create_folder(char* folder_path);
#endif // FL_WIN32

//================================================================
#ifdef FL_DEBUG
    #define os_memory_alloc(sz) win32_memory_alloc(sz,__LINE__,__FILE__)
    #define os_memory_free(ptr) win32_memory_free(ptr,__LINE__,__FILE__)
#else
    #define os_memory_alloc(sz) win32_memory_alloc(sz)
    #define os_memory_free(ptr) win32_memory_free(ptr)
#endif // FL_DEBUG

#define os_read_entire_file(s,ptr,sz)     win32_read_entire_file(s,ptr,sz)
#define os_write_buffer_to_file(s,ptr,sz) win32_write_buffer_to_file(s,ptr,sz)
#define os_create_folder(s)               win32_create_folder(s)


#endif // OS_H
