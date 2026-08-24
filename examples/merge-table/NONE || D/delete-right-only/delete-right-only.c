/*
 * A fixed size sensor table.
 *
 * It holds up to SENSOR_SLOTS sensors, each with a key, a short name, and
 * a reading. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define SENSOR_SLOTS 64
#define SENSOR_NAME_SIZE 24

struct sensor {
    int key;
    char name[SENSOR_NAME_SIZE];
    int reading;
};

static const struct sensor sensor_seed[] = {
    { 101, "sensor-0001",  10 },
    { 102, "sensor-0002",  17 },
    { 103, "sensor-0003",  24 },
    { 104, "sensor-0004",  31 },
    { 105, "sensor-0005",  38 },
    { 106, "sensor-0006",  45 },
    { 107, "sensor-0007",  52 },
    { 108, "sensor-0008",  59 },
    { 109, "sensor-0009",  66 },
    { 110, "sensor-0010",  73 },
    { 111, "sensor-0011",  80 },
    { 112, "sensor-0012",  87 },
    { 113, "sensor-0013",  94 },
    { 114, "sensor-0014",   4 },
    { 115, "sensor-0015",  11 },
    { 116, "sensor-0016",  18 },
    { 117, "sensor-0017",  25 },
    { 118, "sensor-0018",  32 },
    { 119, "sensor-0019",  39 },
    { 120, "sensor-0020",  46 },
    { 121, "sensor-0021",  53 },
    { 122, "sensor-0022",  60 },
    { 123, "sensor-0023",  67 },
    { 124, "sensor-0024",  74 },
    { 125, "sensor-0025",  81 },
    { 126, "sensor-0026",  88 },
    { 127, "sensor-0027",  95 },
    { 128, "sensor-0028",   5 },
    { 129, "sensor-0029",  12 },
    { 130, "sensor-0030",  19 },
    { 131, "sensor-0031",  26 },
    { 132, "sensor-0032",  33 },
    { 133, "sensor-0033",  40 },
    { 134, "sensor-0034",  47 },
    { 135, "sensor-0035",  54 },
    { 136, "sensor-0036",  61 },
    { 137, "sensor-0037",  68 },
    { 138, "sensor-0038",  75 },
    { 139, "sensor-0039",  82 },
    { 140, "sensor-0040",  89 },
    { 141, "sensor-0041",  96 },
    { 142, "sensor-0042",   6 },
    { 143, "sensor-0043",  13 },
    { 144, "sensor-0044",  20 },
    { 145, "sensor-0045",  27 },
    { 146, "sensor-0046",  34 },
    { 147, "sensor-0047",  41 },
    { 148, "sensor-0048",  48 },
};

static struct sensor sensor_table[SENSOR_SLOTS];
static int sensor_used;

void sensor_reset(void)
{
    memset(sensor_table, 0, sizeof(sensor_table));
    sensor_used = 0;
}

int sensor_add(int key, const char *name, int reading)
{
    struct sensor *slot;

    if (sensor_used >= SENSOR_SLOTS) {
        return -1;
    }

    slot = &sensor_table[sensor_used];
    slot->key = key;
    slot->reading = reading;
    strncpy(slot->name, name, SENSOR_NAME_SIZE - 1);
    slot->name[SENSOR_NAME_SIZE - 1] = '\0';
    sensor_used++;

    return sensor_used - 1;
}

struct sensor *sensor_find(int key)
{
    int i;

    for (i = 0; i < sensor_used; i++) {
        if (sensor_table[i].key == key) {
            return &sensor_table[i];
        }
    }

    return NULL;
}

int sensor_remove(int key)
{
    struct sensor *found;

    found = sensor_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = sensor_table[sensor_used - 1];
    sensor_used--;

    return 0;
}

int sensor_total_reading(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < sensor_used; i++) {
        total += sensor_table[i].reading;
    }

    return total;
}

void sensor_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(sensor_seed) / sizeof(sensor_seed[0]); i++) {
        sensor_add(sensor_seed[i].key, sensor_seed[i].name,
                    sensor_seed[i].reading);
    }
}

void sensor_report(FILE *out)
{
    int i;

    fprintf(out, "%d sensors\n", sensor_used);
    for (i = 0; i < sensor_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", sensor_table[i].key,
                sensor_table[i].name, sensor_table[i].reading);
    }
    fprintf(out, "  total reading %d\n", sensor_total_reading());
}

int main(void)
{
    sensor_reset();
    sensor_load_seed();
    sensor_remove(103);
    sensor_report(stdout);

    return 0;
}
