#include <assert.h>

#include "hstk_list.h"


struct unit {
    int a;
    void *p;
};

void print_unit_list(struct HSTKList *lo)
{
    int len = lo->len(lo);
    int idx;
    struct unit *ref_item;

    printf("[");
    for (idx = 0; idx < len; idx++) {
        ref_item = lo->get_item(lo, idx);
        printf("unit(a: %d, p: %p) ,", ref_item->a, ref_item->p);
    }
    printf("]\n");
}

int main(int argc, char *argv[])
{

    struct unit item1 = (struct unit) {.a = 0, .p = NULL};
    struct unit item2 = (struct unit) {.a = 1, .p = &item1};
    struct unit item3 = (struct unit) {.a = 2, .p = &item2};
    struct unit item4 = (struct unit) {.a = 3, .p = &item3};
    struct unit item5 = (struct unit) {.a = 4, .p = &item4};
    struct unit item6 = (struct unit) {.a = 5, .p = &item5};

    struct HSTKList *list = HSTK_list_init();
    printf("append item: %d\n", item1.a);
    list->append(list, &item1);
    print_unit_list(list);

    printf("append item: %d\n", item2.a);
    list->append(list, &item2);
    print_unit_list(list);

    printf("append item: %d\n", item3.a);
    list->append(list, &item3);
    print_unit_list(list);

    printf("append item: %d\n", item4.a);
    list->append(list, &item4);
    print_unit_list(list);

    printf("append item: %d\n", item5.a);
    list->append(list, &item5);
    print_unit_list(list);

    printf("append item: %d\n", item6.a);
    list->append(list, &item6);
    print_unit_list(list);

    struct unit item7 = (struct unit) {.a = 7, .p = &item5};
    printf("insert item: %d at pos=6\n", item7.a);
    list->insert(list, &item7, 6);
    print_unit_list(list);

    struct unit item8 = (struct unit) {.a = 8, .p = &item5};
    printf("insert item: %d at pos=3\n", item8.a);
    list->insert(list, &item8, 3);
    print_unit_list(list);

    printf("pop \n");
    list->pop(list);
    print_unit_list(list);

    printf("pop \n");
    list->pop(list);
    print_unit_list(list);

    printf("remove item: %d\n", item2.a);
    list->remove(list, &item2);
    print_unit_list(list);

    struct unit *ref_item = list->get_item(list, 2);
    printf("get item pos=2, unit(%d, %p)\n", ref_item->a, ref_item->p);

    list->clear(list);
    print_unit_list(list);

    list->destory(list);
}

