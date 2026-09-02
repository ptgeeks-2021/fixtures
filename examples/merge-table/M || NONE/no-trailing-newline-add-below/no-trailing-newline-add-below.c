/*
 * A fixed size courier table.
 *
 * It holds up to COURIER_SLOTS couriers, each with a key, a short name, and
 * a stops. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define COURIER_SLOTS 64
#define COURIER_NAME_SIZE 24

struct courier {
    int key;
    char name[COURIER_NAME_SIZE];
    int stops;
};

static const struct courier courier_seed[] = {
    { 101, "courier-0001",  10 },
    { 102, "courier-0002",  17 },
    { 103, "courier-0003",  24 },
    { 104, "courier-0004",  31 },
    { 105, "courier-0005",  38 },
    { 106, "courier-0006",  45 },
    { 107, "courier-0007",  52 },
    { 108, "courier-0008",  59 },
    { 109, "courier-0009",  66 },
    { 110, "courier-0010",  73 },
    { 111, "courier-0011",  80 },
    { 112, "courier-0012",  87 },
    { 113, "courier-0013",  94 },
    { 114, "courier-0014",   4 },
    { 115, "courier-0015",  11 },
    { 116, "courier-0016",  18 },
    { 117, "courier-0017",  25 },
    { 118, "courier-0018",  32 },
    { 119, "courier-0019",  39 },
    { 120, "courier-0020",  46 },
    { 121, "courier-0021",  53 },
    { 122, "courier-0022",  60 },
    { 123, "courier-0023",  67 },
    { 124, "courier-0024",  74 },
    { 125, "courier-0025",  81 },
    { 126, "courier-0026",  88 },
    { 127, "courier-0027",  95 },
    { 128, "courier-0028",   5 },
    { 129, "courier-0029",  12 },
    { 130, "courier-0030",  19 },
    { 131, "courier-0031",  26 },
    { 132, "courier-0032",  33 },
    { 133, "courier-0033",  40 },
    { 134, "courier-0034",  47 },
    { 135, "courier-0035",  54 },
    { 136, "courier-0036",  61 },
    { 137, "courier-0037",  68 },
    { 138, "courier-0038",  75 },
    { 139, "courier-0039",  82 },
    { 140, "courier-0040",  89 },
    { 141, "courier-0041",  96 },
    { 142, "courier-0042",   6 },
    { 143, "courier-0043",  13 },
    { 144, "courier-0044",  20 },
    { 145, "courier-0045",  27 },
    { 146, "courier-0046",  34 },
    { 147, "courier-0047",  41 },
    { 148, "courier-0048",  48 },
};

static struct courier courier_table[COURIER_SLOTS];
static int courier_used;

void courier_reset(void)
{
    memset(courier_table, 0, sizeof(courier_table));
    courier_used = 0;
}

int courier_add(int key, const char *name, int stops)
{
    struct courier *slot;

    if (courier_used >= COURIER_SLOTS) {
        return -1;
    }

    slot = &courier_table[courier_used];
    slot->key = key;
    slot->stops = stops;
    strncpy(slot->name, name, COURIER_NAME_SIZE - 1);
    slot->name[COURIER_NAME_SIZE - 1] = '\0';
    courier_used++;

    return courier_used - 1;
}

struct courier *courier_find(int key)
{
    int i;

    for (i = 0; i < courier_used; i++) {
        if (courier_table[i].key == key) {
            return &courier_table[i];
        }
    }

    return NULL;
}

int courier_remove(int key)
{
    struct courier *found;

    found = courier_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = courier_table[courier_used - 1];
    courier_used--;

    return 0;
}

int courier_total_stops(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < courier_used; i++) {
        total += courier_table[i].stops;
    }

    return total;
}

void courier_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(courier_seed) / sizeof(courier_seed[0]); i++) {
        courier_add(courier_seed[i].key, courier_seed[i].name,
                    courier_seed[i].stops);
    }
}

void courier_report(FILE *out)
{
    int i;

    fprintf(out, "%d couriers\n", courier_used);
    for (i = 0; i < courier_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", courier_table[i].key,
                courier_table[i].name, courier_table[i].stops);
    }
    fprintf(out, "  total stops %d\n", courier_total_stops());
}

int main(void)
{
    courier_reset();
    courier_load_seed();
    courier_remove(103);
    courier_report(stdout);

    return 0;
}
/* added below */