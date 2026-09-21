/*
 * A fixed size barometer table.
 *
 * It holds up to BAROMETER_SLOTS barometers, each with a key, a short name, and
 * a millibars. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define BAROMETER_SLOTS 64
#define BAROMETER_NAME_SIZE 24

struct barometer {
    int key;
    char name[BAROMETER_NAME_SIZE];
    int millibars;
};

static const struct barometer barometer_seed[] = {
    { 101, "barometer-0001",  10 },
    { 102, "barometer-0002",  17 },
    { 103, "barometer-0003",  24 },
    { 104, "barometer-0004",  31 },
    { 105, "barometer-0005",  38 },
    { 106, "barometer-0006",  45 },
    { 107, "barometer-0007",  52 },
    { 108, "barometer-0008",  59 },
    { 109, "barometer-0009",  66 },
    { 110, "barometer-0010",  73 },
    { 111, "barometer-0011",  80 },
    { 112, "barometer-0012",  87 },
    { 113, "barometer-0013",  94 },
    { 114, "barometer-0014",   4 },
    { 115, "barometer-0015",  11 },
    { 116, "barometer-0016",  18 },
    { 117, "barometer-0017",  25 },
    { 118, "barometer-0018",  32 },
    { 119, "barometer-0019",  39 },
    { 120, "barometer-0020",  46 },
    { 121, "barometer-0021",  53 },
    { 122, "barometer-0022",  60 },
    { 123, "barometer-0023",  67 },
    { 124, "barometer-0024",  74 },
    { 125, "barometer-0025",  81 },
    { 126, "barometer-0026",  88 },
    { 127, "barometer-0027",  95 },
    { 128, "barometer-0028",   5 },
    { 129, "barometer-0029",  12 },
    { 130, "barometer-0030",  19 },
    { 131, "barometer-0031",  26 },
    { 132, "barometer-0032",  33 },
    { 133, "barometer-0033",  40 },
    { 134, "barometer-0034",  47 },
    { 135, "barometer-0035",  54 },
    { 136, "barometer-0036",  61 },
    { 137, "barometer-0037",  68 },
    { 138, "barometer-0038",  75 },
    { 139, "barometer-0039",  82 },
    { 140, "barometer-0040",  89 },
    { 141, "barometer-0041",  96 },
    { 142, "barometer-0042",   6 },
    { 143, "barometer-0043",  13 },
    { 144, "barometer-0044",  20 },
    { 145, "barometer-0045",  27 },
    { 146, "barometer-0046",  34 },
    { 147, "barometer-0047",  41 },
    { 148, "barometer-0048",  48 },
};

static struct barometer barometer_table[BAROMETER_SLOTS];
static int barometer_used;

void barometer_reset(void)
{
    memset(barometer_table, 0, sizeof(barometer_table));
    barometer_used = 0;
}

int barometer_add(int key, const char *name, int millibars)
{
    struct barometer *slot;

    if (barometer_used >= BAROMETER_SLOTS) {
        return -1;
    }

    slot = &barometer_table[barometer_used];
    slot->key = key;
    slot->millibars = millibars;
    strncpy(slot->name, name, BAROMETER_NAME_SIZE - 1);
    slot->name[BAROMETER_NAME_SIZE - 1] = '\0';
    barometer_used++;

    return barometer_used - 1;
}

struct barometer *barometer_find(int key)
{
    int i;

    for (i = 0; i < barometer_used; i++) {
        if (barometer_table[i].key == key) {
            return &barometer_table[i];
        }
    }

    return NULL;
}

int barometer_remove(int key)
{
    struct barometer *found;

    found = barometer_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = barometer_table[barometer_used - 1];
    barometer_used--;

    return 0;
}

int barometer_total_millibars(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < barometer_used; i++) {
        total += barometer_table[i].millibars;
    }

    return total;
}

void barometer_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(barometer_seed) / sizeof(barometer_seed[0]); i++) {
        barometer_add(barometer_seed[i].key, barometer_seed[i].name,
                    barometer_seed[i].millibars);
    }
}

void barometer_report(FILE *out)
{
    int i;

    fprintf(out, "%d barometers\n", barometer_used);
    for (i = 0; i < barometer_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", barometer_table[i].key,
                barometer_table[i].name, barometer_table[i].millibars);
    }
    fprintf(out, "  total millibars %d\n", barometer_total_millibars());
}

int main(void)
{
    barometer_reset();
    barometer_load_seed();
    barometer_remove(103);
    barometer_report(stdout);

    return 0;
}
