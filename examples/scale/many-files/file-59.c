/*
 * A fixed size seismograph table.
 *
 * It holds up to SEISMOGRAPH_SLOTS seismographs, each with a key, a short name, and
 * a tremors. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define SEISMOGRAPH_SLOTS 96
#define SEISMOGRAPH_NAME_SIZE 24

struct seismograph {
    int key;
    char name[SEISMOGRAPH_NAME_SIZE];
    int tremors;
};

static const struct seismograph seismograph_seed[] = {
    { 101, "seismograph-0001",  10 },
    { 102, "seismograph-0002",  17 },
    { 103, "seismograph-0003",  24 },
    { 104, "seismograph-0004",  31 },
    { 105, "seismograph-0005",  38 },
    { 106, "seismograph-0006",  45 },
    { 107, "seismograph-0007",  52 },
    { 108, "seismograph-0008",  59 },
    { 109, "seismograph-0009",  66 },
    { 110, "seismograph-0010",  73 },
    { 111, "seismograph-0011",  80 },
    { 112, "seismograph-0012",  87 },
    { 113, "seismograph-0013",  94 },
    { 114, "seismograph-0014",   4 },
    { 115, "seismograph-0015",  11 },
    { 116, "seismograph-0016",  18 },
    { 117, "seismograph-0017",  25 },
    { 118, "seismograph-0018",  32 },
    { 119, "seismograph-0019",  39 },
    { 120, "seismograph-0020",  46 },
    { 121, "seismograph-0021",  53 },
    { 122, "seismograph-0022",  60 },
    { 123, "seismograph-0023",  67 },
    { 124, "seismograph-0024",  74 },
    { 125, "seismograph-0025",  81 },
    { 126, "seismograph-0026",  88 },
    { 127, "seismograph-0027",  95 },
    { 128, "seismograph-0028",   5 },
    { 129, "seismograph-0029",  12 },
    { 130, "seismograph-0030",  19 },
    { 131, "seismograph-0031",  26 },
    { 132, "seismograph-0032",  33 },
    { 133, "seismograph-0033",  40 },
    { 134, "seismograph-0034",  47 },
    { 135, "seismograph-0035",  54 },
    { 136, "seismograph-0036",  61 },
    { 137, "seismograph-0037",  68 },
    { 138, "seismograph-0038",  75 },
    { 139, "seismograph-0039",  82 },
    { 140, "seismograph-0040",  89 },
    { 141, "seismograph-0041",  96 },
    { 142, "seismograph-0042",   6 },
    { 143, "seismograph-0043",  13 },
    { 144, "seismograph-0044",  20 },
    { 145, "seismograph-0045",  27 },
    { 146, "seismograph-0046",  34 },
    { 147, "seismograph-0047",  41 },
    { 148, "seismograph-0048",  48 },
};

static struct seismograph seismograph_table[SEISMOGRAPH_SLOTS];
static int seismograph_used;

void seismograph_reset(void)
{
    memset(seismograph_table, 0, sizeof(seismograph_table));
    seismograph_used = 0;
}

int seismograph_add(int key, const char *name, int tremors)
{
    struct seismograph *slot;

    if (seismograph_used >= SEISMOGRAPH_SLOTS) {
        return -1;
    }

    slot = &seismograph_table[seismograph_used];
    slot->key = key;
    slot->tremors = tremors;
    strncpy(slot->name, name, SEISMOGRAPH_NAME_SIZE - 1);
    slot->name[SEISMOGRAPH_NAME_SIZE - 1] = '\0';
    seismograph_used++;

    return seismograph_used - 1;
}

struct seismograph *seismograph_find(int key)
{
    int i;

    for (i = 0; i < seismograph_used; i++) {
        if (seismograph_table[i].key == key) {
            return &seismograph_table[i];
        }
    }

    return NULL;
}

int seismograph_remove(int key)
{
    struct seismograph *found;

    found = seismograph_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = seismograph_table[seismograph_used - 1];
    seismograph_used--;

    return 0;
}

int seismograph_total_tremors(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < seismograph_used; i++) {
        total += seismograph_table[i].tremors;
    }

    return total;
}

void seismograph_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(seismograph_seed) / sizeof(seismograph_seed[0]); i++) {
        seismograph_add(seismograph_seed[i].key, seismograph_seed[i].name,
                    seismograph_seed[i].tremors);
    }
}

void seismograph_report(FILE *out)
{
    int i;

    fprintf(out, "%d seismographs\n", seismograph_used);
    for (i = 0; i < seismograph_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", seismograph_table[i].key,
                seismograph_table[i].name, seismograph_table[i].tremors);
    }
    fprintf(out, "  total tremors %d\n", seismograph_total_tremors());
}

int main(void)
{
    seismograph_reset();
    seismograph_load_seed();
    seismograph_remove(103);
    seismograph_report(stdout);

    return 0;
}
