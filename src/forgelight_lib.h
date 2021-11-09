#ifndef FORGELIGHT_LIB_H
#define FORGELIGHT_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


//----------------------------------------------------------------
// Base
//----------------------------------------------------------------
//// Redefinitions
#define internal      static
#define local_persist static
#define global        static
#define dll_external  extern __declspec(dllexport)

#define calloc(n,sz)         calloc(n,sz)
#define malloc(sz)           os_memory_alloc(sz)
#define free(ptr)            os_memory_free(ptr)
#define memcpy(src,dst,sz)   memcpy(dst,src,sz)
#define memcmp(ptr1,ptr2,sz) memcmp(ptr1,ptr2,sz)
#define memset(ptr,val,sz)   memset(ptr,val,sz)


//// Macros
#define KB(n) (n << 10)
#define MB(n) (n << 20)
#define GB(n) ((u64)n << 30)
#define TB(n) ((u64)n << 40)

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)

#define EVAL_PRINT_U8(x)   printf("%s = %u:%xh\n",#x,(u8)x,(u8)x)
#define EVAL_PRINT_I8(x)   printf("%s = %d:%xh\n",#x,(i8)x,(i8)x)
#define EVAL_PRINT_U16(x)  printf("%s = %u:%xh\n",#x,(u16)x,(u16)x)
#define EVAL_PRINT_I16(x)  printf("%s = %d:%xh\n",#x,(i16)x,(i16)x)
#define EVAL_PRINT_U32(x)  printf("%s = %u:%xh\n",#x,(u32)x,(u32)x)
#define EVAL_PRINT_I32(x)  printf("%s = %d:%xh\n",#x,(i32)x,(i32)x)
#define EVAL_PRINT_U64(x)  printf("%s = %llu:%llxh\n",#x,(u64)x,(u64)x)
#define EVAL_PRINT_I64(x)  printf("%s = %lld:%llxh\n",#x,(i64)x,(i64)x)
#define EVAL_PRINT_CSTR(x) printf("%s = \"%s\"\n",#x,x)

#define ARRAY_COUNT(a)     sizeof(a) / sizeof(a[0])


//// Types
typedef   int8_t i8;
typedef  int16_t i16;
typedef  int32_t i32;
typedef  int64_t i64;
typedef intptr_t iptr;

typedef      uint8_t u8;
typedef     uint16_t u16;
typedef     uint32_t u32;
typedef     uint64_t u64;
typedef    uintptr_t uptr;
typedef unsigned int uint;

typedef  i8 b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;

typedef  float f32;
typedef double f64;

typedef struct String8 String8;
struct String8
    {
    u32 length;
    u8* content;
    };


//----------------------------------------------------------------
// OS
//----------------------------------------------------------------
#define OS_MAX_PATH_LENGTH 256


//// Memory functions
extern void* 
os_memory_alloc(u32 size);

extern b32
os_memory_free(void* pointer);

//// File functions
extern b32
os_load_entire_file(char* file_path, u8* buffer, u32 max_size);

extern b32
os_write_buffer_to_file(char* file_path, u8* buffer, u32 buffer_length);

extern b32
os_create_folder(char* folder_path);

//// Other functions
extern String8
os_local_time_as_string8();

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
    win32_load_entire_file(char* file_path, u8* buffer, u32 max_size);

    extern b32
    win32_write_buffer_to_file(char* file_path, u8* buffer, u32 buffer_length);

    extern b32
    win32_create_folder(char* folder_path);

    extern String8
    win32_local_time_as_string8();
#endif // FL_WIN32

//================================================================
#ifdef FL_DEBUG
    #define os_memory_alloc(sz) win32_memory_alloc(sz,__LINE__,__FILE__)
    #define os_memory_free(ptr) win32_memory_free(ptr,__LINE__,__FILE__)
#else
    #define os_memory_alloc(sz) win32_memory_alloc(sz)
    #define os_memory_free(ptr) win32_memory_free(ptr)
#endif // FL_DEBUG

#define os_load_entire_file(s,ptr,sz)     win32_load_entire_file(s,ptr,sz)
#define os_write_buffer_to_file(s,ptr,sz) win32_write_buffer_to_file(s,ptr,sz)
#define os_create_folder(s)               win32_create_folder(s)
#define os_local_time_as_string8()        win32_local_time_as_string8()  


//----------------------------------------------------------------
// Endian IO
//----------------------------------------------------------------
//// Little endian
extern u16
endian_get_u16_le(u8* data);

extern u32
endian_get_u32_le(u8* data);

extern u64
endian_get_u64_le(u8* data);

extern f32
endian_get_f32_le(u8* data);

//// Big endian
extern u16
endian_get_u16_be(u8* data);

extern u32
endian_get_u32_be(u8* data);


//----------------------------------------------------------------
// Pack2
//----------------------------------------------------------------
#define FL_PACK2_BUFFER_SIZE  MB(500)
#define FL_ASSET2_BUFFER_SIZE MB(100)

#define FL_NAMELIST_FILE_BUFFER_SIZE MB(50)
// TODO(rhett): If I reduce the amount of garbage strings that get scraped, we can save a ton of space
#define FL_NAMELIST_MAX_NAME_AMOUNT  2000000 // * sizeof(Pack2_NamelistEntry) 
#define FL_NAMELIST_MAX_NAME_LENGTH  256

typedef struct Pack2_NamelistEntry Pack2_NamelistEntry;
struct Pack2_NamelistEntry
    {
    u64     hash;
    String8 name;
    };

typedef struct Pack2_Namelist Pack2_Namelist;
struct Pack2_Namelist
    {
    uint                 count;
    uint                 capacity;
    Pack2_NamelistEntry* entries;
    // NOTE(rhett): Keeping a pointer to the loaded file in memory.
    u8*                  raw_data_ptr;
    };

typedef struct Asset2 Asset2;
struct Asset2
    {
    u64 name_hash;
    u64 data_offset;
    u64 raw_data_length;
    u32 unzipped_data_length;
    u32 zip_flag;
    };

typedef struct Pack2 Pack2;
struct Pack2
    {
    String8              pack_path;
    u32                  asset_count;
    u64                  pack_length;
    Asset2*              asset2s;
    Pack2_NamelistEntry* asset2_names;
    };

//// Helper functions
extern u64
pack2_names_calculate_hash(String8 to_hash);

// TODO(rhett): Redo this
// extern Pack2_Namelist
// pack2_names_generate_namelist_from_string_list(String8* string_list, uint string_count);

extern Pack2_Namelist
pack2_names_generate_namelist_from_file(char* file_path, u32 file_buffer_size);

//// Main functions
extern Pack2
pack2_load_from_file(char* pack_path, u8* pack2_buffer, u32 pack2_max_size);

extern Asset2
pack2_asset2_get_by_hash(u64 hash, Pack2 pack);

extern Asset2
pack2_asset2_get_by_name(char* name, Pack2 pack);

// NOTE(rhett): Returns a copy of asset with the unzipped asset size.
extern Asset2
pack2_asset2_load_to_buffer(Asset2 asset, u8* pack2_buffer, u8* asset2_buffer, u32 max_asset2_size);

extern void
pack2_export_assets_as_files(char* pack_path, char* output_folder, char* namelist_path);

extern void
pack2_consolidate_with_namelist(Pack2* pack, Pack2_Namelist namelist);

#ifdef FL_DEBUG
    extern void
    debug_allocation_check_for_unfreed_memory();
#endif // FL_DEBUG


#endif // FORGELIGHT_LIB_H
