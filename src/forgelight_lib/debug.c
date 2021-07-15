#ifdef FL_DEBUG

#include "../forgelight_lib.h"
#include "internal.h"


AllocationTable debug_allocation_table_global = {.tail = 0,
                                                 .capacity = MAX_ALLOCATIONS,
                                                 .allocation_array = {0}};

void
debug_allocation_register(void* pointer, uint line, char* file)
    {
    if (debug_allocation_table_global.tail == debug_allocation_table_global.capacity)
        {
        printf("[X] Exceeded max allocations! capacity=%u, tail=%u, line=%u, file=%s\n",
               debug_allocation_table_global.capacity,
               debug_allocation_table_global.tail,
               line,
               file);

        // TODO(rhett): Not sure about this, but it should work for now
        abort();
        }

    AllocationEntry entry = {pointer, 0};
    debug_allocation_table_global.allocation_array[debug_allocation_table_global.tail] = entry;
    ++debug_allocation_table_global.tail;
    return;
    }

void
debug_allocation_mark_as_freed(void* pointer, uint line, char* file)
    {
    // NOTE(rhett): Attempt to free pointer that hasn't been allocated
    AllocationEntry* entry = debug_allocation_get_by_pointer(pointer);
    if (entry == 0)
        {
        printf("[X] Attempting to free a pointer that has not been allocated! address=%p, line=%u, file=%s\n",
               pointer,
               line,
               file);
        abort();
        }

    // NOTE(rhett): Attempt to free a pointer that has already been freed
    if (entry->has_been_freed)
        {
        printf("[X] Attempting to free a pointer that has already been freed! address=%p, line=%u, file=%s\n",
               pointer,
               line,
               file);
        abort();
        }

    // NOTE(rhett): should be able to mark as freed now
    entry->has_been_freed = 1;
    }

AllocationEntry*
debug_allocation_get_by_pointer(void* pointer)
    {
    for (uint i = 0; i < debug_allocation_table_global.tail; ++i)
        {
        if (debug_allocation_table_global.allocation_array[i].pointer == pointer)
            {
            return &debug_allocation_table_global.allocation_array[i];
            }
        }
    // TODO(rhett): Should this be a fatal error alone?
    printf("[!] Unable to get allocation entry by pointer! address=%p\n", pointer);
    return 0;
    }

#endif // FL_DEBUG
