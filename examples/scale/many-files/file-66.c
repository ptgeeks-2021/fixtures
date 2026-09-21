/*
 * A fixed size windmill table.
 *
 * It holds up to WINDMILL_SLOTS windmills, each with a key, a short name, and
 * a sails. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define WINDMILL_SLOTS 64
#define WINDMILL_NAME_SIZE 24

struct windmill {
    int key;
    char name[WINDMILL_NAME_SIZE];
    int sails;
};

static const struct windmill windmill_seed[] = {
    { 101, "windmill-0001",  10 },
    { 102, "windmill-0002",  17 },
    { 103, "windmill-0003",  24 },
    { 104, "windmill-0004",  31 },
    { 105, "windmill-0005",  38 },
    { 106, "windmill-0006",  45 },
    { 107, "windmill-0007",  52 },
    { 108, "windmill-0008",  59 },
    { 109, "windmill-0009",  66 },
    { 110, "windmill-0010",  73 },
    { 111, "windmill-0011",  80 },
    { 112, "windmill-0012",  87 },
    { 113, "windmill-0013",  94 },
    { 114, "windmill-0014",   4 },
    { 115, "windmill-0015",  11 },
    { 116, "windmill-0016",  18 },
    { 117, "windmill-0017",  25 },
    { 118, "windmill-0018",  32 },
    { 119, "windmill-0019",  39 },
    { 120, "windmill-0020",  46 },
    { 121, "windmill-0021",  53 },
    { 122, "windmill-0022",  60 },
    { 123, "windmill-0023",  67 },
    { 124, "windmill-0024",  74 },
    { 125, "windmill-0025",  81 },
    { 126, "windmill-0026",  88 },
    { 127, "windmill-0027",  95 },
    { 128, "windmill-0028",   5 },
    { 129, "windmill-0029",  12 },
    { 130, "windmill-0030",  19 },
    { 131, "windmill-0031",  26 },
    { 132, "windmill-0032",  33 },
    { 133, "windmill-0033",  40 },
    { 134, "windmill-0034",  47 },
    { 135, "windmill-0035",  54 },
    { 136, "windmill-0036",  61 },
    { 137, "windmill-0037",  68 },
    { 138, "windmill-0038",  75 },
    { 139, "windmill-0039",  82 },
    { 140, "windmill-0040",  89 },
    { 141, "windmill-0041",  96 },
    { 142, "windmill-0042",   6 },
    { 143, "windmill-0043",  13 },
    { 144, "windmill-0044",  20 },
    { 145, "windmill-0045",  27 },
    { 146, "windmill-0046",  34 },
    { 147, "windmill-0047",  41 },
    { 148, "windmill-0048",  48 },
};

static struct windmill windmill_table[WINDMILL_SLOTS];
static int windmill_used;

void windmill_reset(void)
{
    memset(windmill_table, 0, sizeof(windmill_table));
    windmill_used = 0;
}

int windmill_add(int key, const char *name, int sails)
{
    struct windmill *slot;

    if (windmill_used >= WINDMILL_SLOTS) {
        return -1;
    }

    slot = &windmill_table[windmill_used];
    slot->key = key;
    slot->sails = sails;
    strncpy(slot->name, name, WINDMILL_NAME_SIZE - 1);
    slot->name[WINDMILL_NAME_SIZE - 1] = '\0';
    windmill_used++;

    return windmill_used - 1;
}

struct windmill *windmill_find(int key)
{
    int i;

    for (i = 0; i < windmill_used; i++) {
        if (windmill_table[i].key == key) {
            return &windmill_table[i];
        }
    }

    return NULL;
}

int windmill_remove(int key)
{
    struct windmill *found;

    found = windmill_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = windmill_table[windmill_used - 1];
    windmill_used--;

    return 0;
}

int windmill_total_sails(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < windmill_used; i++) {
        total += windmill_table[i].sails;
    }

    return total;
}

void windmill_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(windmill_seed) / sizeof(windmill_seed[0]); i++) {
        windmill_add(windmill_seed[i].key, windmill_seed[i].name,
                    windmill_seed[i].sails);
    }
}

void windmill_report(FILE *out)
{
    int i;

    fprintf(out, "%d windmills\n", windmill_used);
    for (i = 0; i < windmill_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", windmill_table[i].key,
                windmill_table[i].name, windmill_table[i].sails);
    }
    fprintf(out, "  total sails %d\n", windmill_total_sails());
}

int main(void)
{
    windmill_reset();
    windmill_load_seed();
    windmill_remove(103);
    windmill_report(stdout);

    return 0;
}
