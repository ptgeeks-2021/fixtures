/*
 * A fixed size flight table.
 *
 * It holds up to FLIGHT_SLOTS flights, each with a key, a short name, and
 * a altitude. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define FLIGHT_SLOTS 64
#define FLIGHT_NAME_SIZE 24

struct flight {
    int key;
    char name[FLIGHT_NAME_SIZE];
    int altitude;
};

static const struct flight flight_seed[] = {
    { 101, "flight-0001",  10 },
    { 102, "flight-0002",  17 },
    { 103, "flight-0003",  24 },
    { 104, "flight-0004",  31 },
    { 105, "flight-0005",  38 },
    { 106, "flight-0006",  45 },
    { 107, "flight-0007",  52 },
    { 108, "flight-0008",  59 },
    { 109, "flight-0009",  66 },
    { 110, "flight-0010",  73 },
    { 111, "flight-0011",  80 },
    { 112, "flight-0012",  87 },
    { 113, "flight-0013",  94 },
    { 114, "flight-0014",   4 },
    { 115, "flight-0015",  11 },
    { 116, "flight-0016",  18 },
    { 117, "flight-0017",  25 },
    { 118, "flight-0018",  32 },
    { 119, "flight-0019",  39 },
    { 120, "flight-0020",  46 },
    { 121, "flight-0021",  53 },
    { 122, "flight-0022",  60 },
    { 123, "flight-0023",  67 },
    { 124, "flight-0024",  74 },
    { 125, "flight-0025",  81 },
    { 126, "flight-0026",  88 },
    { 127, "flight-0027",  95 },
    { 128, "flight-0028",   5 },
    { 129, "flight-0029",  12 },
    { 130, "flight-0030",  19 },
    { 131, "flight-0031",  26 },
    { 132, "flight-0032",  33 },
    { 133, "flight-0033",  40 },
    { 134, "flight-0034",  47 },
    { 135, "flight-0035",  54 },
    { 136, "flight-0036",  61 },
    { 137, "flight-0037",  68 },
    { 138, "flight-0038",  75 },
    { 139, "flight-0039",  82 },
    { 140, "flight-0040",  89 },
    { 141, "flight-0041",  96 },
    { 142, "flight-0042",   6 },
    { 143, "flight-0043",  13 },
    { 144, "flight-0044",  20 },
    { 145, "flight-0045",  27 },
    { 146, "flight-0046",  34 },
    { 147, "flight-0047",  41 },
    { 148, "flight-0048",  48 },
};

static struct flight flight_table[FLIGHT_SLOTS];
static int flight_used;

void flight_reset(void)
{
    memset(flight_table, 0, sizeof(flight_table));
    flight_used = 0;
}

int flight_add(int key, const char *name, int altitude)
{
    struct flight *slot;

    if (flight_used >= FLIGHT_SLOTS) {
        return -1;
    }

    slot = &flight_table[flight_used];
    slot->key = key;
    slot->altitude = altitude;
    strncpy(slot->name, name, FLIGHT_NAME_SIZE - 1);
    slot->name[FLIGHT_NAME_SIZE - 1] = '\0';
    flight_used++;

    return flight_used - 1;
}

struct flight *flight_find(int key)
{
    int i;

    for (i = 0; i < flight_used; i++) {
        if (flight_table[i].key == key) {
            return &flight_table[i];
        }
    }

    return NULL;
}

int flight_remove(int key)
{
    struct flight *found;

    found = flight_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = flight_table[flight_used - 1];
    flight_used--;

    return 0;
}

int flight_total_altitude(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < flight_used; i++) {
        total += flight_table[i].altitude;
    }

    return total;
}

void flight_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(flight_seed) / sizeof(flight_seed[0]); i++) {
        flight_add(flight_seed[i].key, flight_seed[i].name,
                    flight_seed[i].altitude);
    }
}

void flight_report(FILE *out)
{
    int i;

    fprintf(out, "%d flights\n", flight_used);
    for (i = 0; i < flight_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", flight_table[i].key,
                flight_table[i].name, flight_table[i].altitude);
    }
    fprintf(out, "  total altitude %d\n", flight_total_altitude());
}

int main(void)
{
    flight_reset();
    flight_load_seed();
    flight_remove(105);
    flight_report(stdout);

    return 0;
}
