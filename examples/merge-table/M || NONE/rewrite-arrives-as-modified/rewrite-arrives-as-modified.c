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

static const struct warehouse warehouse_seed[] = {
    { 101, "warehouse-0001",  10 },
    { 102, "warehouse-0002",  17 },
    { 103, "warehouse-0003",  24 },
    { 104, "warehouse-0004",  31 },
    { 105, "warehouse-0005",  38 },
    { 106, "warehouse-0006",  45 },
    { 107, "warehouse-0007",  52 },
    { 108, "warehouse-0008",  59 },
    { 109, "warehouse-0009",  66 },
    { 110, "warehouse-0010",  73 },
    { 111, "warehouse-0011",  80 },
    { 112, "warehouse-0012",  87 },
    { 113, "warehouse-0013",  94 },
    { 114, "warehouse-0014",   4 },
    { 115, "warehouse-0015",  11 },
    { 116, "warehouse-0016",  18 },
    { 117, "warehouse-0017",  25 },
    { 118, "warehouse-0018",  32 },
    { 119, "warehouse-0019",  39 },
    { 120, "warehouse-0020",  46 },
    { 121, "warehouse-0021",  53 },
    { 122, "warehouse-0022",  60 },
    { 123, "warehouse-0023",  67 },
    { 124, "warehouse-0024",  74 },
    { 125, "warehouse-0025",  81 },
    { 126, "warehouse-0026",  88 },
    { 127, "warehouse-0027",  95 },
    { 128, "warehouse-0028",   5 },
    { 129, "warehouse-0029",  12 },
    { 130, "warehouse-0030",  19 },
    { 131, "warehouse-0031",  26 },
    { 132, "warehouse-0032",  33 },
    { 133, "warehouse-0033",  40 },
    { 134, "warehouse-0034",  47 },
    { 135, "warehouse-0035",  54 },
    { 136, "warehouse-0036",  61 },
    { 137, "warehouse-0037",  68 },
    { 138, "warehouse-0038",  75 },
    { 139, "warehouse-0039",  82 },
    { 140, "warehouse-0040",  89 },
    { 141, "warehouse-0041",  96 },
    { 142, "warehouse-0042",   6 },
    { 143, "warehouse-0043",  13 },
    { 144, "warehouse-0044",  20 },
    { 145, "warehouse-0045",  27 },
    { 146, "warehouse-0046",  34 },
    { 147, "warehouse-0047",  41 },
    { 148, "warehouse-0048",  48 },
};

static struct warehouse warehouse_table[WAREHOUSE_SLOTS];
static int warehouse_used;

void warehouse_reset(void)
{
    memset(warehouse_table, 0, sizeof(warehouse_table));
    warehouse_used = 0;
}

int warehouse_add(int key, const char *name, int capacity)
{
    struct warehouse *slot;

    if (warehouse_used >= WAREHOUSE_SLOTS) {
        return -1;
    }

    slot = &warehouse_table[warehouse_used];
    slot->key = key;
    slot->capacity = capacity;
    strncpy(slot->name, name, WAREHOUSE_NAME_SIZE - 1);
    slot->name[WAREHOUSE_NAME_SIZE - 1] = '\0';
    warehouse_used++;

    return warehouse_used - 1;
}

struct warehouse *warehouse_find(int key)
{
    int i;

    for (i = 0; i < warehouse_used; i++) {
        if (warehouse_table[i].key == key) {
            return &warehouse_table[i];
        }
    }

    return NULL;
}

int warehouse_remove(int key)
{
    struct warehouse *found;

    found = warehouse_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = warehouse_table[warehouse_used - 1];
    warehouse_used--;

    return 0;
}

int warehouse_total_capacity(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < warehouse_used; i++) {
        total += warehouse_table[i].capacity;
    }

    return total;
}

void warehouse_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(warehouse_seed) / sizeof(warehouse_seed[0]); i++) {
        warehouse_add(warehouse_seed[i].key, warehouse_seed[i].name,
                    warehouse_seed[i].capacity);
    }
}

void warehouse_report(FILE *out)
{
    int i;

    fprintf(out, "%d warehouses\n", warehouse_used);
    for (i = 0; i < warehouse_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", warehouse_table[i].key,
                warehouse_table[i].name, warehouse_table[i].capacity);
    }
    fprintf(out, "  total capacity %d\n", warehouse_total_capacity());
}

int main(void)
{
    warehouse_reset();
    warehouse_load_seed();
    warehouse_remove(103);
    warehouse_report(stdout);

    return 0;
}
