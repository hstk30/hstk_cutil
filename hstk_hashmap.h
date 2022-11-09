/*
 * hashmap
 * rehash to handle hash collision
 */

#ifndef HSTK_HASHMAP_H_
#define HSTK_HASHMAP_H_ 

#include "hstk.h"


typedef void *key_ty;
typedef void *val_ty;

struct HSTKHashmap {
    val_ty  (* get_item)    (struct HSTKHashmap *, key_ty);
    int     (* set_item)    (struct HSTKHashmap *, key_ty, val_ty);
    int     (* del_item)    (struct HSTKHashmap *, key_ty);
    int     (* contains)    (struct HSTKHashmap *, key_ty);
    int     (* len)         (struct HSTKHashmap *);
    int     (* clear)       (struct HSTKHashmap *);
    void    (* destory)     (struct HSTKHashmap *);
    void    *attr;
};

struct HSTKHashmap *HSTK_hashmap_init();

#endif /* ifndef HSTK_HASHMAP_H_ */
