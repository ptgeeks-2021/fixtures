/*
 * A fixed size tanyard table.
 *
 * It holds up to TANYARD_SLOTS tanyards, each with a key, a short name, and
 * a hides. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define TANYARD_SLOTS 64
#define TANYARD_NAME_SIZE 24

struct tanyard {
    int key;
    char name[TANYARD_NAME_SIZE];
    int hides;
};

static const struct tanyard tanyard_seed[] = {
    { 101, "tanyard-0001",  10 },
    { 102, "tanyard-0002",  17 },
    { 103, "tanyard-0003",  24 },
    { 104, "tanyard-0004",  31 },
    { 105, "tanyard-0005",  38 },
    { 106, "tanyard-0006",  45 },
    { 107, "tanyard-0007",  52 },
    { 108, "tanyard-0008",  59 },
    { 109, "tanyard-0009",  66 },
    { 110, "tanyard-0010",  73 },
    { 111, "tanyard-0011",  80 },
    { 112, "tanyard-0012",  87 },
    { 113, "tanyard-0013",  94 },
    { 114, "tanyard-0014",   4 },
    { 115, "tanyard-0015",  11 },
    { 116, "tanyard-0016",  18 },
    { 117, "tanyard-0017",  25 },
    { 118, "tanyard-0018",  32 },
    { 119, "tanyard-0019",  39 },
    { 120, "tanyard-0020",  46 },
    { 121, "tanyard-0021",  53 },
    { 122, "tanyard-0022",  60 },
    { 123, "tanyard-0023",  67 },
    { 124, "tanyard-0024",  74 },
    { 125, "tanyard-0025",  81 },
    { 126, "tanyard-0026",  88 },
    { 127, "tanyard-0027",  95 },
    { 128, "tanyard-0028",   5 },
    { 129, "tanyard-0029",  12 },
    { 130, "tanyard-0030",  19 },
    { 131, "tanyard-0031",  26 },
    { 132, "tanyard-0032",  33 },
    { 133, "tanyard-0033",  40 },
    { 134, "tanyard-0034",  47 },
    { 135, "tanyard-0035",  54 },
    { 136, "tanyard-0036",  61 },
    { 137, "tanyard-0037",  68 },
    { 138, "tanyard-0038",  75 },
    { 139, "tanyard-0039",  82 },
    { 140, "tanyard-0040",  89 },
    { 141, "tanyard-0041",  96 },
    { 142, "tanyard-0042",   6 },
    { 143, "tanyard-0043",  13 },
    { 144, "tanyard-0044",  20 },
    { 145, "tanyard-0045",  27 },
    { 146, "tanyard-0046",  34 },
    { 147, "tanyard-0047",  41 },
    { 148, "tanyard-0048",  48 },
};

static struct tanyard tanyard_table[TANYARD_SLOTS];
static int tanyard_used;

void tanyard_reset(void)
{
    memset(tanyard_table, 0, sizeof(tanyard_table));
    tanyard_used = 0;
}

int tanyard_add(int key, const char *name, int hides)
{
    struct tanyard *slot;

    if (tanyard_used >= TANYARD_SLOTS) {
        return -1;
    }

    slot = &tanyard_table[tanyard_used];
    slot->key = key;
    slot->hides = hides;
    strncpy(slot->name, name, TANYARD_NAME_SIZE - 1);
    slot->name[TANYARD_NAME_SIZE - 1] = '\0';
    tanyard_used++;

    return tanyard_used - 1;
}

struct tanyard *tanyard_find(int key)
{
    int i;

    for (i = 0; i < tanyard_used; i++) {
        if (tanyard_table[i].key == key) {
            return &tanyard_table[i];
        }
    }

    return NULL;
}

int tanyard_remove(int key)
{
    struct tanyard *found;

    found = tanyard_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = tanyard_table[tanyard_used - 1];
    tanyard_used--;

    return 0;
}

int tanyard_total_hides(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < tanyard_used; i++) {
        total += tanyard_table[i].hides;
    }

    return total;
}

void tanyard_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(tanyard_seed) / sizeof(tanyard_seed[0]); i++) {
        tanyard_add(tanyard_seed[i].key, tanyard_seed[i].name,
                    tanyard_seed[i].hides);
    }
}

void tanyard_report(FILE *out)
{
    int i;

    fprintf(out, "%d tanyards\n", tanyard_used);
    for (i = 0; i < tanyard_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", tanyard_table[i].key,
                tanyard_table[i].name, tanyard_table[i].hides);
    }
    fprintf(out, "  total hides %d\n", tanyard_total_hides());
}

int main(void)
{
    tanyard_reset();
    tanyard_load_seed();
    tanyard_remove(103);
    tanyard_report(stdout);

    return 0;
}
