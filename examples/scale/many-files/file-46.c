/*
 * A fixed size mineshaft table.
 *
 * It holds up to MINESHAFT_SLOTS mineshafts, each with a key, a short name, and
 * a tonnes. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define MINESHAFT_SLOTS 96
#define MINESHAFT_NAME_SIZE 24

struct mineshaft {
    int key;
    char name[MINESHAFT_NAME_SIZE];
    int tonnes;
};

static const struct mineshaft mineshaft_seed[] = {
    { 101, "mineshaft-0001",  10 },
    { 102, "mineshaft-0002",  17 },
    { 103, "mineshaft-0003",  24 },
    { 104, "mineshaft-0004",  31 },
    { 105, "mineshaft-0005",  38 },
    { 106, "mineshaft-0006",  45 },
    { 107, "mineshaft-0007",  52 },
    { 108, "mineshaft-0008",  59 },
    { 109, "mineshaft-0009",  66 },
    { 110, "mineshaft-0010",  73 },
    { 111, "mineshaft-0011",  80 },
    { 112, "mineshaft-0012",  87 },
    { 113, "mineshaft-0013",  94 },
    { 114, "mineshaft-0014",   4 },
    { 115, "mineshaft-0015",  11 },
    { 116, "mineshaft-0016",  18 },
    { 117, "mineshaft-0017",  25 },
    { 118, "mineshaft-0018",  32 },
    { 119, "mineshaft-0019",  39 },
    { 120, "mineshaft-0020",  46 },
    { 121, "mineshaft-0021",  53 },
    { 122, "mineshaft-0022",  60 },
    { 123, "mineshaft-0023",  67 },
    { 124, "mineshaft-0024",  74 },
    { 125, "mineshaft-0025",  81 },
    { 126, "mineshaft-0026",  88 },
    { 127, "mineshaft-0027",  95 },
    { 128, "mineshaft-0028",   5 },
    { 129, "mineshaft-0029",  12 },
    { 130, "mineshaft-0030",  19 },
    { 131, "mineshaft-0031",  26 },
    { 132, "mineshaft-0032",  33 },
    { 133, "mineshaft-0033",  40 },
    { 134, "mineshaft-0034",  47 },
    { 135, "mineshaft-0035",  54 },
    { 136, "mineshaft-0036",  61 },
    { 137, "mineshaft-0037",  68 },
    { 138, "mineshaft-0038",  75 },
    { 139, "mineshaft-0039",  82 },
    { 140, "mineshaft-0040",  89 },
    { 141, "mineshaft-0041",  96 },
    { 142, "mineshaft-0042",   6 },
    { 143, "mineshaft-0043",  13 },
    { 144, "mineshaft-0044",  20 },
    { 145, "mineshaft-0045",  27 },
    { 146, "mineshaft-0046",  34 },
    { 147, "mineshaft-0047",  41 },
    { 148, "mineshaft-0048",  48 },
};

static struct mineshaft mineshaft_table[MINESHAFT_SLOTS];
static int mineshaft_used;

void mineshaft_reset(void)
{
    memset(mineshaft_table, 0, sizeof(mineshaft_table));
    mineshaft_used = 0;
}

int mineshaft_add(int key, const char *name, int tonnes)
{
    struct mineshaft *slot;

    if (mineshaft_used >= MINESHAFT_SLOTS) {
        return -1;
    }

    slot = &mineshaft_table[mineshaft_used];
    slot->key = key;
    slot->tonnes = tonnes;
    strncpy(slot->name, name, MINESHAFT_NAME_SIZE - 1);
    slot->name[MINESHAFT_NAME_SIZE - 1] = '\0';
    mineshaft_used++;

    return mineshaft_used - 1;
}

struct mineshaft *mineshaft_find(int key)
{
    int i;

    for (i = 0; i < mineshaft_used; i++) {
        if (mineshaft_table[i].key == key) {
            return &mineshaft_table[i];
        }
    }

    return NULL;
}

int mineshaft_remove(int key)
{
    struct mineshaft *found;

    found = mineshaft_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = mineshaft_table[mineshaft_used - 1];
    mineshaft_used--;

    return 0;
}

int mineshaft_total_tonnes(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < mineshaft_used; i++) {
        total += mineshaft_table[i].tonnes;
    }

    return total;
}

void mineshaft_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(mineshaft_seed) / sizeof(mineshaft_seed[0]); i++) {
        mineshaft_add(mineshaft_seed[i].key, mineshaft_seed[i].name,
                    mineshaft_seed[i].tonnes);
    }
}

void mineshaft_report(FILE *out)
{
    int i;

    fprintf(out, "%d mineshafts\n", mineshaft_used);
    for (i = 0; i < mineshaft_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", mineshaft_table[i].key,
                mineshaft_table[i].name, mineshaft_table[i].tonnes);
    }
    fprintf(out, "  total tonnes %d\n", mineshaft_total_tonnes());
}

int main(void)
{
    mineshaft_reset();
    mineshaft_load_seed();
    mineshaft_remove(103);
    mineshaft_report(stdout);

    return 0;
}
