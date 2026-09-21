/*
 * A fixed size switchboard table.
 *
 * It holds up to SWITCHBOARD_SLOTS switchboards, each with a key, a short name, and
 * a circuits. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define SWITCHBOARD_SLOTS 64
#define SWITCHBOARD_NAME_SIZE 24

struct switchboard {
    int key;
    char name[SWITCHBOARD_NAME_SIZE];
    int circuits;
};

static const struct switchboard switchboard_seed[] = {
    { 101, "switchboard-0001",  10 },
    { 102, "switchboard-0002",  17 },
    { 103, "switchboard-0003",  24 },
    { 104, "switchboard-0004",  31 },
    { 105, "switchboard-0005",  38 },
    { 106, "switchboard-0006",  45 },
    { 107, "switchboard-0007",  52 },
    { 108, "switchboard-0008",  59 },
    { 109, "switchboard-0009",  66 },
    { 110, "switchboard-0010",  73 },
    { 111, "switchboard-0011",  80 },
    { 112, "switchboard-0012",  87 },
    { 113, "switchboard-0013",  94 },
    { 114, "switchboard-0014",   4 },
    { 115, "switchboard-0015",  11 },
    { 116, "switchboard-0016",  18 },
    { 117, "switchboard-0017",  25 },
    { 118, "switchboard-0018",  32 },
    { 119, "switchboard-0019",  39 },
    { 120, "switchboard-0020",  46 },
    { 121, "switchboard-0021",  53 },
    { 122, "switchboard-0022",  60 },
    { 123, "switchboard-0023",  67 },
    { 124, "switchboard-0024",  74 },
    { 125, "switchboard-0025",  81 },
    { 126, "switchboard-0026",  88 },
    { 127, "switchboard-0027",  95 },
    { 128, "switchboard-0028",   5 },
    { 129, "switchboard-0029",  12 },
    { 130, "switchboard-0030",  19 },
    { 131, "switchboard-0031",  26 },
    { 132, "switchboard-0032",  33 },
    { 133, "switchboard-0033",  40 },
    { 134, "switchboard-0034",  47 },
    { 135, "switchboard-0035",  54 },
    { 136, "switchboard-0036",  61 },
    { 137, "switchboard-0037",  68 },
    { 138, "switchboard-0038",  75 },
    { 139, "switchboard-0039",  82 },
    { 140, "switchboard-0040",  89 },
    { 141, "switchboard-0041",  96 },
    { 142, "switchboard-0042",   6 },
    { 143, "switchboard-0043",  13 },
    { 144, "switchboard-0044",  20 },
    { 145, "switchboard-0045",  27 },
    { 146, "switchboard-0046",  34 },
    { 147, "switchboard-0047",  41 },
    { 148, "switchboard-0048",  48 },
};

static struct switchboard switchboard_table[SWITCHBOARD_SLOTS];
static int switchboard_used;

void switchboard_reset(void)
{
    memset(switchboard_table, 0, sizeof(switchboard_table));
    switchboard_used = 0;
}

int switchboard_add(int key, const char *name, int circuits)
{
    struct switchboard *slot;

    if (switchboard_used >= SWITCHBOARD_SLOTS) {
        return -1;
    }

    slot = &switchboard_table[switchboard_used];
    slot->key = key;
    slot->circuits = circuits;
    strncpy(slot->name, name, SWITCHBOARD_NAME_SIZE - 1);
    slot->name[SWITCHBOARD_NAME_SIZE - 1] = '\0';
    switchboard_used++;

    return switchboard_used - 1;
}

struct switchboard *switchboard_find(int key)
{
    int i;

    for (i = 0; i < switchboard_used; i++) {
        if (switchboard_table[i].key == key) {
            return &switchboard_table[i];
        }
    }

    return NULL;
}

int switchboard_remove(int key)
{
    struct switchboard *found;

    found = switchboard_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = switchboard_table[switchboard_used - 1];
    switchboard_used--;

    return 0;
}

int switchboard_total_circuits(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < switchboard_used; i++) {
        total += switchboard_table[i].circuits;
    }

    return total;
}

void switchboard_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(switchboard_seed) / sizeof(switchboard_seed[0]); i++) {
        switchboard_add(switchboard_seed[i].key, switchboard_seed[i].name,
                    switchboard_seed[i].circuits);
    }
}

void switchboard_report(FILE *out)
{
    int i;

    fprintf(out, "%d switchboards\n", switchboard_used);
    for (i = 0; i < switchboard_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", switchboard_table[i].key,
                switchboard_table[i].name, switchboard_table[i].circuits);
    }
    fprintf(out, "  total circuits %d\n", switchboard_total_circuits());
}

int main(void)
{
    switchboard_reset();
    switchboard_load_seed();
    switchboard_remove(103);
    switchboard_report(stdout);

    return 0;
}
