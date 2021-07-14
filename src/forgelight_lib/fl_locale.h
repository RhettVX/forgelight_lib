//================================================================
#ifndef FL_LOCALE_H
#define FL_LOCALE_H


fl_internal u32
fl_lookup2(char* to_hash);


#endif // FL_LOCALE_H
//================================================================

//================================================================
#ifdef FL_LOCALE_IMPL


fl_internal u32
fl_lookup2(char* to_hash)
{
    return hash(to_hash, cstring_length(to_hash), 0); 
}


#endif // FL_LOCALE_IMPL
//================================================================
