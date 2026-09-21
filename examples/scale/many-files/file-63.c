/*
 * A fixed size incubator table.
 *
 * It holds up to INCUBATOR_SLOTS incubators, each with a key, a short name, and
 * a trays. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define INCUBATOR_SLOTS 64
#define INCUBATOR_NAME_SIZE 24

struct incubator {
    int key;
    char name[INCUBATOR_NAME_SIZE];
    int trays;
};

static const struct incubator incubator_seed[] = {
    { 101, "incubator-0001",  10 },
    { 102, "incubator-0002",  17 },
    { 103, "incubator-0003",  24 },
    { 104, "incubator-0004",  31 },
    { 105, "incubator-0005",  38 },
    { 106, "incubator-0006",  45 },
    { 107, "incubator-0007",  52 },
    { 108, "incubator-0008",  59 },
    { 109, "incubator-0009",  66 },
    { 110, "incubator-0010",  73 },
    { 111, "incubator-0011",  80 },
    { 112, "incubator-0012",  87 },
    { 113, "incubator-0013",  94 },
    { 114, "incubator-0014",   4 },
    { 115, "incubator-0015",  11 },
    { 116, "incubator-0016",  18 },
    { 117, "incubator-0017",  25 },
    { 118, "incubator-0018",  32 },
    { 119, "incubator-0019",  39 },
    { 120, "incubator-0020",  46 },
    { 121, "incubator-0021",  53 },
    { 122, "incubator-0022",  60 },
    { 123, "incubator-0023",  67 },
    { 124, "incubator-0024",  74 },
    { 125, "incubator-0025",  81 },
    { 126, "incubator-0026",  88 },
    { 127, "incubator-0027",  95 },
    { 128, "incubator-0028",   5 },
    { 129, "incubator-0029",  12 },
    { 130, "incubator-0030",  19 },
    { 131, "incubator-0031",  26 },
    { 132, "incubator-0032",  33 },
    { 133, "incubator-0033",  40 },
    { 134, "incubator-0034",  47 },
    { 135, "incubator-0035",  54 },
    { 136, "incubator-0036",  61 },
    { 137, "incubator-0037",  68 },
    { 138, "incubator-0038",  75 },
    { 139, "incubator-0039",  82 },
    { 140, "incubator-0040",  89 },
    { 141, "incubator-0041",  96 },
    { 142, "incubator-0042",   6 },
    { 143, "incubator-0043",  13 },
    { 144, "incubator-0044",  20 },
    { 145, "incubator-0045",  27 },
    { 146, "incubator-0046",  34 },
    { 147, "incubator-0047",  41 },
    { 148, "incubator-0048",  48 },
};

static struct incubator incubator_table[INCUBATOR_SLOTS];
static int incubator_used;

void incubator_reset(void)
{
    memset(incubator_table, 0, sizeof(incubator_table));
    incubator_used = 0;
}

int incubator_add(int key, const char *name, int trays)
{
    struct incubator *slot;

    if (incubator_used >= INCUBATOR_SLOTS) {
        return -1;
    }

    slot = &incubator_table[incubator_used];
    slot->key = key;
    slot->trays = trays;
    strncpy(slot->name, name, INCUBATOR_NAME_SIZE - 1);
    slot->name[INCUBATOR_NAME_SIZE - 1] = '\0';
    incubator_used++;

    return incubator_used - 1;
}

struct incubator *incubator_find(int key)
{
    int i;

    for (i = 0; i < incubator_used; i++) {
        if (incubator_table[i].key == key) {
            return &incubator_table[i];
        }
    }

    return NULL;
}

int incubator_remove(int key)
{
    struct incubator *found;

    found = incubator_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = incubator_table[incubator_used - 1];
    incubator_used--;

    return 0;
}

int incubator_total_trays(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < incubator_used; i++) {
        total += incubator_table[i].trays;
    }

    return total;
}

void incubator_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(incubator_seed) / sizeof(incubator_seed[0]); i++) {
        incubator_add(incubator_seed[i].key, incubator_seed[i].name,
                    incubator_seed[i].trays);
    }
}

void incubator_report(FILE *out)
{
    int i;

    fprintf(out, "%d incubators\n", incubator_used);
    for (i = 0; i < incubator_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", incubator_table[i].key,
                incubator_table[i].name, incubator_table[i].trays);
    }
    fprintf(out, "  total trays %d\n", incubator_total_trays());
}

int main(void)
{
    incubator_reset();
    incubator_load_seed();
    incubator_remove(103);
    incubator_report(stdout);

    return 0;
}
