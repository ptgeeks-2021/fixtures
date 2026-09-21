/*
 * A fixed size centrifuge table.
 *
 * It holds up to CENTRIFUGE_SLOTS centrifuges, each with a key, a short name, and
 * a cycles. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define CENTRIFUGE_SLOTS 64
#define CENTRIFUGE_NAME_SIZE 24

struct centrifuge {
    int key;
    char name[CENTRIFUGE_NAME_SIZE];
    int cycles;
};

static const struct centrifuge centrifuge_seed[] = {
    { 101, "centrifuge-0001",  10 },
    { 102, "centrifuge-0002",  17 },
    { 103, "centrifuge-0003",  24 },
    { 104, "centrifuge-0004",  31 },
    { 105, "centrifuge-0005",  38 },
    { 106, "centrifuge-0006",  45 },
    { 107, "centrifuge-0007",  52 },
    { 108, "centrifuge-0008",  59 },
    { 109, "centrifuge-0009",  66 },
    { 110, "centrifuge-0010",  73 },
    { 111, "centrifuge-0011",  80 },
    { 112, "centrifuge-0012",  87 },
    { 113, "centrifuge-0013",  94 },
    { 114, "centrifuge-0014",   4 },
    { 115, "centrifuge-0015",  11 },
    { 116, "centrifuge-0016",  18 },
    { 117, "centrifuge-0017",  25 },
    { 118, "centrifuge-0018",  32 },
    { 119, "centrifuge-0019",  39 },
    { 120, "centrifuge-0020",  46 },
    { 121, "centrifuge-0021",  53 },
    { 122, "centrifuge-0022",  60 },
    { 123, "centrifuge-0023",  67 },
    { 124, "centrifuge-0024",  74 },
    { 125, "centrifuge-0025",  81 },
    { 126, "centrifuge-0026",  88 },
    { 127, "centrifuge-0027",  95 },
    { 128, "centrifuge-0028",   5 },
    { 129, "centrifuge-0029",  12 },
    { 130, "centrifuge-0030",  19 },
    { 131, "centrifuge-0031",  26 },
    { 132, "centrifuge-0032",  33 },
    { 133, "centrifuge-0033",  40 },
    { 134, "centrifuge-0034",  47 },
    { 135, "centrifuge-0035",  54 },
    { 136, "centrifuge-0036",  61 },
    { 137, "centrifuge-0037",  68 },
    { 138, "centrifuge-0038",  75 },
    { 139, "centrifuge-0039",  82 },
    { 140, "centrifuge-0040",  89 },
    { 141, "centrifuge-0041",  96 },
    { 142, "centrifuge-0042",   6 },
    { 143, "centrifuge-0043",  13 },
    { 144, "centrifuge-0044",  20 },
    { 145, "centrifuge-0045",  27 },
    { 146, "centrifuge-0046",  34 },
    { 147, "centrifuge-0047",  41 },
    { 148, "centrifuge-0048",  48 },
};

static struct centrifuge centrifuge_table[CENTRIFUGE_SLOTS];
static int centrifuge_used;

void centrifuge_reset(void)
{
    memset(centrifuge_table, 0, sizeof(centrifuge_table));
    centrifuge_used = 0;
}

int centrifuge_add(int key, const char *name, int cycles)
{
    struct centrifuge *slot;

    if (centrifuge_used >= CENTRIFUGE_SLOTS) {
        return -1;
    }

    slot = &centrifuge_table[centrifuge_used];
    slot->key = key;
    slot->cycles = cycles;
    strncpy(slot->name, name, CENTRIFUGE_NAME_SIZE - 1);
    slot->name[CENTRIFUGE_NAME_SIZE - 1] = '\0';
    centrifuge_used++;

    return centrifuge_used - 1;
}

struct centrifuge *centrifuge_find(int key)
{
    int i;

    for (i = 0; i < centrifuge_used; i++) {
        if (centrifuge_table[i].key == key) {
            return &centrifuge_table[i];
        }
    }

    return NULL;
}

int centrifuge_remove(int key)
{
    struct centrifuge *found;

    found = centrifuge_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = centrifuge_table[centrifuge_used - 1];
    centrifuge_used--;

    return 0;
}

int centrifuge_total_cycles(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < centrifuge_used; i++) {
        total += centrifuge_table[i].cycles;
    }

    return total;
}

void centrifuge_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(centrifuge_seed) / sizeof(centrifuge_seed[0]); i++) {
        centrifuge_add(centrifuge_seed[i].key, centrifuge_seed[i].name,
                    centrifuge_seed[i].cycles);
    }
}

void centrifuge_report(FILE *out)
{
    int i;

    fprintf(out, "%d centrifuges\n", centrifuge_used);
    for (i = 0; i < centrifuge_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", centrifuge_table[i].key,
                centrifuge_table[i].name, centrifuge_table[i].cycles);
    }
    fprintf(out, "  total cycles %d\n", centrifuge_total_cycles());
}

int main(void)
{
    centrifuge_reset();
    centrifuge_load_seed();
    centrifuge_remove(103);
    centrifuge_report(stdout);

    return 0;
}
