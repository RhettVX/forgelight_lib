#ifndef FL_CORE_H
#define FL_CORE_H


//----------------------------------------------------------------
// Defines
//----------------------------------------------------------------
#define fl_internal      static
#define fl_global        static
#define fl_local_persist static
#define fl_external      extern

#define cast(type) (type)

#define FALSE 0
#define TRUE  1

#define fl_alloc(sz)          w32_memory_alloc(sz)
#define fl_free(p)            w32_memory_free(p)
#define fl_memcpy(src,dst,sz) memory_copy(src,dst,sz)


//----------------------------------------------------------------
// Type aliases
//----------------------------------------------------------------
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8     bool8;
typedef int16    bool16;
typedef int32    bool32;
typedef int64    bool64;

typedef float    float32;
typedef double   float64;


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
fl_internal void
memory_copy(uint8 *source, uint8 *destination, uint32 size);

fl_internal uint32
get_uint32le(uint8 *data);

fl_internal uint64
get_uint64le(uint8 *data);


#endif // FL_CORE_H