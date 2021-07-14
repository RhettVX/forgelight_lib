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

    }
