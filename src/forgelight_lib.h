#ifndef FORGELIGHT_LIB_H
#define FORGELIGHT_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
extern u32
strings_cstring_length(char* cstring);

extern String8
strings_cstring_to_string8(char* cstring);


//----------------------------------------------------------------
// Pack2
//----------------------------------------------------------------
#define FL_PACK2_BUFFER_SIZE        MB(500)
#define FL_ASSET2_BUFFER_SIZE       MB(100)
#define FL_NAMELIST_BUFFER_SIZE     MB(50)
#define FL_NAMELIST_MAX_NAME_LENGTH 256

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
    String8 pack_path;
    u32     asset_count;
    u64     pack_length;
    Asset2* asset2s;
    };

//// Helper functions
extern u64
pack2_names_calculate_hash(char* to_hash);

extern Pack2_Namelist
pack2_names_generate_namelist_from_string_list(String8* string_list, uint string_count);

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
pack2_export_assets_as_files(char* pack_path, char* output_folder);

#ifdef FL_DEBUG
    extern void
    debug_allocation_check_for_unfreed_memory();
#endif // FL_DEBUG


#endif // FORGELIGHT_LIB_H
