/*
 * A fixed size turbine table.
 *
 * It holds up to TURBINE_SLOTS turbines, each with a key, a short name, and
 * a rpm. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define TURBINE_SLOTS 64
#define TURBINE_NAME_SIZE 24

struct turbine {
    int key;
    char name[TURBINE_NAME_SIZE];
    int rpm;
};

static const struct turbine turbine_seed[] = {
    { 101, "turbine-0001",  10 },
    { 102, "turbine-0002",  17 },
    { 103, "turbine-0003",  24 },
    { 104, "turbine-0004",  31 },
    { 105, "turbine-0005",  38 },
    { 106, "turbine-0006",  45 },
    { 107, "turbine-0007",  52 },
    { 108, "turbine-0008",  59 },
    { 109, "turbine-0009",  66 },
    { 110, "turbine-0010",  73 },
    { 111, "turbine-0011",  80 },
    { 112, "turbine-0012",  87 },
    { 113, "turbine-0013",  94 },
    { 114, "turbine-0014",   4 },
    { 115, "turbine-0015",  11 },
    { 116, "turbine-0016",  18 },
    { 117, "turbine-0017",  25 },
    { 118, "turbine-0018",  32 },
    { 119, "turbine-0019",  39 },
    { 120, "turbine-0020",  46 },
    { 121, "turbine-0021",  53 },
    { 122, "turbine-0022",  60 },
    { 123, "turbine-0023",  67 },
    { 124, "turbine-0024",  74 },
    { 125, "turbine-0025",  81 },
    { 126, "turbine-0026",  88 },
    { 127, "turbine-0027",  95 },
    { 128, "turbine-0028",   5 },
    { 129, "turbine-0029",  12 },
    { 130, "turbine-0030",  19 },
    { 131, "turbine-0031",  26 },
    { 132, "turbine-0032",  33 },
    { 133, "turbine-0033",  40 },
    { 134, "turbine-0034",  47 },
    { 135, "turbine-0035",  54 },
    { 136, "turbine-0036",  61 },
    { 137, "turbine-0037",  68 },
    { 138, "turbine-0038",  75 },
    { 139, "turbine-0039",  82 },
    { 140, "turbine-0040",  89 },
    { 141, "turbine-0041",  96 },
    { 142, "turbine-0042",   6 },
    { 143, "turbine-0043",  13 },
    { 144, "turbine-0044",  20 },
    { 145, "turbine-0045",  27 },
    { 146, "turbine-0046",  34 },
    { 147, "turbine-0047",  41 },
    { 148, "turbine-0048",  48 },
};

static struct turbine turbine_table[TURBINE_SLOTS];
static int turbine_used;

void turbine_reset(void)
{
    memset(turbine_table, 0, sizeof(turbine_table));
    turbine_used = 0;
}

int turbine_add(int key, const char *name, int rpm)
{
    struct turbine *slot;

    if (turbine_used >= TURBINE_SLOTS) {
        return -1;
    }

    slot = &turbine_table[turbine_used];
    slot->key = key;
    slot->rpm = rpm;
    strncpy(slot->name, name, TURBINE_NAME_SIZE - 1);
    slot->name[TURBINE_NAME_SIZE - 1] = '\0';
    turbine_used++;

    return turbine_used - 1;
}

struct turbine *turbine_find(int key)
{
    int i;

    for (i = 0; i < turbine_used; i++) {
        if (turbine_table[i].key == key) {
            return &turbine_table[i];
        }
    }

    return NULL;
}

int turbine_remove(int key)
{
    struct turbine *found;

    found = turbine_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = turbine_table[turbine_used - 1];
    turbine_used--;

    return 0;
}

int turbine_total_rpm(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < turbine_used; i++) {
        total += turbine_table[i].rpm;
    }

    return total;
}

void turbine_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(turbine_seed) / sizeof(turbine_seed[0]); i++) {
        turbine_add(turbine_seed[i].key, turbine_seed[i].name,
                    turbine_seed[i].rpm);
    }
}

void turbine_report(FILE *out)
{
    int i;

    fprintf(out, "%d turbines\n", turbine_used);
    for (i = 0; i < turbine_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", turbine_table[i].key,
                turbine_table[i].name, turbine_table[i].rpm);
    }
    fprintf(out, "  total rpm %d\n", turbine_total_rpm());
}

int main(void)
{
    turbine_reset();
    turbine_load_seed();
    turbine_remove(103);
    turbine_report(stdout);

    return 0;
}
