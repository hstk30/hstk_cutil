/*
 * hashset
 * rehash to handle hash collision
 */

#ifndef HSTK_HASHSET_H_
#define HSTK_HASHSET_H_ 

#include "hstk.h"


typedef void *ele_ty;

struct HSTKHashset {
    val_ty  (* add)         (struct HSTKHashset *, key_ty);
    int     (* remove)      (struct HSTKHashset *, key_ty, val_ty);
    int     (* union_set)      (struct HSTKHashset *, key_ty, val_ty);
    int     (* diff_set)      (struct HSTKHashset *, key_ty, val_ty);
    int     (* contains)    (struct HSTKHashset *, key_ty);
    int     (* len)         (struct HSTKHashset *);
    int     (* clear)       (struct HSTKHashset *);
    void    (* destory)     (struct HSTKHashset *);
    void    *attr;
};

struct HSTKHashset *HSTK_hashmap_init();

#endif /* ifndef HSTK_HASHSET_H_ */
