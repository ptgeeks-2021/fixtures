/*
 * A fixed size warehouse table.
 *
 * It holds up to WAREHOUSE_SLOTS warehouses, each with a key, a short name, and
 * a capacity. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define WAREHOUSE_SLOTS 64
#define WAREHOUSE_NAME_SIZE 24

struct warehouse {
    int key;
    char name[WAREHOUSE_NAME_SIZE];
    int capacity;
};

static const struct warehouse warehouse_defaults[] = {
    { 101, "warehouse_0001",  10 },
    { 102, "warehouse_0002",  17 },
    { 103, "warehouse_0003",  24 },
    { 104, "warehouse_0004",  31 },
    { 105, "warehouse_0005",  38 },
    { 106, "warehouse_0006",  45 },
    { 107, "warehouse_0007",  52 },
    { 108, "warehouse_0008",  59 },
    { 109, "warehouse_0009",  66 },
    { 110, "warehouse_0010",  73 },
    { 111, "warehouse_0011",  80 },
    { 112, "warehouse_0012",  87 },
    { 113, "warehouse_0013",  94 },
    { 114, "warehouse_0014",   4 },
    { 115, "warehouse_0015",  11 },
    { 116, "warehouse_0016",  18 },
    { 117, "warehouse_0017",  25 },
    { 118, "warehouse_0018",  32 },
    { 119, "warehouse_0019",  39 },
    { 120, "warehouse_0020",  46 },
    { 121, "warehouse_0021",  53 },
    { 122, "warehouse_0022",  60 },
    { 123, "warehouse_0023",  67 },
    { 124, "warehouse_0024",  74 },
    { 125, "warehouse_0025",  81 },
    { 126, "warehouse_0026",  88 },
    { 127, "warehouse_0027",  95 },
    { 128, "warehouse_0028",   5 },
    { 129, "warehouse_0029",  12 },
    { 130, "warehouse_0030",  19 },
    { 131, "warehouse_0031",  26 },
    { 132, "warehouse_0032",  33 },
    { 133, "warehouse_0033",  40 },
    { 134, "warehouse_0034",  47 },
    { 135, "warehouse_0035",  54 },
    { 136, "warehouse_0036",  61 },
    { 137, "warehouse_0037",  68 },
    { 138, "warehouse_0038",  75 },
    { 139, "warehouse_0039",  82 },
    { 140, "warehouse_0040",  89 },
    { 141, "warehouse_0041",  96 },
    { 142, "warehouse_0042",   6 },
    { 143, "warehouse_0043",  13 },
    { 144, "warehouse_0044",  20 },
    { 145, "warehouse_0045",  27 },
    { 146, "warehouse_0046",  34 },
    { 147, "warehouse_0047",  41 },
    { 148, "warehouse_0048",  48 },
};

static struct warehouse warehouse_store[WAREHOUSE_SLOTS];
static int warehouse_length;

void warehouse_reset(void)
{
    memset(warehouse_store, 0, sizeof(warehouse_store));
    warehouse_length = 0;
}

int warehouse_insert(int key, const char *name, int capacity)
{
    struct warehouse *entry;

    if (warehouse_length >= WAREHOUSE_SLOTS) {
        return -1;
    }

    entry = &warehouse_store[warehouse_length];
    entry->key = key;
    entry->capacity = capacity;
    strncpy(entry->name, name, WAREHOUSE_NAME_SIZE - 1);
    entry->name[WAREHOUSE_NAME_SIZE - 1] = '\0';
    warehouse_length++;

    return warehouse_length - 1;
}

struct warehouse *warehouse_lookup(int key)
{
    int i;

    for (i = 0; i < warehouse_length; i++) {
        if (warehouse_store[i].key == key) {
            return &warehouse_store[i];
        }
    }

    return NULL;
}

int warehouse_erase(int key)
{
    struct warehouse *hit;

    hit = warehouse_lookup(key);
    if (hit == NULL) {
        return -1;
    }

    *hit = warehouse_store[warehouse_length - 1];
    warehouse_length--;

    return 0;
}

int warehouse_sum_capacity(void)
{
    int i;
    int sum;

    sum = 0;
    for (i = 0; i < warehouse_length; i++) {
        sum += warehouse_store[i].capacity;
    }

    return sum;
}

void warehouse_load_defaults(void)
{
    size_t i;

    for (i = 0; i < sizeof(warehouse_defaults) / sizeof(warehouse_defaults[0]); i++) {
        warehouse_insert(warehouse_defaults[i].key, warehouse_defaults[i].name,
                    warehouse_defaults[i].capacity);
    }
}

void warehouse_dump(FILE *out)
{
    int i;

    fprintf(out, "%d warehouses\n", warehouse_length);
    for (i = 0; i < warehouse_length; i++) {
        fprintf(out, "  %4d %-24s %6d\n", warehouse_store[i].key,
                warehouse_store[i].name, warehouse_store[i].capacity);
    }
    fprintf(out, "  sum capacity %d\n", warehouse_sum_capacity());
}

int main(void)
{
    warehouse_reset();
    warehouse_load_defaults();
    warehouse_erase(103);
    warehouse_dump(stdout);

    return 0;
}
