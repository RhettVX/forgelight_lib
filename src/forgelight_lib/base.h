#ifndef BASE_H
#define BASE_H


//----------------------------------------------------------------
// Defines
//----------------------------------------------------------------
#define internal      static
#define local_persist static
#define global        static
#define dll_external  extern __declspec(dllexport)

#define KB(n) (n << 10)
#define MB(n) (n << 20)
#define GB(n) ((u64)n << 30)
#define TB(n) ((u64)n << 40)


//----------------------------------------------------------------
// 
//----------------------------------------------------------------
// #include <stdio.h>
#define eval_print_u32(x)  printf("%s = %u:%xh\n",     #x, (u32)x, (u32)x)
#define eval_print_i32(x)  printf("%s = %d:%xh\n",     #x, (i32)x, (i32)x)
#define eval_print_u64(x)  printf("%s = %llu:%llxh\n", #x, (u64)x, (u64)x)
#define eval_print_cstr(x) printf("%s = \"%s\"\n",     #x,      x)
#define array_count(a)     sizeof(a) / sizeof(a[0])


//----------------------------------------------------------------
// Type aliases
//----------------------------------------------------------------
#include <stdint.h>

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


#endif // BASE_H
