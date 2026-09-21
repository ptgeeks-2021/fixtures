/*
 * A fixed size smelter table.
 *
 * It holds up to SMELTER_SLOTS smelters, each with a key, a short name, and
 * a ingots. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define SMELTER_SLOTS 64
#define SMELTER_NAME_SIZE 24

struct smelter {
    int key;
    char name[SMELTER_NAME_SIZE];
    int ingots;
};

static const struct smelter smelter_seed[] = {
    { 101, "smelter-0001",  10 },
    { 102, "smelter-0002",  17 },
    { 103, "smelter-0003",  24 },
    { 104, "smelter-0004",  31 },
    { 105, "smelter-0005",  38 },
    { 106, "smelter-0006",  45 },
    { 107, "smelter-0007",  52 },
    { 108, "smelter-0008",  59 },
    { 109, "smelter-0009",  66 },
    { 110, "smelter-0010",  73 },
    { 111, "smelter-0011",  80 },
    { 112, "smelter-0012",  87 },
    { 113, "smelter-0013",  94 },
    { 114, "smelter-0014",   4 },
    { 115, "smelter-0015",  11 },
    { 116, "smelter-0016",  18 },
    { 117, "smelter-0017",  25 },
    { 118, "smelter-0018",  32 },
    { 119, "smelter-0019",  39 },
    { 120, "smelter-0020",  46 },
    { 121, "smelter-0021",  53 },
    { 122, "smelter-0022",  60 },
    { 123, "smelter-0023",  67 },
    { 124, "smelter-0024",  74 },
    { 125, "smelter-0025",  81 },
    { 126, "smelter-0026",  88 },
    { 127, "smelter-0027",  95 },
    { 128, "smelter-0028",   5 },
    { 129, "smelter-0029",  12 },
    { 130, "smelter-0030",  19 },
    { 131, "smelter-0031",  26 },
    { 132, "smelter-0032",  33 },
    { 133, "smelter-0033",  40 },
    { 134, "smelter-0034",  47 },
    { 135, "smelter-0035",  54 },
    { 136, "smelter-0036",  61 },
    { 137, "smelter-0037",  68 },
    { 138, "smelter-0038",  75 },
    { 139, "smelter-0039",  82 },
    { 140, "smelter-0040",  89 },
    { 141, "smelter-0041",  96 },
    { 142, "smelter-0042",   6 },
    { 143, "smelter-0043",  13 },
    { 144, "smelter-0044",  20 },
    { 145, "smelter-0045",  27 },
    { 146, "smelter-0046",  34 },
    { 147, "smelter-0047",  41 },
    { 148, "smelter-0048",  48 },
};

static struct smelter smelter_table[SMELTER_SLOTS];
static int smelter_used;

void smelter_reset(void)
{
    memset(smelter_table, 0, sizeof(smelter_table));
    smelter_used = 0;
}

int smelter_add(int key, const char *name, int ingots)
{
    struct smelter *slot;

    if (smelter_used >= SMELTER_SLOTS) {
        return -1;
    }

    slot = &smelter_table[smelter_used];
    slot->key = key;
    slot->ingots = ingots;
    strncpy(slot->name, name, SMELTER_NAME_SIZE - 1);
    slot->name[SMELTER_NAME_SIZE - 1] = '\0';
    smelter_used++;

    return smelter_used - 1;
}

struct smelter *smelter_find(int key)
{
    int i;

    for (i = 0; i < smelter_used; i++) {
        if (smelter_table[i].key == key) {
            return &smelter_table[i];
        }
    }

    return NULL;
}

int smelter_remove(int key)
{
    struct smelter *found;

    found = smelter_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = smelter_table[smelter_used - 1];
    smelter_used--;

    return 0;
}

int smelter_total_ingots(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < smelter_used; i++) {
        total += smelter_table[i].ingots;
    }

    return total;
}

void smelter_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(smelter_seed) / sizeof(smelter_seed[0]); i++) {
        smelter_add(smelter_seed[i].key, smelter_seed[i].name,
                    smelter_seed[i].ingots);
    }
}

void smelter_report(FILE *out)
{
    int i;

    fprintf(out, "%d smelters\n", smelter_used);
    for (i = 0; i < smelter_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", smelter_table[i].key,
                smelter_table[i].name, smelter_table[i].ingots);
    }
    fprintf(out, "  total ingots %d\n", smelter_total_ingots());
}

int main(void)
{
    smelter_reset();
    smelter_load_seed();
    smelter_remove(103);
    smelter_report(stdout);

    return 0;
}
