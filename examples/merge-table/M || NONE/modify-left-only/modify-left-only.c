/*
 * A fixed size pallet table.
 *
 * It holds up to PALLET_SLOTS pallets, each with a key, a short name, and
 * a units. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define PALLET_SLOTS 96
#define PALLET_NAME_SIZE 24

struct pallet {
    int key;
    char name[PALLET_NAME_SIZE];
    int units;
};

static const struct pallet pallet_seed[] = {
    { 101, "pallet-0001",  10 },
    { 102, "pallet-0002",  17 },
    { 103, "pallet-0003",  24 },
    { 104, "pallet-0004",  31 },
    { 105, "pallet-0005",  38 },
    { 106, "pallet-0006",  45 },
    { 107, "pallet-0007",  52 },
    { 108, "pallet-0008",  59 },
    { 109, "pallet-0009",  66 },
    { 110, "pallet-0010",  73 },
    { 111, "pallet-0011",  80 },
    { 112, "pallet-0012",  87 },
    { 113, "pallet-0013",  94 },
    { 114, "pallet-0014",   4 },
    { 115, "pallet-0015",  11 },
    { 116, "pallet-0016",  18 },
    { 117, "pallet-0017",  25 },
    { 118, "pallet-0018",  32 },
    { 119, "pallet-0019",  39 },
    { 120, "pallet-0020",  46 },
    { 121, "pallet-0021",  53 },
    { 122, "pallet-0022",  60 },
    { 123, "pallet-0023",  67 },
    { 124, "pallet-0024",  74 },
    { 125, "pallet-0025",  81 },
    { 126, "pallet-0026",  88 },
    { 127, "pallet-0027",  95 },
    { 128, "pallet-0028",   5 },
    { 129, "pallet-0029",  12 },
    { 130, "pallet-0030",  19 },
    { 131, "pallet-0031",  26 },
    { 132, "pallet-0032",  33 },
    { 133, "pallet-0033",  40 },
    { 134, "pallet-0034",  47 },
    { 135, "pallet-0035",  54 },
    { 136, "pallet-0036",  61 },
    { 137, "pallet-0037",  68 },
    { 138, "pallet-0038",  75 },
    { 139, "pallet-0039",  82 },
    { 140, "pallet-0040",  89 },
    { 141, "pallet-0041",  96 },
    { 142, "pallet-0042",   6 },
    { 143, "pallet-0043",  13 },
    { 144, "pallet-0044",  20 },
    { 145, "pallet-0045",  27 },
    { 146, "pallet-0046",  34 },
    { 147, "pallet-0047",  41 },
    { 148, "pallet-0048",  48 },
};

static struct pallet pallet_table[PALLET_SLOTS];
static int pallet_used;

void pallet_reset(void)
{
    memset(pallet_table, 0, sizeof(pallet_table));
    pallet_used = 0;
}

int pallet_add(int key, const char *name, int units)
{
    struct pallet *slot;

    if (pallet_used >= PALLET_SLOTS) {
        return -1;
    }

    slot = &pallet_table[pallet_used];
    slot->key = key;
    slot->units = units;
    strncpy(slot->name, name, PALLET_NAME_SIZE - 1);
    slot->name[PALLET_NAME_SIZE - 1] = '\0';
    pallet_used++;

    return pallet_used - 1;
}

struct pallet *pallet_find(int key)
{
    int i;

    for (i = 0; i < pallet_used; i++) {
        if (pallet_table[i].key == key) {
            return &pallet_table[i];
        }
    }

    return NULL;
}

int pallet_remove(int key)
{
    struct pallet *found;

    found = pallet_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = pallet_table[pallet_used - 1];
    pallet_used--;

    return 0;
}

int pallet_total_units(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < pallet_used; i++) {
        total += pallet_table[i].units;
    }

    return total;
}

void pallet_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(pallet_seed) / sizeof(pallet_seed[0]); i++) {
        pallet_add(pallet_seed[i].key, pallet_seed[i].name,
                    pallet_seed[i].units);
    }
}

void pallet_report(FILE *out)
{
    int i;

    fprintf(out, "%d pallets\n", pallet_used);
    for (i = 0; i < pallet_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", pallet_table[i].key,
                pallet_table[i].name, pallet_table[i].units);
    }
    fprintf(out, "  total units %d\n", pallet_total_units());
}

int main(void)
{
    pallet_reset();
    pallet_load_seed();
    pallet_remove(103);
    pallet_report(stdout);

    return 0;
}
