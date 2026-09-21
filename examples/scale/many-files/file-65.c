/*
 * A fixed size greenhouse table.
 *
 * It holds up to GREENHOUSE_SLOTS greenhouses, each with a key, a short name, and
 * a beds. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define GREENHOUSE_SLOTS 96
#define GREENHOUSE_NAME_SIZE 24

struct greenhouse {
    int key;
    char name[GREENHOUSE_NAME_SIZE];
    int beds;
};

static const struct greenhouse greenhouse_seed[] = {
    { 101, "greenhouse-0001",  10 },
    { 102, "greenhouse-0002",  17 },
    { 103, "greenhouse-0003",  24 },
    { 104, "greenhouse-0004",  31 },
    { 105, "greenhouse-0005",  38 },
    { 106, "greenhouse-0006",  45 },
    { 107, "greenhouse-0007",  52 },
    { 108, "greenhouse-0008",  59 },
    { 109, "greenhouse-0009",  66 },
    { 110, "greenhouse-0010",  73 },
    { 111, "greenhouse-0011",  80 },
    { 112, "greenhouse-0012",  87 },
    { 113, "greenhouse-0013",  94 },
    { 114, "greenhouse-0014",   4 },
    { 115, "greenhouse-0015",  11 },
    { 116, "greenhouse-0016",  18 },
    { 117, "greenhouse-0017",  25 },
    { 118, "greenhouse-0018",  32 },
    { 119, "greenhouse-0019",  39 },
    { 120, "greenhouse-0020",  46 },
    { 121, "greenhouse-0021",  53 },
    { 122, "greenhouse-0022",  60 },
    { 123, "greenhouse-0023",  67 },
    { 124, "greenhouse-0024",  74 },
    { 125, "greenhouse-0025",  81 },
    { 126, "greenhouse-0026",  88 },
    { 127, "greenhouse-0027",  95 },
    { 128, "greenhouse-0028",   5 },
    { 129, "greenhouse-0029",  12 },
    { 130, "greenhouse-0030",  19 },
    { 131, "greenhouse-0031",  26 },
    { 132, "greenhouse-0032",  33 },
    { 133, "greenhouse-0033",  40 },
    { 134, "greenhouse-0034",  47 },
    { 135, "greenhouse-0035",  54 },
    { 136, "greenhouse-0036",  61 },
    { 137, "greenhouse-0037",  68 },
    { 138, "greenhouse-0038",  75 },
    { 139, "greenhouse-0039",  82 },
    { 140, "greenhouse-0040",  89 },
    { 141, "greenhouse-0041",  96 },
    { 142, "greenhouse-0042",   6 },
    { 143, "greenhouse-0043",  13 },
    { 144, "greenhouse-0044",  20 },
    { 145, "greenhouse-0045",  27 },
    { 146, "greenhouse-0046",  34 },
    { 147, "greenhouse-0047",  41 },
    { 148, "greenhouse-0048",  48 },
};

static struct greenhouse greenhouse_table[GREENHOUSE_SLOTS];
static int greenhouse_used;

void greenhouse_reset(void)
{
    memset(greenhouse_table, 0, sizeof(greenhouse_table));
    greenhouse_used = 0;
}

int greenhouse_add(int key, const char *name, int beds)
{
    struct greenhouse *slot;

    if (greenhouse_used >= GREENHOUSE_SLOTS) {
        return -1;
    }

    slot = &greenhouse_table[greenhouse_used];
    slot->key = key;
    slot->beds = beds;
    strncpy(slot->name, name, GREENHOUSE_NAME_SIZE - 1);
    slot->name[GREENHOUSE_NAME_SIZE - 1] = '\0';
    greenhouse_used++;

    return greenhouse_used - 1;
}

struct greenhouse *greenhouse_find(int key)
{
    int i;

    for (i = 0; i < greenhouse_used; i++) {
        if (greenhouse_table[i].key == key) {
            return &greenhouse_table[i];
        }
    }

    return NULL;
}

int greenhouse_remove(int key)
{
    struct greenhouse *found;

    found = greenhouse_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = greenhouse_table[greenhouse_used - 1];
    greenhouse_used--;

    return 0;
}

int greenhouse_total_beds(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < greenhouse_used; i++) {
        total += greenhouse_table[i].beds;
    }

    return total;
}

void greenhouse_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(greenhouse_seed) / sizeof(greenhouse_seed[0]); i++) {
        greenhouse_add(greenhouse_seed[i].key, greenhouse_seed[i].name,
                    greenhouse_seed[i].beds);
    }
}

void greenhouse_report(FILE *out)
{
    int i;

    fprintf(out, "%d greenhouses\n", greenhouse_used);
    for (i = 0; i < greenhouse_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", greenhouse_table[i].key,
                greenhouse_table[i].name, greenhouse_table[i].beds);
    }
    fprintf(out, "  total beds %d\n", greenhouse_total_beds());
}

int main(void)
{
    greenhouse_reset();
    greenhouse_load_seed();
    greenhouse_remove(103);
    greenhouse_report(stdout);

    return 0;
}
