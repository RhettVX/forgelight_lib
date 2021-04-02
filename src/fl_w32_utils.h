#ifndef FL_W32_UTILS_H
#define FL_W32_UTILS_H


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
//// Memory functions
fl_internal ui8 *
w32_memory_alloc(ui32 size);

fl_internal b32
w32_memory_free(ui8 *pointer);

//// File functions
fl_internal ui8 *
w32_read_entire_file(char *file_path);


#endif // FL_W32_UTILS_H