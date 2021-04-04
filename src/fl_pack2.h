#ifndef FL_PACK2_H
#define FL_PACK2_H


//----------------------------------------------------------------
// Defines
//----------------------------------------------------------------
#define FL_ASSET_NOT_FOUND -1


//----------------------------------------------------------------
// Structures
//----------------------------------------------------------------
typedef struct Asset2_Data Asset2_Data;
struct Asset2_Data
    {
    ui64 data_offset;
    ui64 raw_data_length;
    ui32 zip_flag;
    };

typedef struct Pack2 Pack2;
struct Pack2
    {
    String8      pack_path;
    ui32         asset_count;
    ui64        *name_hashes;
    ui64         pack_length;
    Asset2_Data *asset_data;
    };


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
//// External function prototypes
fl_external ui64
crc64(ui64 crc, ui8 const *s, ui64 l);


//// Helper functions
fl_external ui64
fl_crc64(char *to_hash);

fl_external ui32
fl_oaat(char *to_hash); // TODO(rhett): 

//// Main functions
fl_external void
fl_load_pack2(Pack2 *ptr_pack, char *pack_path);

fl_external i32
fl_get_asset_index_by_hash(ui64 hash, Pack2 pack);

fl_external i32
fl_get_asset_index_by_name(char *name, Pack2 pack);

fl_external b32
fl_unpack_asset_from_pack2(ui32 asset_index, Pack2 pack);


#endif // FL_PACK2_H