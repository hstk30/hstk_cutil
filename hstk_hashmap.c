#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "hstk_hashmap.h"
#include "hstk.h"


#define HASHMAP_INIT_CAP 8
#define HASHMAP_RESIZE_FACTOR 2

#define PERTURB_SHIFT 5


typedef unsigned long hash_ty;

enum EntryStatus {
    EMPTY,
    ACTIVED,
    DIED,
};

struct MapEntry {
    enum EntryStatus status;
    key_ty key;
    val_ty val;
};

struct TKHashmap_ {
    size_t ksize;       /* key item size, used by hash */
    unsigned int capacity;
    unsigned int actived;
    unsigned int used;  /* actived + died */

    struct MapEntry *table;
};


static TKHashmap rehash(TKHashmap map);
static hash_ty hash_it(const unsigned char *, size_t);
static struct MapEntry *find_entry(TKHashmap, key_ty);
static TKHashmap TKHashmap_new1(size_t, size_t);

/* djb2 hash xor */
static hash_ty hash_it(const unsigned char *key, size_t ksize) {
	hash_ty hash = 5381;
    int idx;
    size_t n_block = ksize / sizeof(char);

    for (idx = 0; idx < n_block; idx++)
        /* hash * 33 ^ c */
        hash = ((hash << 5) + hash) ^ (int)key[idx];

	return hash;
}

static TKHashmap rehash(TKHashmap map)
{
    TKHashmap new_map;
    unsigned int old_cap = map->capacity;
    struct MapEntry *old_table = map->table;
    unsigned int new_cap = map->used * HASHMAP_RESIZE_FACTOR;
    int pos;

    new_map = TKHashmap_new1(map->ksize, new_cap);
    for (pos = 0; pos < old_cap; pos++)
        if (old_table[pos].status == ACTIVED)
            TKHashmap_set_item(map, old_table[pos].key, old_table[pos].val);

    TKHashmap_destory(map);
    return new_map;
}

/*
 * return: if key is in map, then return this entry, else a empty entry returned for set
 */
static struct MapEntry *find_entry(TKHashmap map, key_ty key)
{
    hash_ty hash = hash_it(key, map->ksize);
    unsigned int pos = (unsigned int)hash % map->capacity;
    struct MapEntry *entry = &map->table[pos];
    size_t perturb;

    perturb = hash;
    while (entry->status != EMPTY && entry->key != key) {
        pos = ((pos << 2) + pos + perturb + 1) % map->capacity;
        entry = &map->table[pos];
        perturb >>= PERTURB_SHIFT;
    }

    return entry;
}

ret_flag TKHashmap_set_item(TKHashmap map, key_ty key, val_ty val)
{
    if (map->used * 3 +  3 > map->capacity * 2 &&
            rehash(map) != 0)
        return 1;

    struct MapEntry *entry = find_entry(map, key);
    switch (entry->status) {
        case ACTIVED: {
            assert(entry->key == key);
            entry->val = val;
            return 0;
        }
        case DIED: {
            assert(entry->key == key);
            entry->status = ACTIVED;
            entry->val = val;
            map->actived++;
            return 0;
        }
        case EMPTY: {
            entry->status = ACTIVED;
            entry->key = key;
            entry->val = val;
            map->actived++;
            map->used++;
            return 0;
        }
        default: assert(0);
    }
}

val_ty TKHashmap_get_item(TKHashmap map, key_ty key)
{
    struct MapEntry *entry = find_entry(map, key);
    if (entry->status == EMPTY || entry->status == DIED)
        return NULL;

    return entry->val;
}

ret_flag TKHashmap_del_item(TKHashmap map, key_ty key)
{
    struct MapEntry *entry = find_entry(map, key);
    if (entry->status == EMPTY || entry->status == DIED)
        return 1;

    entry->status = DIED;
    map->actived--;
    return 0;
}

bool TKHashmap_contains(TKHashmap map, key_ty key)
{
    struct MapEntry *entry = find_entry(map, key);
    return entry->status == ACTIVED;
}

int TKHashmap_size(TKHashmap map)
{
    return map->used;
}

TKHashmap TKHashmap_new(size_t ksize)
{
    return TKHashmap_new1(ksize, HASHMAP_INIT_CAP);
}

static TKHashmap TKHashmap_new1(size_t ksize, size_t msize)
{
    TKHashmap map = Malloc(sizeof(struct TKHashmap_));
    struct MapEntry *table = Malloc(sizeof(struct MapEntry) * msize);
    map->capacity = msize;
    map->ksize = ksize;
    map->actived = 0;
    map->used = 0;
    map->table = table;

    int idx;
    for (idx = 0; idx < map->capacity; idx++)
        map->table[idx].status = EMPTY;

    return map;
}

void TKHashmap_destory(TKHashmap map)
{
    Free(map->table);
    Free(map);
}

/* Oriented Object */

static ret_flag mo_set_item(TKHashmapObj mo, key_ty key, val_ty val)
{
    TKHashmap map = mo->attr;
    return TKHashmap_set_item(map, key, val);
}

static val_ty mo_get_item(TKHashmapObj mo, key_ty key)
{
    TKHashmap map = mo->attr;
    return TKHashmap_get_item(map, key);
}

static ret_flag mo_del_item(TKHashmapObj mo, key_ty key)
{
    TKHashmap map = mo->attr;
    return TKHashmap_del_item(map, key);
}

static bool mo_contains(TKHashmapObj mo, key_ty key)
{
    TKHashmap map = mo->attr;
    return TKHashmap_contains(map, key);
}

static int mo_size(TKHashmapObj mo)
{
    TKHashmap map = mo->attr;
    return TKHashmap_size(map);
}

TKHashmapObj TKHashmapObj_new()
{
    TKHashmapObj mo = Malloc(sizeof(struct TKHashmapObj_));
    TKHashmap map = Malloc(sizeof(struct TKHashmap_));

    mo->set_item = &mo_set_item;
    mo->get_item = &mo_get_item;
    mo->del_item = &mo_del_item;
    mo->contains = &mo_contains;
    mo->size = &mo_size;

    mo->attr = map;
    return mo;
}

void TKHashmapObj_destory(TKHashmapObj mo)
{
    TKHashmap map = mo->attr;
    TKHashmap_destory(map);
    Free(mo);
}


