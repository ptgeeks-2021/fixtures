/*
 * A fixed size telescope table.
 *
 * It holds up to TELESCOPE_SLOTS telescopes, each with a key, a short name, and
 * a apertures. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define TELESCOPE_SLOTS 128
#define TELESCOPE_NAME_SIZE 24

struct telescope {
    int key;
    char name[TELESCOPE_NAME_SIZE];
    int apertures;
};

static const struct telescope telescope_seed[] = {
    { 101, "telescope-0001",  10 },
    { 102, "telescope-0002",  17 },
    { 103, "telescope-0003",  24 },
    { 104, "telescope-0004",  31 },
    { 105, "telescope-0005",  38 },
    { 106, "telescope-0006",  45 },
    { 107, "telescope-0007",  52 },
    { 108, "telescope-0008",  59 },
    { 109, "telescope-0009",  66 },
    { 110, "telescope-0010",  73 },
    { 111, "telescope-0011",  80 },
    { 112, "telescope-0012",  87 },
    { 113, "telescope-0013",  94 },
    { 114, "telescope-0014",   4 },
    { 115, "telescope-0015",  11 },
    { 116, "telescope-0016",  18 },
    { 117, "telescope-0017",  25 },
    { 118, "telescope-0018",  32 },
    { 119, "telescope-0019",  39 },
    { 120, "telescope-0020",  46 },
    { 121, "telescope-0021",  53 },
    { 122, "telescope-0022",  60 },
    { 123, "telescope-0023",  67 },
    { 124, "telescope-0024",  74 },
    { 125, "telescope-0025",  81 },
    { 126, "telescope-0026",  88 },
    { 127, "telescope-0027",  95 },
    { 128, "telescope-0028",   5 },
    { 129, "telescope-0029",  12 },
    { 130, "telescope-0030",  19 },
    { 131, "telescope-0031",  26 },
    { 132, "telescope-0032",  33 },
    { 133, "telescope-0033",  40 },
    { 134, "telescope-0034",  47 },
    { 135, "telescope-0035",  54 },
    { 136, "telescope-0036",  61 },
    { 137, "telescope-0037",  68 },
    { 138, "telescope-0038",  75 },
    { 139, "telescope-0039",  82 },
    { 140, "telescope-0040",  89 },
    { 141, "telescope-0041",  96 },
    { 142, "telescope-0042",   6 },
    { 143, "telescope-0043",  13 },
    { 144, "telescope-0044",  20 },
    { 145, "telescope-0045",  27 },
    { 146, "telescope-0046",  34 },
    { 147, "telescope-0047",  41 },
    { 148, "telescope-0048",  48 },
};

static struct telescope telescope_table[TELESCOPE_SLOTS];
static int telescope_used;

void telescope_reset(void)
{
    memset(telescope_table, 0, sizeof(telescope_table));
    telescope_used = 0;
}

int telescope_add(int key, const char *name, int apertures)
{
    struct telescope *slot;

    if (telescope_used >= TELESCOPE_SLOTS) {
        return -1;
    }

    slot = &telescope_table[telescope_used];
    slot->key = key;
    slot->apertures = apertures;
    strncpy(slot->name, name, TELESCOPE_NAME_SIZE - 1);
    slot->name[TELESCOPE_NAME_SIZE - 1] = '\0';
    telescope_used++;

    return telescope_used - 1;
}

struct telescope *telescope_find(int key)
{
    int i;

    for (i = 0; i < telescope_used; i++) {
        if (telescope_table[i].key == key) {
            return &telescope_table[i];
        }
    }

    return NULL;
}

int telescope_remove(int key)
{
    struct telescope *found;

    found = telescope_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = telescope_table[telescope_used - 1];
    telescope_used--;

    return 0;
}

int telescope_total_apertures(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < telescope_used; i++) {
        total += telescope_table[i].apertures;
    }

    return total;
}

void telescope_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(telescope_seed) / sizeof(telescope_seed[0]); i++) {
        telescope_add(telescope_seed[i].key, telescope_seed[i].name,
                    telescope_seed[i].apertures);
    }
}

void telescope_report(FILE *out)
{
    int i;

    fprintf(out, "%d telescopes\n", telescope_used);
    for (i = 0; i < telescope_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", telescope_table[i].key,
                telescope_table[i].name, telescope_table[i].apertures);
    }
    fprintf(out, "  total apertures %d\n", telescope_total_apertures());
}

int main(void)
{
    telescope_reset();
    telescope_load_seed();
    telescope_remove(103);
    telescope_report(stdout);

    return 0;
}
