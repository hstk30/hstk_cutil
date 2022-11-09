#include "hstk_list.h"
#include "hstk.h"
#include <stdio.h>


struct List {
    unsigned int capacity;
    unsigned int used;

    item_ty *items;
};

static int list_resize(struct HSTKList *lo, int newsize)
{

    int new_cap;
    item_ty *new_items;
    struct List *list = lo->attr;
    /* Reference from Cpython
     * This over-allocates proportional to the list size, making room
	 * for additional growth.  The over-allocation is mild, but is
	 * enough to give linear-time amortized behavior over a long
	 * sequence of appends() in the presence of a poorly-performing
	 * system realloc().
	 * The growth pattern is:  0, 4, 8, 16, 25, 35, 46, 58, 72, 88, ...
	 */
	new_cap = (newsize >> 3) + (newsize < 9 ? 3 : 6) + newsize;
    /* fprintf(stdout, "List resize for %d to %d\n", list->capacity, new_cap); */
    new_items = Realloc(list->items, new_cap * sizeof(item_ty));
    list->items = new_items;
    list->capacity = new_cap;
    return 0;
}

int list_append(struct HSTKList *lo, item_ty item)
{
    struct List *list = lo->attr;
    if (list->capacity == list->used &&
            list_resize(lo, list->capacity + 1) == -1)
        return -1;
    list->items[list->used++] = item;
    return 0;
}


int list_insert(struct HSTKList *lo, item_ty item, int pos)
{
    struct List *list = lo->attr;
    int idx;
    if (pos < 0 || pos > list->used)
        hstk_error_msg("HSTKList: Insert position out of range");

    if (pos == list->used)
        return list_append(lo, item);

    if (list->capacity == list->used &&
            list_resize(lo, list->capacity + 1) == -1)
        return -1;

    for (idx = list->used; idx > pos; idx--)
        list->items[idx] = list->items[idx - 1];
    list->items[pos] = item;
    list->used++;

    return 0;
}

item_ty list_pop(struct HSTKList *lo)
{
    struct List *list = lo->attr;
    item_ty item;

    item = list->items[list->used];
    list->items[list->used--] = NULL;

    return item;
}

int list_remove(struct HSTKList *lo, item_ty item)
{
    struct List *list = lo->attr;
    int idx;

    for (idx = 0; idx < list->used; idx++)
        if (item == list->items[idx])
            break;

    if (idx == list->used)
        hstk_error_msg("HSTKList: Remove not have this item");
    if (idx == list->used - 1) {
        list_pop(lo);
        return 0;
    }

    for (; idx < list->used; idx++)
        list->items[idx] = list->items[idx + 1];

    list->used -= 1;
    return 0;
}

item_ty list_get_item(struct HSTKList *lo, int pos)
{
    struct List *list = lo->attr;
    if (pos < 0 || pos > list->used)
        hstk_error_msg("HSTKList: GetItem position out of range");

    return list->items[pos];
}

int list_len(struct HSTKList *lo)
{
    struct List *list = lo->attr;
    return list->used;
}

int list_clear(struct HSTKList *lo)
{
    struct List *list = lo->attr;

    Free(list->items);
    list->items = NULL;
    list->used = 0;
    list->capacity = 0;

    return 0;
}

void list_destory(struct HSTKList *lo)
{
    struct List *list = lo->attr;
    if (list->items != NULL)
        Free(list->items);
    Free(list);
    Free(lo);
}

struct HSTKList *HSTK_list_init()
{
    struct HSTKList *list_obj = Malloc(sizeof(struct HSTKList));
    struct List *list = Malloc(sizeof(struct List));
    list->capacity = 0;
    list->used = 0;
    list->items = NULL;

    list_obj->append = &list_append;
    list_obj->insert = &list_insert;
    list_obj->pop = &list_pop;
    list_obj->remove = &list_remove;
    list_obj->get_item = &list_get_item;
    list_obj->len = &list_len;
    list_obj->clear = &list_clear;
    list_obj->destory = &list_destory;
    list_obj->attr = list;
    return list_obj;
}
