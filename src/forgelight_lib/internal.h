#ifndef INTERNAL_H
#define INTERNAL_H


// TODO(rhett): Name this better
//----------------------------------------------------------------
// External libs
//----------------------------------------------------------------
extern u64
crc64(u64 crc, char const* s, u64 l);


//----------------------------------------------------------------
// Endian IO
//----------------------------------------------------------------
//// Little endian
extern u32
endian_get_u32_le(u8* data);

extern u64
endian_get_u64_le(u8* data);

//// Big endian
extern u16
endian_get_u16_be(u8* data);

extern u32
endian_get_u32_be(u8* data);


//----------------------------------------------------------------
// Debug
//----------------------------------------------------------------
#ifdef FL_DEBUG

    #define MAX_ALLOCATIONS 256

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
        AllocationEntry entries[MAX_ALLOCATIONS];
        };

    // extern AllocationTable debug_allocation_table_global;

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
