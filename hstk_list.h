/*
 * Dynamic list, but will not shrink
 * all item is **point** 
 */

#ifndef HSTK_LIST_H_
#define HSTK_LIST_H_ 

#include "hstk.h" 

typedef void *item_ty;

struct HSTKList {
    int     (* append)     (struct HSTKList *, item_ty);
    int     (* insert)     (struct HSTKList *, item_ty, int);
    item_ty (* pop)        (struct HSTKList *);
    int     (* remove)     (struct HSTKList *, item_ty);
    item_ty (* get_item)   (struct HSTKList *, int);
    int     (* len)        (struct HSTKList *);
    int     (* clear)      (struct HSTKList *);
    void    (* destory)    (struct HSTKList *);
    void    *attr;  /* should not access by user */
};

struct HSTKList *HSTK_list_init();

#endif /* ifndef HSTK_LIST_H_ */

