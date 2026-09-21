/*
 * A fixed size kiln table.
 *
 * It holds up to KILN_SLOTS kilns, each with a key, a short name, and
 * a firings. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define KILN_SLOTS 96
#define KILN_NAME_SIZE 24

struct kiln {
    int key;
    char name[KILN_NAME_SIZE];
    int firings;
};

static const struct kiln kiln_seed[] = {
    { 101, "kiln-0001",  10 },
    { 102, "kiln-0002",  17 },
    { 103, "kiln-0003",  24 },
    { 104, "kiln-0004",  31 },
    { 105, "kiln-0005",  38 },
    { 106, "kiln-0006",  45 },
    { 107, "kiln-0007",  52 },
    { 108, "kiln-0008",  59 },
    { 109, "kiln-0009",  66 },
    { 110, "kiln-0010",  73 },
    { 111, "kiln-0011",  80 },
    { 112, "kiln-0012",  87 },
    { 113, "kiln-0013",  94 },
    { 114, "kiln-0014",   4 },
    { 115, "kiln-0015",  11 },
    { 116, "kiln-0016",  18 },
    { 117, "kiln-0017",  25 },
    { 118, "kiln-0018",  32 },
    { 119, "kiln-0019",  39 },
    { 120, "kiln-0020",  46 },
    { 121, "kiln-0021",  53 },
    { 122, "kiln-0022",  60 },
    { 123, "kiln-0023",  67 },
    { 124, "kiln-0024",  74 },
    { 125, "kiln-0025",  81 },
    { 126, "kiln-0026",  88 },
    { 127, "kiln-0027",  95 },
    { 128, "kiln-0028",   5 },
    { 129, "kiln-0029",  12 },
    { 130, "kiln-0030",  19 },
    { 131, "kiln-0031",  26 },
    { 132, "kiln-0032",  33 },
    { 133, "kiln-0033",  40 },
    { 134, "kiln-0034",  47 },
    { 135, "kiln-0035",  54 },
    { 136, "kiln-0036",  61 },
    { 137, "kiln-0037",  68 },
    { 138, "kiln-0038",  75 },
    { 139, "kiln-0039",  82 },
    { 140, "kiln-0040",  89 },
    { 141, "kiln-0041",  96 },
    { 142, "kiln-0042",   6 },
    { 143, "kiln-0043",  13 },
    { 144, "kiln-0044",  20 },
    { 145, "kiln-0045",  27 },
    { 146, "kiln-0046",  34 },
    { 147, "kiln-0047",  41 },
    { 148, "kiln-0048",  48 },
};

static struct kiln kiln_table[KILN_SLOTS];
static int kiln_used;

void kiln_reset(void)
{
    memset(kiln_table, 0, sizeof(kiln_table));
    kiln_used = 0;
}

int kiln_add(int key, const char *name, int firings)
{
    struct kiln *slot;

    if (kiln_used >= KILN_SLOTS) {
        return -1;
    }

    slot = &kiln_table[kiln_used];
    slot->key = key;
    slot->firings = firings;
    strncpy(slot->name, name, KILN_NAME_SIZE - 1);
    slot->name[KILN_NAME_SIZE - 1] = '\0';
    kiln_used++;

    return kiln_used - 1;
}

struct kiln *kiln_find(int key)
{
    int i;

    for (i = 0; i < kiln_used; i++) {
        if (kiln_table[i].key == key) {
            return &kiln_table[i];
        }
    }

    return NULL;
}

int kiln_remove(int key)
{
    struct kiln *found;

    found = kiln_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = kiln_table[kiln_used - 1];
    kiln_used--;

    return 0;
}

int kiln_total_firings(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < kiln_used; i++) {
        total += kiln_table[i].firings;
    }

    return total;
}

void kiln_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(kiln_seed) / sizeof(kiln_seed[0]); i++) {
        kiln_add(kiln_seed[i].key, kiln_seed[i].name,
                    kiln_seed[i].firings);
    }
}

void kiln_report(FILE *out)
{
    int i;

    fprintf(out, "%d kilns\n", kiln_used);
    for (i = 0; i < kiln_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", kiln_table[i].key,
                kiln_table[i].name, kiln_table[i].firings);
    }
    fprintf(out, "  total firings %d\n", kiln_total_firings());
}

int main(void)
{
    kiln_reset();
    kiln_load_seed();
    kiln_remove(103);
    kiln_report(stdout);

    return 0;
}
