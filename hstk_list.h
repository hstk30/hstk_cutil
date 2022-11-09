#ifndef HSTK_LIST_H_
#define HSTK_LIST_H_ 

#include "hstk.h" 


typedef void *item_ty;

struct HSTKList {
    void    (* insert)     (struct HSTKList *, item_ty, int);
    void    (* append)     (struct HSTKList *, item_ty);
    void    (* remove)     (struct HSTKList *);
    item_ty (* pop)        (struct HSTKList *);
    item_ty (* get_item)   (struct HSTKList *, int);
    void    (* set_item)   (struct HSTKList *, int, item_ty);
    int     (* len)        (struct HSTKList *);
    void    (* clear)      (struct HSTKList *);
    void    (* destory)    (struct HSTKList *);
    void    *attr;  /* should not access by user */
};

struct HSTKList *HSTK_list_init(size_t itemsize);

#endif /* ifndef HSTK_LIST_H_ */
