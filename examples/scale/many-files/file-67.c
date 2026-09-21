/*
 * A fixed size lighthouse table.
 *
 * It holds up to LIGHTHOUSE_SLOTS lighthouses, each with a key, a short name, and
 * a flashes. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define LIGHTHOUSE_SLOTS 96
#define LIGHTHOUSE_NAME_SIZE 24

struct lighthouse {
    int key;
    char name[LIGHTHOUSE_NAME_SIZE];
    int flashes;
};

static const struct lighthouse lighthouse_seed[] = {
    { 101, "lighthouse-0001",  10 },
    { 102, "lighthouse-0002",  17 },
    { 103, "lighthouse-0003",  24 },
    { 104, "lighthouse-0004",  31 },
    { 105, "lighthouse-0005",  38 },
    { 106, "lighthouse-0006",  45 },
    { 107, "lighthouse-0007",  52 },
    { 108, "lighthouse-0008",  59 },
    { 109, "lighthouse-0009",  66 },
    { 110, "lighthouse-0010",  73 },
    { 111, "lighthouse-0011",  80 },
    { 112, "lighthouse-0012",  87 },
    { 113, "lighthouse-0013",  94 },
    { 114, "lighthouse-0014",   4 },
    { 115, "lighthouse-0015",  11 },
    { 116, "lighthouse-0016",  18 },
    { 117, "lighthouse-0017",  25 },
    { 118, "lighthouse-0018",  32 },
    { 119, "lighthouse-0019",  39 },
    { 120, "lighthouse-0020",  46 },
    { 121, "lighthouse-0021",  53 },
    { 122, "lighthouse-0022",  60 },
    { 123, "lighthouse-0023",  67 },
    { 124, "lighthouse-0024",  74 },
    { 125, "lighthouse-0025",  81 },
    { 126, "lighthouse-0026",  88 },
    { 127, "lighthouse-0027",  95 },
    { 128, "lighthouse-0028",   5 },
    { 129, "lighthouse-0029",  12 },
    { 130, "lighthouse-0030",  19 },
    { 131, "lighthouse-0031",  26 },
    { 132, "lighthouse-0032",  33 },
    { 133, "lighthouse-0033",  40 },
    { 134, "lighthouse-0034",  47 },
    { 135, "lighthouse-0035",  54 },
    { 136, "lighthouse-0036",  61 },
    { 137, "lighthouse-0037",  68 },
    { 138, "lighthouse-0038",  75 },
    { 139, "lighthouse-0039",  82 },
    { 140, "lighthouse-0040",  89 },
    { 141, "lighthouse-0041",  96 },
    { 142, "lighthouse-0042",   6 },
    { 143, "lighthouse-0043",  13 },
    { 144, "lighthouse-0044",  20 },
    { 145, "lighthouse-0045",  27 },
    { 146, "lighthouse-0046",  34 },
    { 147, "lighthouse-0047",  41 },
    { 148, "lighthouse-0048",  48 },
};

static struct lighthouse lighthouse_table[LIGHTHOUSE_SLOTS];
static int lighthouse_used;

void lighthouse_reset(void)
{
    memset(lighthouse_table, 0, sizeof(lighthouse_table));
    lighthouse_used = 0;
}

int lighthouse_add(int key, const char *name, int flashes)
{
    struct lighthouse *slot;

    if (lighthouse_used >= LIGHTHOUSE_SLOTS) {
        return -1;
    }

    slot = &lighthouse_table[lighthouse_used];
    slot->key = key;
    slot->flashes = flashes;
    strncpy(slot->name, name, LIGHTHOUSE_NAME_SIZE - 1);
    slot->name[LIGHTHOUSE_NAME_SIZE - 1] = '\0';
    lighthouse_used++;

    return lighthouse_used - 1;
}

struct lighthouse *lighthouse_find(int key)
{
    int i;

    for (i = 0; i < lighthouse_used; i++) {
        if (lighthouse_table[i].key == key) {
            return &lighthouse_table[i];
        }
    }

    return NULL;
}

int lighthouse_remove(int key)
{
    struct lighthouse *found;

    found = lighthouse_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = lighthouse_table[lighthouse_used - 1];
    lighthouse_used--;

    return 0;
}

int lighthouse_total_flashes(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < lighthouse_used; i++) {
        total += lighthouse_table[i].flashes;
    }

    return total;
}

void lighthouse_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(lighthouse_seed) / sizeof(lighthouse_seed[0]); i++) {
        lighthouse_add(lighthouse_seed[i].key, lighthouse_seed[i].name,
                    lighthouse_seed[i].flashes);
    }
}

void lighthouse_report(FILE *out)
{
    int i;

    fprintf(out, "%d lighthouses\n", lighthouse_used);
    for (i = 0; i < lighthouse_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", lighthouse_table[i].key,
                lighthouse_table[i].name, lighthouse_table[i].flashes);
    }
    fprintf(out, "  total flashes %d\n", lighthouse_total_flashes());
}

int main(void)
{
    lighthouse_reset();
    lighthouse_load_seed();
    lighthouse_remove(103);
    lighthouse_report(stdout);

    return 0;
}
