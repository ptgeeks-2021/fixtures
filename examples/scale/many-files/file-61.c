/*
 * A fixed size microscope table.
 *
 * It holds up to MICROSCOPE_SLOTS microscopes, each with a key, a short name, and
 * a lenses. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define MICROSCOPE_SLOTS 64
#define MICROSCOPE_NAME_SIZE 24

struct microscope {
    int key;
    char name[MICROSCOPE_NAME_SIZE];
    int lenses;
};

static const struct microscope microscope_seed[] = {
    { 101, "microscope-0001",  10 },
    { 102, "microscope-0002",  17 },
    { 103, "microscope-0003",  24 },
    { 104, "microscope-0004",  31 },
    { 105, "microscope-0005",  38 },
    { 106, "microscope-0006",  45 },
    { 107, "microscope-0007",  52 },
    { 108, "microscope-0008",  59 },
    { 109, "microscope-0009",  66 },
    { 110, "microscope-0010",  73 },
    { 111, "microscope-0011",  80 },
    { 112, "microscope-0012",  87 },
    { 113, "microscope-0013",  94 },
    { 114, "microscope-0014",   4 },
    { 115, "microscope-0015",  11 },
    { 116, "microscope-0016",  18 },
    { 117, "microscope-0017",  25 },
    { 118, "microscope-0018",  32 },
    { 119, "microscope-0019",  39 },
    { 120, "microscope-0020",  46 },
    { 121, "microscope-0021",  53 },
    { 122, "microscope-0022",  60 },
    { 123, "microscope-0023",  67 },
    { 124, "microscope-0024",  74 },
    { 125, "microscope-0025",  81 },
    { 126, "microscope-0026",  88 },
    { 127, "microscope-0027",  95 },
    { 128, "microscope-0028",   5 },
    { 129, "microscope-0029",  12 },
    { 130, "microscope-0030",  19 },
    { 131, "microscope-0031",  26 },
    { 132, "microscope-0032",  33 },
    { 133, "microscope-0033",  40 },
    { 134, "microscope-0034",  47 },
    { 135, "microscope-0035",  54 },
    { 136, "microscope-0036",  61 },
    { 137, "microscope-0037",  68 },
    { 138, "microscope-0038",  75 },
    { 139, "microscope-0039",  82 },
    { 140, "microscope-0040",  89 },
    { 141, "microscope-0041",  96 },
    { 142, "microscope-0042",   6 },
    { 143, "microscope-0043",  13 },
    { 144, "microscope-0044",  20 },
    { 145, "microscope-0045",  27 },
    { 146, "microscope-0046",  34 },
    { 147, "microscope-0047",  41 },
    { 148, "microscope-0048",  48 },
};

static struct microscope microscope_table[MICROSCOPE_SLOTS];
static int microscope_used;

void microscope_reset(void)
{
    memset(microscope_table, 0, sizeof(microscope_table));
    microscope_used = 0;
}

int microscope_add(int key, const char *name, int lenses)
{
    struct microscope *slot;

    if (microscope_used >= MICROSCOPE_SLOTS) {
        return -1;
    }

    slot = &microscope_table[microscope_used];
    slot->key = key;
    slot->lenses = lenses;
    strncpy(slot->name, name, MICROSCOPE_NAME_SIZE - 1);
    slot->name[MICROSCOPE_NAME_SIZE - 1] = '\0';
    microscope_used++;

    return microscope_used - 1;
}

struct microscope *microscope_find(int key)
{
    int i;

    for (i = 0; i < microscope_used; i++) {
        if (microscope_table[i].key == key) {
            return &microscope_table[i];
        }
    }

    return NULL;
}

int microscope_remove(int key)
{
    struct microscope *found;

    found = microscope_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = microscope_table[microscope_used - 1];
    microscope_used--;

    return 0;
}

int microscope_total_lenses(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < microscope_used; i++) {
        total += microscope_table[i].lenses;
    }

    return total;
}

void microscope_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(microscope_seed) / sizeof(microscope_seed[0]); i++) {
        microscope_add(microscope_seed[i].key, microscope_seed[i].name,
                    microscope_seed[i].lenses);
    }
}

void microscope_report(FILE *out)
{
    int i;

    fprintf(out, "%d microscopes\n", microscope_used);
    for (i = 0; i < microscope_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", microscope_table[i].key,
                microscope_table[i].name, microscope_table[i].lenses);
    }
    fprintf(out, "  total lenses %d\n", microscope_total_lenses());
}

int main(void)
{
    microscope_reset();
    microscope_load_seed();
    microscope_remove(103);
    microscope_report(stdout);

    return 0;
}
