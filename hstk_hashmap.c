#include "hstk_hashmap.h"
#include "hstk.h"
#include <stddef.h>

/* load factor is 2/3  */
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

struct Hashmap {
    size_t ksize;       /* key item size, used by hash */
    unsigned int capacity;
    unsigned int actived;
    unsigned int used;  /* actived + died */

    struct MapEntry *table;
};


static int rehash(struct Hashmap *map);
static int hashmap_insert(struct Hashmap *, key_ty, val_ty);


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

static int rehash(struct Hashmap *map)
{
    int pos;
    unsigned int new_cap = map->used * HASHMAP_RESIZE_FACTOR;
    struct MapEntry *old_table = map->table;
    struct MapEntry *new_table = Malloc(sizeof(struct MapEntry) * new_cap);

    for (pos = 0; pos < new_cap; pos++)
        new_table[pos].status = EMPTY;

    for (pos = 0; pos < map->capacity; pos++)
        if (old_table[pos].status == ACTIVED)
            hashmap_insert(map, old_table[pos].key, old_table[pos].val);

    map->capacity = new_cap;
    map->used = map->actived;
    map->table = new_table;
    Free(old_table);
    return 0;
}

static struct MapEntry *find_entry(struct Hashmap *map, key_ty key)
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

static int hashmap_insert(struct Hashmap *map, key_ty key, val_ty val)
{
    if (map->used * 3 +  3 > map->capacity * 2)
        rehash(map);
    struct MapEntry *entry = find_entry(map, key)
}


static int hashmap_size(struct HSTKHashmap *mo)
{
    struct Hashmap *map = mo->attr;
    return map->used;
}

static struct Hashmap *new_hashmap(size_t ksize)
{
    struct Hashmap *map = Malloc(sizeof(struct Hashmap));
    struct MapEntry *table = Malloc(sizeof(struct MapEntry) * HASHMAP_INIT_CAP);
    map->capacity = HASHMAP_INIT_CAP;
    map->ksize = ksize;
    map->actived = 0;
    map->used = 0;
    map->table = table;

    int idx;
    for (idx = 0; idx < map->capacity; idx++)
        map->table[idx].status = EMPTY;

    return map;
}

struct HSTKHashmap *HSTK_hashmap_new(size_t ksize)
{
    struct HSTKHashmap *map_obj = Malloc(sizeof(struct HSTKHashmap));
    struct Hashmap *map = new_hashmap(ksize);

    map_obj->attr = map;
    return map_obj;
}

void HSTK_hashmap_destory(struct HSTKHashmap *mo)
{
    struct Hashmap *map = mo->attr;
    Free(map->table);
    Free(map);
    Free(mo);
}

