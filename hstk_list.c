#include "hstk_list.h"
#include "hstk.h"


struct TKList_ {
    unsigned int capacity;
    unsigned int used;

    item_ty *items;
};


static int resize(TKList li, int newsize)
{

    int new_cap;
    item_ty *new_items;
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
    new_items = Realloc(li->items, new_cap * sizeof(item_ty));
    li->items = new_items;
    li->capacity = new_cap;
    return 0;
}

int TKList_append(TKList li, item_ty item)
{
    if (li->capacity == li->used &&
            resize(li, li->capacity + 1) == -1)
        return -1;
    li->items[li->used++] = item;
    return 0;
}

int TKList_insert(TKList li, item_ty item, int pos)
{
    int idx;
    if (pos < 0 || pos > li->used)
        hstk_error_msg("List: Insert position out of range");

    if (pos == li->used)
        return TKList_append(li, item);

    if (li->capacity == li->used &&
            resize(li, li->capacity + 1) == -1)
        return -1;

    for (idx = li->used; idx > pos; idx--)
        li->items[idx] = li->items[idx - 1];
    li->items[pos] = item;
    li->used++;

    return 0;
}

item_ty TKList_pop(TKList li)
{
    item_ty item;

    item = li->items[li->used];
    li->items[li->used--] = NULL;

    return item;
}

int TKList_remove(TKList li, item_ty item)
{
    int idx;

    for (idx = 0; idx < li->used; idx++)
        if (item == li->items[idx])
            break;

    if (idx == li->used)
        hstk_error_msg("List: Remove not have this item");
    if (idx == li->used - 1) {
        TKList_pop(li);
        return 0;
    }

    for (; idx < li->used; idx++)
        li->items[idx] = li->items[idx + 1];

    li->used -= 1;
    return 0;
}

item_ty TKList_get_item(TKList li, int pos)
{
    if (pos < 0 || pos > li->used)
        hstk_error_msg("List: GetItem position out of range");

    return li->items[pos];
}

int TKList_len(TKList li)
{
    return li->used;
}

int TKList_clear(TKList li)
{
    Free(li->items);
    li->items = NULL;
    li->used = 0;
    li->capacity = 0;

    return 0;
}

TKList TKList_new()
{
    TKList list = Malloc(sizeof(struct TKList_));
    list->capacity = 0;
    list->used = 0;
    list->items = NULL;
    return list;
}

void TKList_destory(TKList list)
{
    if (list->items != NULL)
        Free(list->items);
    Free(list);
}

/* Oriented Object */
static int lo_append(TKListObj lo, item_ty item)
{
    TKList list = lo->attr;
    return TKList_append(list, item);
}

static int lo_insert(TKListObj lo, item_ty item, int pos)
{
    TKList list = lo->attr;
    return TKList_insert(list, item, pos);
}

static item_ty lo_pop(TKListObj lo)
{
    TKList list = lo->attr;
    return TKList_pop(list);
}

static int lo_remove(TKListObj lo, item_ty item)
{
    TKList list = lo->attr;
    return TKList_remove(list, item);
}

static item_ty lo_get_item(TKListObj lo, int pos)
{
    TKList list = lo->attr;
    return TKList_get_item(list, pos);
}

static int lo_len(TKListObj lo)
{
    TKList list = lo->attr;
    return TKList_len(list);
}

static int lo_clear(TKListObj lo)
{
    TKList list = lo->attr;
    return TKList_clear(list);
}

TKListObj TKListObj_new()
{
    TKListObj list_obj = Malloc(sizeof(struct TKListObj_));
    TKList list = TKList_new();

    list_obj->append = &lo_append;
    list_obj->insert = &lo_insert;
    list_obj->pop = &lo_pop;
    list_obj->remove = &lo_remove;
    list_obj->get_item = &lo_get_item;
    list_obj->len = &lo_len;
    list_obj->clear = &lo_clear;
    list_obj->attr = list;
    return list_obj;
}

void TKListObj_destory(TKListObj lo)
{
    TKList list = lo->attr;
    TKList_destory(list);
    Free(lo);
}
