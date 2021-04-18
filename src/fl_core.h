#ifndef FL_CORE_H
#define FL_CORE_H


//----------------------------------------------------------------
// Defines
//----------------------------------------------------------------
#define fl_internal      static
#define fl_global        static
#define fl_local_persist static
#define fl_external      extern

#define fl_cast(type) (type)

#define FL_FALSE 0
#define FL_TRUE  1

#define fl_alloc(sz)          w32_memory_alloc(sz)
#define fl_free(p)            w32_memory_free(p)
#define fl_memcpy(src,dst,sz) memory_copy(src,dst,sz)


//----------------------------------------------------------------
// Type aliases
//----------------------------------------------------------------
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  ui8;  // NOTE(rhett): Using ui8 instead of u8 to avoid name conflicts with the utf string prefix
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef uint64_t ui64;

typedef i8       b8;
typedef i16      b16;
typedef i32      b32;
typedef i64      b64;

typedef float    f32;
typedef double   f64;


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
fl_internal void
memory_copy(ui8 *source, ui8 *destination, ui32 size);

//// Little endian
fl_internal ui32
get_ui32le(ui8 *data); // TODO(rhett): Is this name too hard to read?

fl_internal ui64
get_ui64le(ui8 *data);

//// Big endian
fl_internal ui16
get_ui16be(ui8 *data);

fl_internal ui32
get_ui32be(ui8 *data);


#endif // FL_CORE_H