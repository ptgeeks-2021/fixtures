/*
 * A fixed size vineyard table.
 *
 * It holds up to VINEYARD_SLOTS vineyards, each with a key, a short name, and
 * a barrels. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define VINEYARD_SLOTS 64
#define VINEYARD_NAME_SIZE 24

struct vineyard {
    int key;
    char name[VINEYARD_NAME_SIZE];
    int barrels;
};

static const struct vineyard vineyard_seed[] = {
    { 101, "vineyard-0001",  10 },
    { 102, "vineyard-0002",  17 },
    { 103, "vineyard-0003",  24 },
    { 104, "vineyard-0004",  31 },
    { 105, "vineyard-0005",  38 },
    { 106, "vineyard-0006",  45 },
    { 107, "vineyard-0007",  52 },
    { 108, "vineyard-0008",  59 },
    { 109, "vineyard-0009",  66 },
    { 110, "vineyard-0010",  73 },
    { 111, "vineyard-0011",  80 },
    { 112, "vineyard-0012",  87 },
    { 113, "vineyard-0013",  94 },
    { 114, "vineyard-0014",   4 },
    { 115, "vineyard-0015",  11 },
    { 116, "vineyard-0016",  18 },
    { 117, "vineyard-0017",  25 },
    { 118, "vineyard-0018",  32 },
    { 119, "vineyard-0019",  39 },
    { 120, "vineyard-0020",  46 },
    { 121, "vineyard-0021",  53 },
    { 122, "vineyard-0022",  60 },
    { 123, "vineyard-0023",  67 },
    { 124, "vineyard-0024",  74 },
    { 125, "vineyard-0025",  81 },
    { 126, "vineyard-0026",  88 },
    { 127, "vineyard-0027",  95 },
    { 128, "vineyard-0028",   5 },
    { 129, "vineyard-0029",  12 },
    { 130, "vineyard-0030",  19 },
    { 131, "vineyard-0031",  26 },
    { 132, "vineyard-0032",  33 },
    { 133, "vineyard-0033",  40 },
    { 134, "vineyard-0034",  47 },
    { 135, "vineyard-0035",  54 },
    { 136, "vineyard-0036",  61 },
    { 137, "vineyard-0037",  68 },
    { 138, "vineyard-0038",  75 },
    { 139, "vineyard-0039",  82 },
    { 140, "vineyard-0040",  89 },
    { 141, "vineyard-0041",  96 },
    { 142, "vineyard-0042",   6 },
    { 143, "vineyard-0043",  13 },
    { 144, "vineyard-0044",  20 },
    { 145, "vineyard-0045",  27 },
    { 146, "vineyard-0046",  34 },
    { 147, "vineyard-0047",  41 },
    { 148, "vineyard-0048",  48 },
};

static struct vineyard vineyard_table[VINEYARD_SLOTS];
static int vineyard_used;

void vineyard_reset(void)
{
    memset(vineyard_table, 0, sizeof(vineyard_table));
    vineyard_used = 0;
}

int vineyard_add(int key, const char *name, int barrels)
{
    struct vineyard *slot;

    if (vineyard_used >= VINEYARD_SLOTS) {
        return -1;
    }

    slot = &vineyard_table[vineyard_used];
    slot->key = key;
    slot->barrels = barrels;
    strncpy(slot->name, name, VINEYARD_NAME_SIZE - 1);
    slot->name[VINEYARD_NAME_SIZE - 1] = '\0';
    vineyard_used++;

    return vineyard_used - 1;
}

struct vineyard *vineyard_find(int key)
{
    int i;

    for (i = 0; i < vineyard_used; i++) {
        if (vineyard_table[i].key == key) {
            return &vineyard_table[i];
        }
    }

    return NULL;
}

int vineyard_remove(int key)
{
    struct vineyard *found;

    found = vineyard_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = vineyard_table[vineyard_used - 1];
    vineyard_used--;

    return 0;
}

int vineyard_total_barrels(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < vineyard_used; i++) {
        total += vineyard_table[i].barrels;
    }

    return total;
}

void vineyard_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(vineyard_seed) / sizeof(vineyard_seed[0]); i++) {
        vineyard_add(vineyard_seed[i].key, vineyard_seed[i].name,
                    vineyard_seed[i].barrels);
    }
}

void vineyard_report(FILE *out)
{
    int i;

    fprintf(out, "%d vineyards\n", vineyard_used);
    for (i = 0; i < vineyard_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", vineyard_table[i].key,
                vineyard_table[i].name, vineyard_table[i].barrels);
    }
    fprintf(out, "  total barrels %d\n", vineyard_total_barrels());
}

int main(void)
{
    vineyard_reset();
    vineyard_load_seed();
    vineyard_remove(103);
    vineyard_report(stdout);

    return 0;
}
