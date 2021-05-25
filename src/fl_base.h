//================================================================
#ifndef FL_BASE_H
#define FL_BASE_H


//----------------------------------------------------------------
// Defines
//----------------------------------------------------------------
#define fl_internal      static
#define fl_external      extern
#define fl_local_persist static
#define fl_global        static
#define fl_dll_external  fl_external __declspec(dllexport)

#define KB(n) (n << 10)
#define MB(n) (n << 20)
#define GB(n) ((fl_cast(u64)n) << 30)
#define TB(n) ((fl_cast(u64)n) << 40)

#define fl_printf             printf
#define fl_eval_print_u32(x)  fl_printf("%s = %u:%xh\n",#x,fl_cast(u32)x,fl_cast(u32)x)
#define fl_eval_print_i32(x)  fl_printf("%s = %d:%xh\n",#x,fl_cast(i32)x,fl_cast(i32)x)
#define fl_eval_print_u64(x)  fl_printf("%s = %llu:%llxh\n",#x,fl_cast(u64)x,fl_cast(u64)x)
#define fl_eval_print_cstr(x) fl_printf("%s = \"%s\"\n",#x,x)
#define fl_cast(type)         (type)

#define fl_alloc(sz)            fl_w32_memory_alloc(sz)
#define fl_free(ptr)            fl_w32_memory_free(ptr)
#define fl_memcpy(src,dst,sz)   fl_memory_copy(src,dst,sz)
#define fl_memcmp(ptr1,ptr2,sz) memcmp(ptr1,ptr2,sz)
#define fl_memset(ptr,val,sz)   memset(ptr,val,sz)


//----------------------------------------------------------------
// Type aliases
//----------------------------------------------------------------
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef intptr_t iptr;

typedef uint8_t   u8;  // NOTE(rhett): Using u8 instead of u8 to avoid name conflicts with the utf string prefix
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef uintptr_t uptr;

typedef i8  b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;

typedef float  f32;
typedef double f64;


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
fl_internal void
fl_memory_copy(u8 *source, u8 *destination, u32 size);

//// Little endian
fl_internal u32
fl_get_u32le(u8 *data); // TODO(rhett): Is this name too hard to read?

fl_internal u64
fl_get_u64le(u8 *data);

//// Big endian
fl_internal u16
fl_get_u16be(u8 *data);

fl_internal u32
fl_get_u32be(u8 *data);


#endif // FL_BASE_H
//================================================================

//================================================================
#ifdef FL_BASE_IMPL


//----------------------------------------------------------------
// Function definitions
//----------------------------------------------------------------
fl_internal void
fl_memory_copy(u8 *source, u8 *destination, u32 size)
    {
    while(size--) // NOTE(rhett): Must be postfix
        {
        *(destination + size) = *(source + size);
        }
    }

//// Little endian
fl_internal u32
fl_get_u32le(u8 *data)
    {
    u32 result = 0;
    for (u32 i = 4; i > 0; --i)
        {
        result <<= 8;
        result |= *(data + (i - 1));
        }

    return result;
    }

fl_internal u64
fl_get_u64le(u8 *data)
    {
    u64 result = 0;
    for (u64 i = 8; i > 0; --i)
        {
        result <<= 8;
        result |= *(data + (i - 1));
        }

    return result;
    }

//// Big endian
fl_internal u16
fl_get_u16be(u8 *data)
    {
    u16 result = 0;
    for (u32 i = 0; i < 2; ++i)
        {
        result <<= 8;
        result |= *(data + i);
        }

    return result;
    }

fl_internal u32
fl_get_u32be(u8 *data)
    {
    u32 result = 0;
    for (u32 i = 0; i < 4; ++i)
        {
        result <<= 8;
        result |= *(data + i);
        }

    return result;
    }


#endif // FL_BASE_IMPL
//================================================================
