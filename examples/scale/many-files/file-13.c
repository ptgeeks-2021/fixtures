/*
 * A fixed size vehicle table.
 *
 * It holds up to VEHICLE_SLOTS vehicles, each with a key, a short name, and
 * a mileage. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define VEHICLE_SLOTS 96
#define VEHICLE_NAME_SIZE 24

struct vehicle {
    int key;
    char name[VEHICLE_NAME_SIZE];
    int mileage;
};

static const struct vehicle vehicle_seed[] = {
    { 101, "vehicle-0001",  10 },
    { 102, "vehicle-0002",  17 },
    { 103, "vehicle-0003",  24 },
    { 104, "vehicle-0004",  31 },
    { 105, "vehicle-0005",  38 },
    { 106, "vehicle-0006",  45 },
    { 107, "vehicle-0007",  52 },
    { 108, "vehicle-0008",  59 },
    { 109, "vehicle-0009",  66 },
    { 110, "vehicle-0010",  73 },
    { 111, "vehicle-0011",  80 },
    { 112, "vehicle-0012",  87 },
    { 113, "vehicle-0013",  94 },
    { 114, "vehicle-0014",   4 },
    { 115, "vehicle-0015",  11 },
    { 116, "vehicle-0016",  18 },
    { 117, "vehicle-0017",  25 },
    { 118, "vehicle-0018",  32 },
    { 119, "vehicle-0019",  39 },
    { 120, "vehicle-0020",  46 },
    { 121, "vehicle-0021",  53 },
    { 122, "vehicle-0022",  60 },
    { 123, "vehicle-0023",  67 },
    { 124, "vehicle-0024",  74 },
    { 125, "vehicle-0025",  81 },
    { 126, "vehicle-0026",  88 },
    { 127, "vehicle-0027",  95 },
    { 128, "vehicle-0028",   5 },
    { 129, "vehicle-0029",  12 },
    { 130, "vehicle-0030",  19 },
    { 131, "vehicle-0031",  26 },
    { 132, "vehicle-0032",  33 },
    { 133, "vehicle-0033",  40 },
    { 134, "vehicle-0034",  47 },
    { 135, "vehicle-0035",  54 },
    { 136, "vehicle-0036",  61 },
    { 137, "vehicle-0037",  68 },
    { 138, "vehicle-0038",  75 },
    { 139, "vehicle-0039",  82 },
    { 140, "vehicle-0040",  89 },
    { 141, "vehicle-0041",  96 },
    { 142, "vehicle-0042",   6 },
    { 143, "vehicle-0043",  13 },
    { 144, "vehicle-0044",  20 },
    { 145, "vehicle-0045",  27 },
    { 146, "vehicle-0046",  34 },
    { 147, "vehicle-0047",  41 },
    { 148, "vehicle-0048",  48 },
};

static struct vehicle vehicle_table[VEHICLE_SLOTS];
static int vehicle_used;

void vehicle_reset(void)
{
    memset(vehicle_table, 0, sizeof(vehicle_table));
    vehicle_used = 0;
}

int vehicle_add(int key, const char *name, int mileage)
{
    struct vehicle *slot;

    if (vehicle_used >= VEHICLE_SLOTS) {
        return -1;
    }

    slot = &vehicle_table[vehicle_used];
    slot->key = key;
    slot->mileage = mileage;
    strncpy(slot->name, name, VEHICLE_NAME_SIZE - 1);
    slot->name[VEHICLE_NAME_SIZE - 1] = '\0';
    vehicle_used++;

    return vehicle_used - 1;
}

struct vehicle *vehicle_find(int key)
{
    int i;

    for (i = 0; i < vehicle_used; i++) {
        if (vehicle_table[i].key == key) {
            return &vehicle_table[i];
        }
    }

    return NULL;
}

int vehicle_remove(int key)
{
    struct vehicle *found;

    found = vehicle_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = vehicle_table[vehicle_used - 1];
    vehicle_used--;

    return 0;
}

int vehicle_total_mileage(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < vehicle_used; i++) {
        total += vehicle_table[i].mileage;
    }

    return total;
}

void vehicle_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(vehicle_seed) / sizeof(vehicle_seed[0]); i++) {
        vehicle_add(vehicle_seed[i].key, vehicle_seed[i].name,
                    vehicle_seed[i].mileage);
    }
}

void vehicle_report(FILE *out)
{
    int i;

    fprintf(out, "%d vehicles\n", vehicle_used);
    for (i = 0; i < vehicle_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", vehicle_table[i].key,
                vehicle_table[i].name, vehicle_table[i].mileage);
    }
    fprintf(out, "  total mileage %d\n", vehicle_total_mileage());
}

int main(void)
{
    vehicle_reset();
    vehicle_load_seed();
    vehicle_remove(103);
    vehicle_report(stdout);

    return 0;
}
