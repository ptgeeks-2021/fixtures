/*
 * A fixed size sextant table.
 *
 * It holds up to SEXTANT_SLOTS sextants, each with a key, a short name, and
 * a degrees. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define SEXTANT_SLOTS 128
#define SEXTANT_NAME_SIZE 24

struct sextant {
    int key;
    char name[SEXTANT_NAME_SIZE];
    int degrees;
};

static const struct sextant sextant_seed[] = {
    { 101, "sextant-0001",  10 },
    { 102, "sextant-0002",  17 },
    { 103, "sextant-0003",  24 },
    { 104, "sextant-0004",  31 },
    { 105, "sextant-0005",  38 },
    { 106, "sextant-0006",  45 },
    { 107, "sextant-0007",  52 },
    { 108, "sextant-0008",  59 },
    { 109, "sextant-0009",  66 },
    { 110, "sextant-0010",  73 },
    { 111, "sextant-0011",  80 },
    { 112, "sextant-0012",  87 },
    { 113, "sextant-0013",  94 },
    { 114, "sextant-0014",   4 },
    { 115, "sextant-0015",  11 },
    { 116, "sextant-0016",  18 },
    { 117, "sextant-0017",  25 },
    { 118, "sextant-0018",  32 },
    { 119, "sextant-0019",  39 },
    { 120, "sextant-0020",  46 },
    { 121, "sextant-0021",  53 },
    { 122, "sextant-0022",  60 },
    { 123, "sextant-0023",  67 },
    { 124, "sextant-0024",  74 },
    { 125, "sextant-0025",  81 },
    { 126, "sextant-0026",  88 },
    { 127, "sextant-0027",  95 },
    { 128, "sextant-0028",   5 },
    { 129, "sextant-0029",  12 },
    { 130, "sextant-0030",  19 },
    { 131, "sextant-0031",  26 },
    { 132, "sextant-0032",  33 },
    { 133, "sextant-0033",  40 },
    { 134, "sextant-0034",  47 },
    { 135, "sextant-0035",  54 },
    { 136, "sextant-0036",  61 },
    { 137, "sextant-0037",  68 },
    { 138, "sextant-0038",  75 },
    { 139, "sextant-0039",  82 },
    { 140, "sextant-0040",  89 },
    { 141, "sextant-0041",  96 },
    { 142, "sextant-0042",   6 },
    { 143, "sextant-0043",  13 },
    { 144, "sextant-0044",  20 },
    { 145, "sextant-0045",  27 },
    { 146, "sextant-0046",  34 },
    { 147, "sextant-0047",  41 },
    { 148, "sextant-0048",  48 },
};

static struct sextant sextant_table[SEXTANT_SLOTS];
static int sextant_used;

void sextant_reset(void)
{
    memset(sextant_table, 0, sizeof(sextant_table));
    sextant_used = 0;
}

int sextant_add(int key, const char *name, int degrees)
{
    struct sextant *slot;

    if (sextant_used >= SEXTANT_SLOTS) {
        return -1;
    }

    slot = &sextant_table[sextant_used];
    slot->key = key;
    slot->degrees = degrees;
    strncpy(slot->name, name, SEXTANT_NAME_SIZE - 1);
    slot->name[SEXTANT_NAME_SIZE - 1] = '\0';
    sextant_used++;

    return sextant_used - 1;
}

struct sextant *sextant_find(int key)
{
    int i;

    for (i = 0; i < sextant_used; i++) {
        if (sextant_table[i].key == key) {
            return &sextant_table[i];
        }
    }

    return NULL;
}

int sextant_remove(int key)
{
    struct sextant *found;

    found = sextant_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = sextant_table[sextant_used - 1];
    sextant_used--;

    return 0;
}

int sextant_total_degrees(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < sextant_used; i++) {
        total += sextant_table[i].degrees;
    }

    return total;
}

void sextant_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(sextant_seed) / sizeof(sextant_seed[0]); i++) {
        sextant_add(sextant_seed[i].key, sextant_seed[i].name,
                    sextant_seed[i].degrees);
    }
}

void sextant_report(FILE *out)
{
    int i;

    fprintf(out, "%d sextants\n", sextant_used);
    for (i = 0; i < sextant_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", sextant_table[i].key,
                sextant_table[i].name, sextant_table[i].degrees);
    }
    fprintf(out, "  total degrees %d\n", sextant_total_degrees());
}

int main(void)
{
    sextant_reset();
    sextant_load_seed();
    sextant_remove(103);
    sextant_report(stdout);

    return 0;
}
