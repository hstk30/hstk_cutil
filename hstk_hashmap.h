/*
 * hashmap
 * rehash to handle hash collision
 * load factor is 2/3 
 */

#ifndef HSTK_HASHMAP_H_
#define HSTK_HASHMAP_H_ 

#include "hstk.h"


typedef void *key_ty;
typedef void *val_ty;

typedef struct TKHashmap_ *TKHashmap;

TKHashmap TKHashmap_new();
ret_flag TKHashmap_set_item(TKHashmap, key_ty, val_ty);
val_ty TKHashmap_get_item(TKHashmap, key_ty);
ret_flag TKHashmap_del_item(TKHashmap, key_ty);
bool TKHashmap_contains(TKHashmap, key_ty);
int TKHashmap_size(TKHashmap);
void TKHashmap_destory(TKHashmap);

/* Oriented Object */
typedef struct TKHashmapObj_ *TKHashmapObj;
struct TKHashmapObj_ {
    ret_flag    (* set_item)    (TKHashmapObj, key_ty, val_ty);
    val_ty      (* get_item)    (TKHashmapObj, key_ty);
    ret_flag    (* del_item)    (TKHashmapObj, key_ty);
    bool        (* contains)    (TKHashmapObj, key_ty);
    int         (* size)        (TKHashmapObj);
    TKHashmap   attr;
};

TKHashmapObj TKHashmapObj_new();
void TKHashmapObj_destory(TKHashmapObj);

#endif /* ifndef HSTK_HASHMAP_H_ */
