#ifndef INTERNAL_H
#define INTERNAL_H


//----------------------------------------------------------------
// Endian IO
//----------------------------------------------------------------
//// Little endian
extern u32
endian_get_u32_le(u8 *data); // TODO(rhett): Is this name too hard to read?

extern u64
endian_get_u64_le(u8 *data);

//// Big endian
extern u16
endian_get_u16_be(u8 *data);

extern u32
endian_get_u32_be(u8 *data);


#endif // INTERNAL_H
