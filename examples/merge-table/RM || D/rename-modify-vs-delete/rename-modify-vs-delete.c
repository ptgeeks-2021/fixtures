/*
 * A fixed size lantern table.
 *
 * It holds up to LANTERN_SLOTS lanterns, each with a key, a short name, and
 * a lumens. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define LANTERN_SLOTS 64
#define LANTERN_NAME_SIZE 24

struct lantern {
    int key;
    char name[LANTERN_NAME_SIZE];
    int lumens;
};

static const struct lantern lantern_seed[] = {
    { 101, "lantern-0001",  10 },
    { 102, "lantern-0002",  17 },
    { 103, "lantern-0003",  24 },
    { 104, "lantern-0004",  31 },
    { 105, "lantern-0005",  38 },
    { 106, "lantern-0006",  45 },
    { 107, "lantern-0007",  52 },
    { 108, "lantern-0008",  59 },
    { 109, "lantern-0009",  66 },
    { 110, "lantern-0010",  73 },
    { 111, "lantern-0011",  80 },
    { 112, "lantern-0012",  87 },
    { 113, "lantern-0013",  94 },
    { 114, "lantern-0014",   4 },
    { 115, "lantern-0015",  11 },
    { 116, "lantern-0016",  18 },
    { 117, "lantern-0017",  25 },
    { 118, "lantern-0018",  32 },
    { 119, "lantern-0019",  39 },
    { 120, "lantern-0020",  46 },
    { 121, "lantern-0021",  53 },
    { 122, "lantern-0022",  60 },
    { 123, "lantern-0023",  67 },
    { 124, "lantern-0024",  74 },
    { 125, "lantern-0025",  81 },
    { 126, "lantern-0026",  88 },
    { 127, "lantern-0027",  95 },
    { 128, "lantern-0028",   5 },
    { 129, "lantern-0029",  12 },
    { 130, "lantern-0030",  19 },
    { 131, "lantern-0031",  26 },
    { 132, "lantern-0032",  33 },
    { 133, "lantern-0033",  40 },
    { 134, "lantern-0034",  47 },
    { 135, "lantern-0035",  54 },
    { 136, "lantern-0036",  61 },
    { 137, "lantern-0037",  68 },
    { 138, "lantern-0038",  75 },
    { 139, "lantern-0039",  82 },
    { 140, "lantern-0040",  89 },
    { 141, "lantern-0041",  96 },
    { 142, "lantern-0042",   6 },
    { 143, "lantern-0043",  13 },
    { 144, "lantern-0044",  20 },
    { 145, "lantern-0045",  27 },
    { 146, "lantern-0046",  34 },
    { 147, "lantern-0047",  41 },
    { 148, "lantern-0048",  48 },
};

static struct lantern lantern_table[LANTERN_SLOTS];
static int lantern_used;

void lantern_reset(void)
{
    memset(lantern_table, 0, sizeof(lantern_table));
    lantern_used = 0;
}

int lantern_add(int key, const char *name, int lumens)
{
    struct lantern *slot;

    if (lantern_used >= LANTERN_SLOTS) {
        return -1;
    }

    slot = &lantern_table[lantern_used];
    slot->key = key;
    slot->lumens = lumens;
    strncpy(slot->name, name, LANTERN_NAME_SIZE - 1);
    slot->name[LANTERN_NAME_SIZE - 1] = '\0';
    lantern_used++;

    return lantern_used - 1;
}

struct lantern *lantern_find(int key)
{
    int i;

    for (i = 0; i < lantern_used; i++) {
        if (lantern_table[i].key == key) {
            return &lantern_table[i];
        }
    }

    return NULL;
}

int lantern_remove(int key)
{
    struct lantern *found;

    found = lantern_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = lantern_table[lantern_used - 1];
    lantern_used--;

    return 0;
}

int lantern_total_lumens(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < lantern_used; i++) {
        total += lantern_table[i].lumens;
    }

    return total;
}

void lantern_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(lantern_seed) / sizeof(lantern_seed[0]); i++) {
        lantern_add(lantern_seed[i].key, lantern_seed[i].name,
                    lantern_seed[i].lumens);
    }
}

void lantern_report(FILE *out)
{
    int i;

    fprintf(out, "%d lanterns\n", lantern_used);
    for (i = 0; i < lantern_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", lantern_table[i].key,
                lantern_table[i].name, lantern_table[i].lumens);
    }
    fprintf(out, "  total lumens %d\n", lantern_total_lumens());
}

int main(void)
{
    lantern_reset();
    lantern_load_seed();
    lantern_remove(103);
    lantern_report(stdout);

    return 0;
}
