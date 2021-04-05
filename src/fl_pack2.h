#ifndef FL_PACK2_H
#define FL_PACK2_H


//----------------------------------------------------------------
// Defines
//----------------------------------------------------------------


//----------------------------------------------------------------
// Structures
//----------------------------------------------------------------
typedef struct Asset2 Asset2;
struct Asset2
    {
    ui64 name_hash;
    ui64 data_offset;
    ui64 raw_data_length;
    ui32 zip_flag;
    };

typedef struct Pack2 Pack2;
struct Pack2
    {
    String8  pack_path;
    ui32     asset_count;
    ui64     pack_length;
    Asset2  *asset2s;
    };


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
//// Helper functions
fl_external ui64
fl_crc64(char *to_hash);

fl_external ui32
fl_oaat(char *to_hash); // TODO(rhett): 

//// Main functions
fl_external void
fl_load_pack2(Pack2 *ptr_pack, char *pack_path);

fl_external Asset2 *
fl_get_asset_index_by_hash(ui64 hash, Pack2 pack);

fl_external Asset2 *
fl_get_asset_index_by_name(char *name, Pack2 pack);

fl_external b32
fl_unpack_asset2(Asset2 *asset);


#endif // FL_PACK2_H