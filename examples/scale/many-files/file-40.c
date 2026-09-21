/*
 * A fixed size harbour table.
 *
 * It holds up to HARBOUR_SLOTS harbours, each with a key, a short name, and
 * a berths. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define HARBOUR_SLOTS 96
#define HARBOUR_NAME_SIZE 24

struct harbour {
    int key;
    char name[HARBOUR_NAME_SIZE];
    int berths;
};

static const struct harbour harbour_seed[] = {
    { 101, "harbour-0001",  10 },
    { 102, "harbour-0002",  17 },
    { 103, "harbour-0003",  24 },
    { 104, "harbour-0004",  31 },
    { 105, "harbour-0005",  38 },
    { 106, "harbour-0006",  45 },
    { 107, "harbour-0007",  52 },
    { 108, "harbour-0008",  59 },
    { 109, "harbour-0009",  66 },
    { 110, "harbour-0010",  73 },
    { 111, "harbour-0011",  80 },
    { 112, "harbour-0012",  87 },
    { 113, "harbour-0013",  94 },
    { 114, "harbour-0014",   4 },
    { 115, "harbour-0015",  11 },
    { 116, "harbour-0016",  18 },
    { 117, "harbour-0017",  25 },
    { 118, "harbour-0018",  32 },
    { 119, "harbour-0019",  39 },
    { 120, "harbour-0020",  46 },
    { 121, "harbour-0021",  53 },
    { 122, "harbour-0022",  60 },
    { 123, "harbour-0023",  67 },
    { 124, "harbour-0024",  74 },
    { 125, "harbour-0025",  81 },
    { 126, "harbour-0026",  88 },
    { 127, "harbour-0027",  95 },
    { 128, "harbour-0028",   5 },
    { 129, "harbour-0029",  12 },
    { 130, "harbour-0030",  19 },
    { 131, "harbour-0031",  26 },
    { 132, "harbour-0032",  33 },
    { 133, "harbour-0033",  40 },
    { 134, "harbour-0034",  47 },
    { 135, "harbour-0035",  54 },
    { 136, "harbour-0036",  61 },
    { 137, "harbour-0037",  68 },
    { 138, "harbour-0038",  75 },
    { 139, "harbour-0039",  82 },
    { 140, "harbour-0040",  89 },
    { 141, "harbour-0041",  96 },
    { 142, "harbour-0042",   6 },
    { 143, "harbour-0043",  13 },
    { 144, "harbour-0044",  20 },
    { 145, "harbour-0045",  27 },
    { 146, "harbour-0046",  34 },
    { 147, "harbour-0047",  41 },
    { 148, "harbour-0048",  48 },
};

static struct harbour harbour_table[HARBOUR_SLOTS];
static int harbour_used;

void harbour_reset(void)
{
    memset(harbour_table, 0, sizeof(harbour_table));
    harbour_used = 0;
}

int harbour_add(int key, const char *name, int berths)
{
    struct harbour *slot;

    if (harbour_used >= HARBOUR_SLOTS) {
        return -1;
    }

    slot = &harbour_table[harbour_used];
    slot->key = key;
    slot->berths = berths;
    strncpy(slot->name, name, HARBOUR_NAME_SIZE - 1);
    slot->name[HARBOUR_NAME_SIZE - 1] = '\0';
    harbour_used++;

    return harbour_used - 1;
}

struct harbour *harbour_find(int key)
{
    int i;

    for (i = 0; i < harbour_used; i++) {
        if (harbour_table[i].key == key) {
            return &harbour_table[i];
        }
    }

    return NULL;
}

int harbour_remove(int key)
{
    struct harbour *found;

    found = harbour_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = harbour_table[harbour_used - 1];
    harbour_used--;

    return 0;
}

int harbour_total_berths(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < harbour_used; i++) {
        total += harbour_table[i].berths;
    }

    return total;
}

void harbour_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(harbour_seed) / sizeof(harbour_seed[0]); i++) {
        harbour_add(harbour_seed[i].key, harbour_seed[i].name,
                    harbour_seed[i].berths);
    }
}

void harbour_report(FILE *out)
{
    int i;

    fprintf(out, "%d harbours\n", harbour_used);
    for (i = 0; i < harbour_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", harbour_table[i].key,
                harbour_table[i].name, harbour_table[i].berths);
    }
    fprintf(out, "  total berths %d\n", harbour_total_berths());
}

int main(void)
{
    harbour_reset();
    harbour_load_seed();
    harbour_remove(103);
    harbour_report(stdout);

    return 0;
}
