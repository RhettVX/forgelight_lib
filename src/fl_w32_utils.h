#ifndef FL_W32_UTILS_H
#define FL_W32_UTILS_H


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
//// Memory functions
fl_internal uint8 *
w32_memory_alloc(uint32 size);

fl_internal bool32
w32_memory_free(uint8 *pointer);

//// File functions
fl_internal uint8 *
w32_read_entire_file(String8 file_path);


#endif // FL_W32_UTILS_H