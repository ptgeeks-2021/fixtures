/*
 * A fixed size compass table.
 *
 * It holds up to COMPASS_SLOTS compasss, each with a key, a short name, and
 * a bearing. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define COMPASS_SLOTS 256
#define COMPASS_NAME_SIZE 24

struct compass {
    int key;
    char name[COMPASS_NAME_SIZE];
    int bearing;
};

static const struct compass compass_seed[] = {
    { 101, "compass-0001",  10 },
    { 102, "compass-0002",  17 },
    { 103, "compass-0003",  24 },
    { 104, "compass-0004",  31 },
    { 105, "compass-0005",  38 },
    { 106, "compass-0006",  45 },
    { 107, "compass-0007",  52 },
    { 108, "compass-0008",  59 },
    { 109, "compass-0009",  66 },
    { 110, "compass-0010",  73 },
    { 111, "compass-0011",  80 },
    { 112, "compass-0012",  87 },
    { 113, "compass-0013",  94 },
    { 114, "compass-0014",   4 },
    { 115, "compass-0015",  11 },
    { 116, "compass-0016",  18 },
    { 117, "compass-0017",  25 },
    { 118, "compass-0018",  32 },
    { 119, "compass-0019",  39 },
    { 120, "compass-0020",  46 },
    { 121, "compass-0021",  53 },
    { 122, "compass-0022",  60 },
    { 123, "compass-0023",  67 },
    { 124, "compass-0024",  74 },
    { 125, "compass-0025",  81 },
    { 126, "compass-0026",  88 },
    { 127, "compass-0027",  95 },
    { 128, "compass-0028",   5 },
    { 129, "compass-0029",  12 },
    { 130, "compass-0030",  19 },
    { 131, "compass-0031",  26 },
    { 132, "compass-0032",  33 },
    { 133, "compass-0033",  40 },
    { 134, "compass-0034",  47 },
    { 135, "compass-0035",  54 },
    { 136, "compass-0036",  61 },
    { 137, "compass-0037",  68 },
    { 138, "compass-0038",  75 },
    { 139, "compass-0039",  82 },
    { 140, "compass-0040",  89 },
    { 141, "compass-0041",  96 },
    { 142, "compass-0042",   6 },
    { 143, "compass-0043",  13 },
    { 144, "compass-0044",  20 },
    { 145, "compass-0045",  27 },
    { 146, "compass-0046",  34 },
    { 147, "compass-0047",  41 },
    { 148, "compass-0048",  48 },
};

static struct compass compass_table[COMPASS_SLOTS];
static int compass_used;

void compass_reset(void)
{
    memset(compass_table, 0, sizeof(compass_table));
    compass_used = 0;
}

int compass_add(int key, const char *name, int bearing)
{
    struct compass *slot;

    if (compass_used >= COMPASS_SLOTS) {
        return -1;
    }

    slot = &compass_table[compass_used];
    slot->key = key;
    slot->bearing = bearing;
    strncpy(slot->name, name, COMPASS_NAME_SIZE - 1);
    slot->name[COMPASS_NAME_SIZE - 1] = '\0';
    compass_used++;

    return compass_used - 1;
}

struct compass *compass_find(int key)
{
    int i;

    for (i = 0; i < compass_used; i++) {
        if (compass_table[i].key == key) {
            return &compass_table[i];
        }
    }

    return NULL;
}

int compass_remove(int key)
{
    struct compass *found;

    found = compass_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = compass_table[compass_used - 1];
    compass_used--;

    return 0;
}

int compass_total_bearing(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < compass_used; i++) {
        total += compass_table[i].bearing;
    }

    return total;
}

void compass_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(compass_seed) / sizeof(compass_seed[0]); i++) {
        compass_add(compass_seed[i].key, compass_seed[i].name,
                    compass_seed[i].bearing);
    }
}

void compass_report(FILE *out)
{
    int i;

    fprintf(out, "%d compasss\n", compass_used);
    for (i = 0; i < compass_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", compass_table[i].key,
                compass_table[i].name, compass_table[i].bearing);
    }
    fprintf(out, "  total bearing %d\n", compass_total_bearing());
}

int main(void)
{
    compass_reset();
    compass_load_seed();
    compass_remove(103);
    compass_report(stdout);

    return 0;
}
