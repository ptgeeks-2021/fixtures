/*
 * A fixed size bakehouse table.
 *
 * It holds up to BAKEHOUSE_SLOTS bakehouses, each with a key, a short name, and
 * a loaves. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define BAKEHOUSE_SLOTS 128
#define BAKEHOUSE_NAME_SIZE 24

struct bakehouse {
    int key;
    char name[BAKEHOUSE_NAME_SIZE];
    int loaves;
};

static const struct bakehouse bakehouse_seed[] = {
    { 101, "bakehouse-0001",  10 },
    { 102, "bakehouse-0002",  17 },
    { 103, "bakehouse-0003",  24 },
    { 104, "bakehouse-0004",  31 },
    { 105, "bakehouse-0005",  38 },
    { 106, "bakehouse-0006",  45 },
    { 107, "bakehouse-0007",  52 },
    { 108, "bakehouse-0008",  59 },
    { 109, "bakehouse-0009",  66 },
    { 110, "bakehouse-0010",  73 },
    { 111, "bakehouse-0011",  80 },
    { 112, "bakehouse-0012",  87 },
    { 113, "bakehouse-0013",  94 },
    { 114, "bakehouse-0014",   4 },
    { 115, "bakehouse-0015",  11 },
    { 116, "bakehouse-0016",  18 },
    { 117, "bakehouse-0017",  25 },
    { 118, "bakehouse-0018",  32 },
    { 119, "bakehouse-0019",  39 },
    { 120, "bakehouse-0020",  46 },
    { 121, "bakehouse-0021",  53 },
    { 122, "bakehouse-0022",  60 },
    { 123, "bakehouse-0023",  67 },
    { 124, "bakehouse-0024",  74 },
    { 125, "bakehouse-0025",  81 },
    { 126, "bakehouse-0026",  88 },
    { 127, "bakehouse-0027",  95 },
    { 128, "bakehouse-0028",   5 },
    { 129, "bakehouse-0029",  12 },
    { 130, "bakehouse-0030",  19 },
    { 131, "bakehouse-0031",  26 },
    { 132, "bakehouse-0032",  33 },
    { 133, "bakehouse-0033",  40 },
    { 134, "bakehouse-0034",  47 },
    { 135, "bakehouse-0035",  54 },
    { 136, "bakehouse-0036",  61 },
    { 137, "bakehouse-0037",  68 },
    { 138, "bakehouse-0038",  75 },
    { 139, "bakehouse-0039",  82 },
    { 140, "bakehouse-0040",  89 },
    { 141, "bakehouse-0041",  96 },
    { 142, "bakehouse-0042",   6 },
    { 143, "bakehouse-0043",  13 },
    { 144, "bakehouse-0044",  20 },
    { 145, "bakehouse-0045",  27 },
    { 146, "bakehouse-0046",  34 },
    { 147, "bakehouse-0047",  41 },
    { 148, "bakehouse-0048",  48 },
};

static struct bakehouse bakehouse_table[BAKEHOUSE_SLOTS];
static int bakehouse_used;

void bakehouse_reset(void)
{
    memset(bakehouse_table, 0, sizeof(bakehouse_table));
    bakehouse_used = 0;
}

int bakehouse_add(int key, const char *name, int loaves)
{
    struct bakehouse *slot;

    if (bakehouse_used >= BAKEHOUSE_SLOTS) {
        return -1;
    }

    slot = &bakehouse_table[bakehouse_used];
    slot->key = key;
    slot->loaves = loaves;
    strncpy(slot->name, name, BAKEHOUSE_NAME_SIZE - 1);
    slot->name[BAKEHOUSE_NAME_SIZE - 1] = '\0';
    bakehouse_used++;

    return bakehouse_used - 1;
}

struct bakehouse *bakehouse_find(int key)
{
    int i;

    for (i = 0; i < bakehouse_used; i++) {
        if (bakehouse_table[i].key == key) {
            return &bakehouse_table[i];
        }
    }

    return NULL;
}

int bakehouse_remove(int key)
{
    struct bakehouse *found;

    found = bakehouse_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = bakehouse_table[bakehouse_used - 1];
    bakehouse_used--;

    return 0;
}

int bakehouse_total_loaves(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < bakehouse_used; i++) {
        total += bakehouse_table[i].loaves;
    }

    return total;
}

void bakehouse_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(bakehouse_seed) / sizeof(bakehouse_seed[0]); i++) {
        bakehouse_add(bakehouse_seed[i].key, bakehouse_seed[i].name,
                    bakehouse_seed[i].loaves);
    }
}

void bakehouse_report(FILE *out)
{
    int i;

    fprintf(out, "%d bakehouses\n", bakehouse_used);
    for (i = 0; i < bakehouse_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", bakehouse_table[i].key,
                bakehouse_table[i].name, bakehouse_table[i].loaves);
    }
    fprintf(out, "  total loaves %d\n", bakehouse_total_loaves());
}

int main(void)
{
    bakehouse_reset();
    bakehouse_load_seed();
    bakehouse_remove(103);
    bakehouse_report(stdout);

    return 0;
}
