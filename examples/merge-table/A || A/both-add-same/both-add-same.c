/*
 * A fixed size device table.
 *
 * It holds up to DEVICE_SLOTS devices, each with a key, a short name, and
 * a battery. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define DEVICE_SLOTS 64
#define DEVICE_NAME_SIZE 24

struct device {
    int key;
    char name[DEVICE_NAME_SIZE];
    int battery;
};

static const struct device device_seed[] = {
    { 101, "device-0001",  10 },
    { 102, "device-0002",  17 },
    { 103, "device-0003",  24 },
    { 104, "device-0004",  31 },
    { 105, "device-0005",  38 },
    { 106, "device-0006",  45 },
    { 107, "device-0007",  52 },
    { 108, "device-0008",  59 },
    { 109, "device-0009",  66 },
    { 110, "device-0010",  73 },
    { 111, "device-0011",  80 },
    { 112, "device-0012",  87 },
    { 113, "device-0013",  94 },
    { 114, "device-0014",   4 },
    { 115, "device-0015",  11 },
    { 116, "device-0016",  18 },
    { 117, "device-0017",  25 },
    { 118, "device-0018",  32 },
    { 119, "device-0019",  39 },
    { 120, "device-0020",  46 },
    { 121, "device-0021",  53 },
    { 122, "device-0022",  60 },
    { 123, "device-0023",  67 },
    { 124, "device-0024",  74 },
    { 125, "device-0025",  81 },
    { 126, "device-0026",  88 },
    { 127, "device-0027",  95 },
    { 128, "device-0028",   5 },
    { 129, "device-0029",  12 },
    { 130, "device-0030",  19 },
    { 131, "device-0031",  26 },
    { 132, "device-0032",  33 },
    { 133, "device-0033",  40 },
    { 134, "device-0034",  47 },
    { 135, "device-0035",  54 },
    { 136, "device-0036",  61 },
    { 137, "device-0037",  68 },
    { 138, "device-0038",  75 },
    { 139, "device-0039",  82 },
    { 140, "device-0040",  89 },
    { 141, "device-0041",  96 },
    { 142, "device-0042",   6 },
    { 143, "device-0043",  13 },
    { 144, "device-0044",  20 },
    { 145, "device-0045",  27 },
    { 146, "device-0046",  34 },
    { 147, "device-0047",  41 },
    { 148, "device-0048",  48 },
};

static struct device device_table[DEVICE_SLOTS];
static int device_used;

void device_reset(void)
{
    memset(device_table, 0, sizeof(device_table));
    device_used = 0;
}

int device_add(int key, const char *name, int battery)
{
    struct device *slot;

    if (device_used >= DEVICE_SLOTS) {
        return -1;
    }

    slot = &device_table[device_used];
    slot->key = key;
    slot->battery = battery;
    strncpy(slot->name, name, DEVICE_NAME_SIZE - 1);
    slot->name[DEVICE_NAME_SIZE - 1] = '\0';
    device_used++;

    return device_used - 1;
}

struct device *device_find(int key)
{
    int i;

    for (i = 0; i < device_used; i++) {
        if (device_table[i].key == key) {
            return &device_table[i];
        }
    }

    return NULL;
}

int device_remove(int key)
{
    struct device *found;

    found = device_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = device_table[device_used - 1];
    device_used--;

    return 0;
}

int device_total_battery(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < device_used; i++) {
        total += device_table[i].battery;
    }

    return total;
}

void device_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(device_seed) / sizeof(device_seed[0]); i++) {
        device_add(device_seed[i].key, device_seed[i].name,
                    device_seed[i].battery);
    }
}

void device_report(FILE *out)
{
    int i;

    fprintf(out, "%d devices\n", device_used);
    for (i = 0; i < device_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", device_table[i].key,
                device_table[i].name, device_table[i].battery);
    }
    fprintf(out, "  total battery %d\n", device_total_battery());
}

int main(void)
{
    device_reset();
    device_load_seed();
    device_remove(103);
    device_report(stdout);

    return 0;
}
