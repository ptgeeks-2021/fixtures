/*
 * A fixed size pasture table.
 *
 * It holds up to PASTURE_SLOTS pastures, each with a key, a short name, and
 * a hectares. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define PASTURE_SLOTS 96
#define PASTURE_NAME_SIZE 24

struct pasture {
    int key;
    char name[PASTURE_NAME_SIZE];
    int hectares;
};

static const struct pasture pasture_seed[] = {
    { 101, "pasture-0001",  10 },
    { 102, "pasture-0002",  17 },
    { 103, "pasture-0003",  24 },
    { 104, "pasture-0004",  31 },
    { 105, "pasture-0005",  38 },
    { 106, "pasture-0006",  45 },
    { 107, "pasture-0007",  52 },
    { 108, "pasture-0008",  59 },
    { 109, "pasture-0009",  66 },
    { 110, "pasture-0010",  73 },
    { 111, "pasture-0011",  80 },
    { 112, "pasture-0012",  87 },
    { 113, "pasture-0013",  94 },
    { 114, "pasture-0014",   4 },
    { 115, "pasture-0015",  11 },
    { 116, "pasture-0016",  18 },
    { 117, "pasture-0017",  25 },
    { 118, "pasture-0018",  32 },
    { 119, "pasture-0019",  39 },
    { 120, "pasture-0020",  46 },
    { 121, "pasture-0021",  53 },
    { 122, "pasture-0022",  60 },
    { 123, "pasture-0023",  67 },
    { 124, "pasture-0024",  74 },
    { 125, "pasture-0025",  81 },
    { 126, "pasture-0026",  88 },
    { 127, "pasture-0027",  95 },
    { 128, "pasture-0028",   5 },
    { 129, "pasture-0029",  12 },
    { 130, "pasture-0030",  19 },
    { 131, "pasture-0031",  26 },
    { 132, "pasture-0032",  33 },
    { 133, "pasture-0033",  40 },
    { 134, "pasture-0034",  47 },
    { 135, "pasture-0035",  54 },
    { 136, "pasture-0036",  61 },
    { 137, "pasture-0037",  68 },
    { 138, "pasture-0038",  75 },
    { 139, "pasture-0039",  82 },
    { 140, "pasture-0040",  89 },
    { 141, "pasture-0041",  96 },
    { 142, "pasture-0042",   6 },
    { 143, "pasture-0043",  13 },
    { 144, "pasture-0044",  20 },
    { 145, "pasture-0045",  27 },
    { 146, "pasture-0046",  34 },
    { 147, "pasture-0047",  41 },
    { 148, "pasture-0048",  48 },
};

static struct pasture pasture_table[PASTURE_SLOTS];
static int pasture_used;

void pasture_reset(void)
{
    memset(pasture_table, 0, sizeof(pasture_table));
    pasture_used = 0;
}

int pasture_add(int key, const char *name, int hectares)
{
    struct pasture *slot;

    if (pasture_used >= PASTURE_SLOTS) {
        return -1;
    }

    slot = &pasture_table[pasture_used];
    slot->key = key;
    slot->hectares = hectares;
    strncpy(slot->name, name, PASTURE_NAME_SIZE - 1);
    slot->name[PASTURE_NAME_SIZE - 1] = '\0';
    pasture_used++;

    return pasture_used - 1;
}

struct pasture *pasture_find(int key)
{
    int i;

    for (i = 0; i < pasture_used; i++) {
        if (pasture_table[i].key == key) {
            return &pasture_table[i];
        }
    }

    return NULL;
}

int pasture_remove(int key)
{
    struct pasture *found;

    found = pasture_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = pasture_table[pasture_used - 1];
    pasture_used--;

    return 0;
}

int pasture_total_hectares(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < pasture_used; i++) {
        total += pasture_table[i].hectares;
    }

    return total;
}

void pasture_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(pasture_seed) / sizeof(pasture_seed[0]); i++) {
        pasture_add(pasture_seed[i].key, pasture_seed[i].name,
                    pasture_seed[i].hectares);
    }
}

void pasture_report(FILE *out)
{
    int i;

    fprintf(out, "%d pastures\n", pasture_used);
    for (i = 0; i < pasture_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", pasture_table[i].key,
                pasture_table[i].name, pasture_table[i].hectares);
    }
    fprintf(out, "  total hectares %d\n", pasture_total_hectares());
}

int main(void)
{
    pasture_reset();
    pasture_load_seed();
    pasture_remove(103);
    pasture_report(stdout);

    return 0;
}
