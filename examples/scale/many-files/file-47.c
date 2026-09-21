/*
 * A fixed size sawmill table.
 *
 * It holds up to SAWMILL_SLOTS sawmills, each with a key, a short name, and
 * a planks. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define SAWMILL_SLOTS 64
#define SAWMILL_NAME_SIZE 24

struct sawmill {
    int key;
    char name[SAWMILL_NAME_SIZE];
    int planks;
};

static const struct sawmill sawmill_seed[] = {
    { 101, "sawmill-0001",  10 },
    { 102, "sawmill-0002",  17 },
    { 103, "sawmill-0003",  24 },
    { 104, "sawmill-0004",  31 },
    { 105, "sawmill-0005",  38 },
    { 106, "sawmill-0006",  45 },
    { 107, "sawmill-0007",  52 },
    { 108, "sawmill-0008",  59 },
    { 109, "sawmill-0009",  66 },
    { 110, "sawmill-0010",  73 },
    { 111, "sawmill-0011",  80 },
    { 112, "sawmill-0012",  87 },
    { 113, "sawmill-0013",  94 },
    { 114, "sawmill-0014",   4 },
    { 115, "sawmill-0015",  11 },
    { 116, "sawmill-0016",  18 },
    { 117, "sawmill-0017",  25 },
    { 118, "sawmill-0018",  32 },
    { 119, "sawmill-0019",  39 },
    { 120, "sawmill-0020",  46 },
    { 121, "sawmill-0021",  53 },
    { 122, "sawmill-0022",  60 },
    { 123, "sawmill-0023",  67 },
    { 124, "sawmill-0024",  74 },
    { 125, "sawmill-0025",  81 },
    { 126, "sawmill-0026",  88 },
    { 127, "sawmill-0027",  95 },
    { 128, "sawmill-0028",   5 },
    { 129, "sawmill-0029",  12 },
    { 130, "sawmill-0030",  19 },
    { 131, "sawmill-0031",  26 },
    { 132, "sawmill-0032",  33 },
    { 133, "sawmill-0033",  40 },
    { 134, "sawmill-0034",  47 },
    { 135, "sawmill-0035",  54 },
    { 136, "sawmill-0036",  61 },
    { 137, "sawmill-0037",  68 },
    { 138, "sawmill-0038",  75 },
    { 139, "sawmill-0039",  82 },
    { 140, "sawmill-0040",  89 },
    { 141, "sawmill-0041",  96 },
    { 142, "sawmill-0042",   6 },
    { 143, "sawmill-0043",  13 },
    { 144, "sawmill-0044",  20 },
    { 145, "sawmill-0045",  27 },
    { 146, "sawmill-0046",  34 },
    { 147, "sawmill-0047",  41 },
    { 148, "sawmill-0048",  48 },
};

static struct sawmill sawmill_table[SAWMILL_SLOTS];
static int sawmill_used;

void sawmill_reset(void)
{
    memset(sawmill_table, 0, sizeof(sawmill_table));
    sawmill_used = 0;
}

int sawmill_add(int key, const char *name, int planks)
{
    struct sawmill *slot;

    if (sawmill_used >= SAWMILL_SLOTS) {
        return -1;
    }

    slot = &sawmill_table[sawmill_used];
    slot->key = key;
    slot->planks = planks;
    strncpy(slot->name, name, SAWMILL_NAME_SIZE - 1);
    slot->name[SAWMILL_NAME_SIZE - 1] = '\0';
    sawmill_used++;

    return sawmill_used - 1;
}

struct sawmill *sawmill_find(int key)
{
    int i;

    for (i = 0; i < sawmill_used; i++) {
        if (sawmill_table[i].key == key) {
            return &sawmill_table[i];
        }
    }

    return NULL;
}

int sawmill_remove(int key)
{
    struct sawmill *found;

    found = sawmill_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = sawmill_table[sawmill_used - 1];
    sawmill_used--;

    return 0;
}

int sawmill_total_planks(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < sawmill_used; i++) {
        total += sawmill_table[i].planks;
    }

    return total;
}

void sawmill_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(sawmill_seed) / sizeof(sawmill_seed[0]); i++) {
        sawmill_add(sawmill_seed[i].key, sawmill_seed[i].name,
                    sawmill_seed[i].planks);
    }
}

void sawmill_report(FILE *out)
{
    int i;

    fprintf(out, "%d sawmills\n", sawmill_used);
    for (i = 0; i < sawmill_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", sawmill_table[i].key,
                sawmill_table[i].name, sawmill_table[i].planks);
    }
    fprintf(out, "  total planks %d\n", sawmill_total_planks());
}

int main(void)
{
    sawmill_reset();
    sawmill_load_seed();
    sawmill_remove(103);
    sawmill_report(stdout);

    return 0;
}
