#ifndef FL_PACK2_H
#define FL_PACK2_H

//----------------------------------------------------------------
// Structures
//----------------------------------------------------------------
typedef struct Asset2_Data Asset2_Data;
struct Asset2_Data
    {
    uint64 data_offset;
    uint64 raw_data_length;
    uint32 zip_flag;
    };

typedef struct Pack2 Pack2;
struct Pack2
    {
    String8      pack_path;
    uint32       asset_count;
    uint64      *name_hashes;
    uint64       pack_length;
    Asset2_Data *asset_data;
    };


//----------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------
//// Helper functions
fl_external uint64
fl_crc64(char *to_hash);

//// Main functions
fl_external void
fl_load_pack2(Pack2 *ptr_pack, char *pack_path);

fl_external Asset2_Data
fl_get_asset_by_hash(uint64 hash, Pack2 pack);

fl_external Asset2_Data
fl_get_asset_by_name(char *name, Pack2 pack);


#endif // FL_PACK2_H