/*
 * A fixed size license table.
 *
 * It holds up to LICENSE_SLOTS licenses, each with a key, a short name, and
 * a seats. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define LICENSE_SLOTS 64
#define LICENSE_NAME_SIZE 24

struct license {
    int key;
    char name[LICENSE_NAME_SIZE];
    int seats;
};

static const struct license license_seed[] = {
    { 101, "license-0001",  10 },
    { 102, "license-0002",  17 },
    { 103, "license-0003",  24 },
    { 104, "license-0004",  31 },
    { 105, "license-0005",  38 },
    { 106, "license-0006",  45 },
    { 107, "license-0007",  52 },
    { 108, "license-0008",  59 },
    { 109, "license-0009",  66 },
    { 110, "license-0010",  73 },
    { 111, "license-0011",  80 },
    { 112, "license-0012",  87 },
    { 113, "license-0013",  94 },
    { 114, "license-0014",   4 },
    { 115, "license-0015",  11 },
    { 116, "license-0016",  18 },
    { 117, "license-0017",  25 },
    { 118, "license-0018",  32 },
    { 119, "license-0019",  39 },
    { 120, "license-0020",  46 },
    { 121, "license-0021",  53 },
    { 122, "license-0022",  60 },
    { 123, "license-0023",  67 },
    { 124, "license-0024",  74 },
    { 125, "license-0025",  81 },
    { 126, "license-0026",  88 },
    { 127, "license-0027",  95 },
    { 128, "license-0028",   5 },
    { 129, "license-0029",  12 },
    { 130, "license-0030",  19 },
    { 131, "license-0031",  26 },
    { 132, "license-0032",  33 },
    { 133, "license-0033",  40 },
    { 134, "license-0034",  47 },
    { 135, "license-0035",  54 },
    { 136, "license-0036",  61 },
    { 137, "license-0037",  68 },
    { 138, "license-0038",  75 },
    { 139, "license-0039",  82 },
    { 140, "license-0040",  89 },
    { 141, "license-0041",  96 },
    { 142, "license-0042",   6 },
    { 143, "license-0043",  13 },
    { 144, "license-0044",  20 },
    { 145, "license-0045",  27 },
    { 146, "license-0046",  34 },
    { 147, "license-0047",  41 },
    { 148, "license-0048",  48 },
};

static struct license license_table[LICENSE_SLOTS];
static int license_used;

void license_reset(void)
{
    memset(license_table, 0, sizeof(license_table));
    license_used = 0;
}

int license_add(int key, const char *name, int seats)
{
    struct license *slot;

    if (license_used >= LICENSE_SLOTS) {
        return -1;
    }

    slot = &license_table[license_used];
    slot->key = key;
    slot->seats = seats;
    strncpy(slot->name, name, LICENSE_NAME_SIZE - 1);
    slot->name[LICENSE_NAME_SIZE - 1] = '\0';
    license_used++;

    return license_used - 1;
}

struct license *license_find(int key)
{
    int i;

    for (i = 0; i < license_used; i++) {
        if (license_table[i].key == key) {
            return &license_table[i];
        }
    }

    return NULL;
}

int license_remove(int key)
{
    struct license *found;

    found = license_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = license_table[license_used - 1];
    license_used--;

    return 0;
}

int license_total_seats(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < license_used; i++) {
        total += license_table[i].seats;
    }

    return total;
}

void license_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(license_seed) / sizeof(license_seed[0]); i++) {
        license_add(license_seed[i].key, license_seed[i].name,
                    license_seed[i].seats);
    }
}

void license_report(FILE *out)
{
    int i;

    fprintf(out, "%d licenses\n", license_used);
    for (i = 0; i < license_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", license_table[i].key,
                license_table[i].name, license_table[i].seats);
    }
    fprintf(out, "  total seats %d\n", license_total_seats());
}

int main(void)
{
    license_reset();
    license_load_seed();
    license_remove(103);
    license_report(stdout);

    return 0;
}
