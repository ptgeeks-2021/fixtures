/*
 * A fixed size aquarium table.
 *
 * It holds up to AQUARIUM_SLOTS aquariums, each with a key, a short name, and
 * a gallons. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define AQUARIUM_SLOTS 64
#define AQUARIUM_NAME_SIZE 24

struct aquarium {
    int key;
    char name[AQUARIUM_NAME_SIZE];
    int gallons;
};

static const struct aquarium aquarium_seed[] = {
    { 101, "aquarium-0001",  10 },
    { 102, "aquarium-0002",  17 },
    { 103, "aquarium-0003",  24 },
    { 104, "aquarium-0004",  31 },
    { 105, "aquarium-0005",  38 },
    { 106, "aquarium-0006",  45 },
    { 107, "aquarium-0007",  52 },
    { 108, "aquarium-0008",  59 },
    { 109, "aquarium-0009",  66 },
    { 110, "aquarium-0010",  73 },
    { 111, "aquarium-0011",  80 },
    { 112, "aquarium-0012",  87 },
    { 113, "aquarium-0013",  94 },
    { 114, "aquarium-0014",   4 },
    { 115, "aquarium-0015",  11 },
    { 116, "aquarium-0016",  18 },
    { 117, "aquarium-0017",  25 },
    { 118, "aquarium-0018",  32 },
    { 119, "aquarium-0019",  39 },
    { 120, "aquarium-0020",  46 },
    { 121, "aquarium-0021",  53 },
    { 122, "aquarium-0022",  60 },
    { 123, "aquarium-0023",  67 },
    { 124, "aquarium-0024",  74 },
    { 125, "aquarium-0025",  81 },
    { 126, "aquarium-0026",  88 },
    { 127, "aquarium-0027",  95 },
    { 128, "aquarium-0028",   5 },
    { 129, "aquarium-0029",  12 },
    { 130, "aquarium-0030",  19 },
    { 131, "aquarium-0031",  26 },
    { 132, "aquarium-0032",  33 },
    { 133, "aquarium-0033",  40 },
    { 134, "aquarium-0034",  47 },
    { 135, "aquarium-0035",  54 },
    { 136, "aquarium-0036",  61 },
    { 137, "aquarium-0037",  68 },
    { 138, "aquarium-0038",  75 },
    { 139, "aquarium-0039",  82 },
    { 140, "aquarium-0040",  89 },
    { 141, "aquarium-0041",  96 },
    { 142, "aquarium-0042",   6 },
    { 143, "aquarium-0043",  13 },
    { 144, "aquarium-0044",  20 },
    { 145, "aquarium-0045",  27 },
    { 146, "aquarium-0046",  34 },
    { 147, "aquarium-0047",  41 },
    { 148, "aquarium-0048",  48 },
};

static struct aquarium aquarium_table[AQUARIUM_SLOTS];
static int aquarium_used;

void aquarium_reset(void)
{
    memset(aquarium_table, 0, sizeof(aquarium_table));
    aquarium_used = 0;
}

int aquarium_add(int key, const char *name, int gallons)
{
    struct aquarium *slot;

    if (aquarium_used >= AQUARIUM_SLOTS) {
        return -1;
    }

    slot = &aquarium_table[aquarium_used];
    slot->key = key;
    slot->gallons = gallons;
    strncpy(slot->name, name, AQUARIUM_NAME_SIZE - 1);
    slot->name[AQUARIUM_NAME_SIZE - 1] = '\0';
    aquarium_used++;

    return aquarium_used - 1;
}

struct aquarium *aquarium_find(int key)
{
    int i;

    for (i = 0; i < aquarium_used; i++) {
        if (aquarium_table[i].key == key) {
            return &aquarium_table[i];
        }
    }

    return NULL;
}

int aquarium_remove(int key)
{
    struct aquarium *found;

    found = aquarium_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = aquarium_table[aquarium_used - 1];
    aquarium_used--;

    return 0;
}

int aquarium_total_gallons(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < aquarium_used; i++) {
        total += aquarium_table[i].gallons;
    }

    return total;
}

void aquarium_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(aquarium_seed) / sizeof(aquarium_seed[0]); i++) {
        aquarium_add(aquarium_seed[i].key, aquarium_seed[i].name,
                    aquarium_seed[i].gallons);
    }
}

void aquarium_report(FILE *out)
{
    int i;

    fprintf(out, "%d aquariums\n", aquarium_used);
    for (i = 0; i < aquarium_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", aquarium_table[i].key,
                aquarium_table[i].name, aquarium_table[i].gallons);
    }
    fprintf(out, "  total gallons %d\n", aquarium_total_gallons());
}

int main(void)
{
    aquarium_reset();
    aquarium_load_seed();
    aquarium_remove(103);
    aquarium_report(stdout);

    return 0;
}
