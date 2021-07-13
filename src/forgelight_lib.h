#ifndef FORGELIGHT_LIB_H
#define FORGELIGHT_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FL_WIN32
#include "forgelight_lib/base.h"
#include "forgelight_lib/os.h"


#define malloc(sz)           os_memory_alloc(sz)
#define free(ptr)            os_memory_free(ptr)
#define memcpy(src,dst,sz)   memcmp(dst,src,sz)
#define memcmp(ptr1,ptr2,sz) memcmp(ptr1,ptr2,sz)
#define memset(ptr,val,sz)   memset(ptr,val,sz)


//----------------------------------------------------------------
// Strings
//----------------------------------------------------------------
typedef struct String8 String8;
struct String8
    {
    u8  *content;
    u32  length;
    };

extern u32
strings_cstring_length(char *cstring);

extern String8
strings_cstring_to_string8(char *cstring);


//----------------------------------------------------------------
// Pack2
//----------------------------------------------------------------
#define FL_PACK2_BUFFER_SIZE  MB(500)
#define FL_ASSET2_BUFFER_SIZE MB(100)

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
    String8  pack_path;
    u32      asset_count;
    u64      pack_length;
    Asset2  *asset2s;
    };

//// Helper functions
extern u64
pack2_crc64_uppercase(char *to_hash);

//// Main functions
extern Pack2
pack2_load_from_file(char *pack_path, u8 *pack2_buffer, u32 pack2_max_size);

extern Asset2
pack2_asset2_get_by_hash(u64 hash, Pack2 pack);

extern Asset2
pack2_asset2_get_by_name(char *name, Pack2 pack);

// NOTE(rhett): Returns a copy of asset with the unzipped asset size.
extern Asset2
pack2_asset2_load_to_buffer(Asset2 asset, u8 *pack2_buffer, u8 *asset2_buffer, u32 max_asset2_size);

extern void
pack2_export_assets_as_files(char *pack_path, char *output_folder);


#endif // FORGELIGHT_LIB_H
