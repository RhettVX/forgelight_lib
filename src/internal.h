#ifndef INTERNAL_H
#define INTERNAL_H

#include <stdarg.h> // NOTE(rhett): util_log_printf


//----------------------------------------------------------------
// External libs
//----------------------------------------------------------------
extern u64
crc64(u64 crc, char const* s, u64 l);


//----------------------------------------------------------------
// Strings
//----------------------------------------------------------------
extern u32
strings_cstring_length(char* cstring);

extern String8
strings_cstring_to_string8(char* cstring);


//----------------------------------------------------------------
// Util
//----------------------------------------------------------------
extern void
util_log_printf(char* format_string, ...);

extern void
util_byte_dump(void* data, uint length);


//----------------------------------------------------------------
// Debug
//----------------------------------------------------------------
#ifdef FL_DEBUG
    #define FL_DEBUG_MAX_ALLOCATIONS 256

    typedef struct AllocationEntry AllocationEntry;
    struct AllocationEntry
        {
        void* pointer;
        b32   has_been_freed;
        char* origin_file;
        uint  origin_line;
        };

    typedef struct AllocationTable AllocationTable;
    struct AllocationTable
        {
        uint            count;
        uint            capacity;
        AllocationEntry entries[FL_DEBUG_MAX_ALLOCATIONS];
        };

    extern void
    debug_allocation_register(void* pointer, uint line, char* file);

    extern void
    debug_allocation_mark_as_freed(void* pointer, uint line, char* file);

    extern AllocationEntry*
    debug_allocation_get_by_pointer(void* pointer);

    // NOTE(rhett): Also externed in forgelight_lib.h
    extern void
    debug_allocation_check_for_unfreed_memory();
#endif // FL_DEBUG


#endif // INTERNAL_H
