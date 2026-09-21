/*
 * A fixed size package table.
 *
 * It holds up to PACKAGE_SLOTS packages, each with a key, a short name, and
 * a volume. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define PACKAGE_SLOTS 64
#define PACKAGE_NAME_SIZE 24

struct package {
    int key;
    char name[PACKAGE_NAME_SIZE];
    int volume;
};

static const struct package package_seed[] = {
    { 101, "package-0001",  10 },
    { 102, "package-0002",  17 },
    { 103, "package-0003",  24 },
    { 104, "package-0004",  31 },
    { 105, "package-0005",  38 },
    { 106, "package-0006",  45 },
    { 107, "package-0007",  52 },
    { 108, "package-0008",  59 },
    { 109, "package-0009",  66 },
    { 110, "package-0010",  73 },
    { 111, "package-0011",  80 },
    { 112, "package-0012",  87 },
    { 113, "package-0013",  94 },
    { 114, "package-0014",   4 },
    { 115, "package-0015",  11 },
    { 116, "package-0016",  18 },
    { 117, "package-0017",  25 },
    { 118, "package-0018",  32 },
    { 119, "package-0019",  39 },
    { 120, "package-0020",  46 },
    { 121, "package-0021",  53 },
    { 122, "package-0022",  60 },
    { 123, "package-0023",  67 },
    { 124, "package-0024",  74 },
    { 125, "package-0025",  81 },
    { 126, "package-0026",  88 },
    { 127, "package-0027",  95 },
    { 128, "package-0028",   5 },
    { 129, "package-0029",  12 },
    { 130, "package-0030",  19 },
    { 131, "package-0031",  26 },
    { 132, "package-0032",  33 },
    { 133, "package-0033",  40 },
    { 134, "package-0034",  47 },
    { 135, "package-0035",  54 },
    { 136, "package-0036",  61 },
    { 137, "package-0037",  68 },
    { 138, "package-0038",  75 },
    { 139, "package-0039",  82 },
    { 140, "package-0040",  89 },
    { 141, "package-0041",  96 },
    { 142, "package-0042",   6 },
    { 143, "package-0043",  13 },
    { 144, "package-0044",  20 },
    { 145, "package-0045",  27 },
    { 146, "package-0046",  34 },
    { 147, "package-0047",  41 },
    { 148, "package-0048",  48 },
};

static struct package package_table[PACKAGE_SLOTS];
static int package_used;

void package_reset(void)
{
    memset(package_table, 0, sizeof(package_table));
    package_used = 0;
}

int package_add(int key, const char *name, int volume)
{
    struct package *slot;

    if (package_used >= PACKAGE_SLOTS) {
        return -1;
    }

    slot = &package_table[package_used];
    slot->key = key;
    slot->volume = volume;
    strncpy(slot->name, name, PACKAGE_NAME_SIZE - 1);
    slot->name[PACKAGE_NAME_SIZE - 1] = '\0';
    package_used++;

    return package_used - 1;
}

struct package *package_find(int key)
{
    int i;

    for (i = 0; i < package_used; i++) {
        if (package_table[i].key == key) {
            return &package_table[i];
        }
    }

    return NULL;
}

int package_remove(int key)
{
    struct package *found;

    found = package_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = package_table[package_used - 1];
    package_used--;

    return 0;
}

int package_total_volume(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < package_used; i++) {
        total += package_table[i].volume;
    }

    return total;
}

void package_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(package_seed) / sizeof(package_seed[0]); i++) {
        package_add(package_seed[i].key, package_seed[i].name,
                    package_seed[i].volume);
    }
}

void package_report(FILE *out)
{
    int i;

    fprintf(out, "%d packages\n", package_used);
    for (i = 0; i < package_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", package_table[i].key,
                package_table[i].name, package_table[i].volume);
    }
    fprintf(out, "  total volume %d\n", package_total_volume());
}

int main(void)
{
    package_reset();
    package_load_seed();
    package_remove(103);
    package_report(stdout);

    return 0;
}
