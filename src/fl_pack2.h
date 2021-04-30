#ifndef FL_PACK2_H
#define FL_PACK2_H


//----------------------------------------------------------------
// Defines
//----------------------------------------------------------------
#define FL_PACK2_BUFFER_SIZE  Megabytes(500)
#define FL_ASSET2_BUFFER_SIZE Megabytes(100)


//----------------------------------------------------------------
// Structures
//----------------------------------------------------------------
typedef struct Asset2 Asset2;
struct Asset2
    {
    ui64 name_hash;
    ui64 data_offset;
    ui64 raw_data_length;
    ui32 unzipped_data_length;
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
fl_external Pack2
fl_load_pack2(char *pack_path, ui8 *pack2_buffer, ui32 pack2_max_size);

fl_external Asset2
fl_get_asset2_by_hash(ui64 hash, Pack2 pack);

fl_external Asset2
fl_get_asset2_by_name(char *name, Pack2 pack);

// NOTE(rhett): Returns a copy of asset with the unzipped asset size.
fl_external Asset2
fl_read_asset2(Asset2 asset, ui8 *pack2_buffer, ui8 *asset2_buffer, ui32 max_asset2_size);


#endif // FL_PACK2_H