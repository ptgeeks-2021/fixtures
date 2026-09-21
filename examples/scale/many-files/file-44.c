/*
 * A fixed size brewhouse table.
 *
 * It holds up to BREWHOUSE_SLOTS brewhouses, each with a key, a short name, and
 * a litres. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define BREWHOUSE_SLOTS 64
#define BREWHOUSE_NAME_SIZE 24

struct brewhouse {
    int key;
    char name[BREWHOUSE_NAME_SIZE];
    int litres;
};

static const struct brewhouse brewhouse_seed[] = {
    { 101, "brewhouse-0001",  10 },
    { 102, "brewhouse-0002",  17 },
    { 103, "brewhouse-0003",  24 },
    { 104, "brewhouse-0004",  31 },
    { 105, "brewhouse-0005",  38 },
    { 106, "brewhouse-0006",  45 },
    { 107, "brewhouse-0007",  52 },
    { 108, "brewhouse-0008",  59 },
    { 109, "brewhouse-0009",  66 },
    { 110, "brewhouse-0010",  73 },
    { 111, "brewhouse-0011",  80 },
    { 112, "brewhouse-0012",  87 },
    { 113, "brewhouse-0013",  94 },
    { 114, "brewhouse-0014",   4 },
    { 115, "brewhouse-0015",  11 },
    { 116, "brewhouse-0016",  18 },
    { 117, "brewhouse-0017",  25 },
    { 118, "brewhouse-0018",  32 },
    { 119, "brewhouse-0019",  39 },
    { 120, "brewhouse-0020",  46 },
    { 121, "brewhouse-0021",  53 },
    { 122, "brewhouse-0022",  60 },
    { 123, "brewhouse-0023",  67 },
    { 124, "brewhouse-0024",  74 },
    { 125, "brewhouse-0025",  81 },
    { 126, "brewhouse-0026",  88 },
    { 127, "brewhouse-0027",  95 },
    { 128, "brewhouse-0028",   5 },
    { 129, "brewhouse-0029",  12 },
    { 130, "brewhouse-0030",  19 },
    { 131, "brewhouse-0031",  26 },
    { 132, "brewhouse-0032",  33 },
    { 133, "brewhouse-0033",  40 },
    { 134, "brewhouse-0034",  47 },
    { 135, "brewhouse-0035",  54 },
    { 136, "brewhouse-0036",  61 },
    { 137, "brewhouse-0037",  68 },
    { 138, "brewhouse-0038",  75 },
    { 139, "brewhouse-0039",  82 },
    { 140, "brewhouse-0040",  89 },
    { 141, "brewhouse-0041",  96 },
    { 142, "brewhouse-0042",   6 },
    { 143, "brewhouse-0043",  13 },
    { 144, "brewhouse-0044",  20 },
    { 145, "brewhouse-0045",  27 },
    { 146, "brewhouse-0046",  34 },
    { 147, "brewhouse-0047",  41 },
    { 148, "brewhouse-0048",  48 },
};

static struct brewhouse brewhouse_table[BREWHOUSE_SLOTS];
static int brewhouse_used;

void brewhouse_reset(void)
{
    memset(brewhouse_table, 0, sizeof(brewhouse_table));
    brewhouse_used = 0;
}

int brewhouse_add(int key, const char *name, int litres)
{
    struct brewhouse *slot;

    if (brewhouse_used >= BREWHOUSE_SLOTS) {
        return -1;
    }

    slot = &brewhouse_table[brewhouse_used];
    slot->key = key;
    slot->litres = litres;
    strncpy(slot->name, name, BREWHOUSE_NAME_SIZE - 1);
    slot->name[BREWHOUSE_NAME_SIZE - 1] = '\0';
    brewhouse_used++;

    return brewhouse_used - 1;
}

struct brewhouse *brewhouse_find(int key)
{
    int i;

    for (i = 0; i < brewhouse_used; i++) {
        if (brewhouse_table[i].key == key) {
            return &brewhouse_table[i];
        }
    }

    return NULL;
}

int brewhouse_remove(int key)
{
    struct brewhouse *found;

    found = brewhouse_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = brewhouse_table[brewhouse_used - 1];
    brewhouse_used--;

    return 0;
}

int brewhouse_total_litres(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < brewhouse_used; i++) {
        total += brewhouse_table[i].litres;
    }

    return total;
}

void brewhouse_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(brewhouse_seed) / sizeof(brewhouse_seed[0]); i++) {
        brewhouse_add(brewhouse_seed[i].key, brewhouse_seed[i].name,
                    brewhouse_seed[i].litres);
    }
}

void brewhouse_report(FILE *out)
{
    int i;

    fprintf(out, "%d brewhouses\n", brewhouse_used);
    for (i = 0; i < brewhouse_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", brewhouse_table[i].key,
                brewhouse_table[i].name, brewhouse_table[i].litres);
    }
    fprintf(out, "  total litres %d\n", brewhouse_total_litres());
}

int main(void)
{
    brewhouse_reset();
    brewhouse_load_seed();
    brewhouse_remove(103);
    brewhouse_report(stdout);

    return 0;
}
