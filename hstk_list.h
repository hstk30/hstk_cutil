/*
 * Dynamic list, but will not shrink
 * all item is **point** 
 */

#ifndef HSTK_LIST_H_
#define HSTK_LIST_H_ 

#include "hstk.h" 


typedef void *item_ty;

/* traditional  */
typedef struct TKList_ *TKList;

TKList TKList_new();

int TKList_append(TKList, item_ty);
int TKList_insert(TKList, item_ty, int);
item_ty TKList_pop(TKList);
int TKList_remove(TKList, item_ty);
item_ty TKList_get_item(TKList, int);
int TKList_len(TKList);
int TKList_clear(TKList);

void TKList_destory(TKList);

/* Oriented Object */
typedef struct TKListObj_ *TKListObj;

struct TKListObj_ {
    int     (* append)     (TKListObj, item_ty);
    int     (* insert)     (TKListObj, item_ty, int);
    item_ty (* pop)        (TKListObj);
    int     (* remove)     (TKListObj, item_ty);
    item_ty (* get_item)   (TKListObj, int);
    int     (* len)        (TKListObj);
    int     (* clear)      (TKListObj);
    TKList  attr;  
};

TKListObj TKListObj_new();
void TKListObj_destory(TKListObj);

#endif /* ifndef HSTK_LIST_H_ */

