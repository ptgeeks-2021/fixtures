/*
 * A fixed size beacon table.
 *
 * It holds up to BEACON_SLOTS beacons, each with a key, a short name, and
 * a pulses. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define BEACON_SLOTS 64
#define BEACON_NAME_SIZE 24

struct beacon {
    int key;
    char name[BEACON_NAME_SIZE];
    int pulses;
};

static const struct beacon beacon_seed[] = {
    { 101, "beacon-0001",  10 },
    { 102, "beacon-0002",  17 },
    { 103, "beacon-0003",  24 },
    { 104, "beacon-0004",  31 },
    { 105, "beacon-0005",  38 },
    { 106, "beacon-0006",  45 },
    { 107, "beacon-0007",  52 },
    { 108, "beacon-0008",  59 },
    { 109, "beacon-0009",  66 },
    { 110, "beacon-0010",  73 },
    { 111, "beacon-0011",  80 },
    { 112, "beacon-0012",  87 },
    { 113, "beacon-0013",  94 },
    { 114, "beacon-0014",   4 },
    { 115, "beacon-0015",  11 },
    { 116, "beacon-0016",  18 },
    { 117, "beacon-0017",  25 },
    { 118, "beacon-0018",  32 },
    { 119, "beacon-0019",  39 },
    { 120, "beacon-0020",  46 },
    { 121, "beacon-0021",  53 },
    { 122, "beacon-0022",  60 },
    { 123, "beacon-0023",  67 },
    { 124, "beacon-0024",  74 },
    { 125, "beacon-0025",  81 },
    { 126, "beacon-0026",  88 },
    { 127, "beacon-0027",  95 },
    { 128, "beacon-0028",   5 },
    { 129, "beacon-0029",  12 },
    { 130, "beacon-0030",  19 },
    { 131, "beacon-0031",  26 },
    { 132, "beacon-0032",  33 },
    { 133, "beacon-0033",  40 },
    { 134, "beacon-0034",  47 },
    { 135, "beacon-0035",  54 },
    { 136, "beacon-0036",  61 },
    { 137, "beacon-0037",  68 },
    { 138, "beacon-0038",  75 },
    { 139, "beacon-0039",  82 },
    { 140, "beacon-0040",  89 },
    { 141, "beacon-0041",  96 },
    { 142, "beacon-0042",   6 },
    { 143, "beacon-0043",  13 },
    { 144, "beacon-0044",  20 },
    { 145, "beacon-0045",  27 },
    { 146, "beacon-0046",  34 },
    { 147, "beacon-0047",  41 },
    { 148, "beacon-0048",  48 },
};

static struct beacon beacon_table[BEACON_SLOTS];
static int beacon_used;

void beacon_reset(void)
{
    memset(beacon_table, 0, sizeof(beacon_table));
    beacon_used = 0;
}

int beacon_add(int key, const char *name, int pulses)
{
    struct beacon *slot;

    if (beacon_used >= BEACON_SLOTS) {
        return -1;
    }

    slot = &beacon_table[beacon_used];
    slot->key = key;
    slot->pulses = pulses;
    strncpy(slot->name, name, BEACON_NAME_SIZE - 1);
    slot->name[BEACON_NAME_SIZE - 1] = '\0';
    beacon_used++;

    return beacon_used - 1;
}

struct beacon *beacon_find(int key)
{
    int i;

    for (i = 0; i < beacon_used; i++) {
        if (beacon_table[i].key == key) {
            return &beacon_table[i];
        }
    }

    return NULL;
}

int beacon_remove(int key)
{
    struct beacon *found;

    found = beacon_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = beacon_table[beacon_used - 1];
    beacon_used--;

    return 0;
}

int beacon_total_pulses(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < beacon_used; i++) {
        total += beacon_table[i].pulses;
    }

    return total;
}

void beacon_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(beacon_seed) / sizeof(beacon_seed[0]); i++) {
        beacon_add(beacon_seed[i].key, beacon_seed[i].name,
                    beacon_seed[i].pulses);
    }
}

void beacon_report(FILE *out)
{
    int i;

    fprintf(out, "%d beacons\n", beacon_used);
    for (i = 0; i < beacon_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", beacon_table[i].key,
                beacon_table[i].name, beacon_table[i].pulses);
    }
    fprintf(out, "  total pulses %d\n", beacon_total_pulses());
}

int main(void)
{
    beacon_reset();
    beacon_load_seed();
    beacon_remove(103);
    beacon_report(stdout);

    return 0;
}
