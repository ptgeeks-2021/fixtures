/*
 * A fixed size scanner table.
 *
 * It holds up to SCANNER_SLOTS scanners, each with a key, a short name, and
 * a sheets. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define SCANNER_SLOTS 64
#define SCANNER_NAME_SIZE 24

struct scanner {
    int key;
    char name[SCANNER_NAME_SIZE];
    int sheets;
};

static const struct scanner scanner_seed[] = {
    { 101, "scanner-0001",  10 },
    { 102, "scanner-0002",  17 },
    { 103, "scanner-0003",  24 },
    { 104, "scanner-0004",  31 },
    { 105, "scanner-0005",  38 },
    { 106, "scanner-0006",  45 },
    { 107, "scanner-0007",  52 },
    { 108, "scanner-0008",  59 },
    { 109, "scanner-0009",  66 },
    { 110, "scanner-0010",  73 },
    { 111, "scanner-0011",  80 },
    { 112, "scanner-0012",  87 },
    { 113, "scanner-0013",  94 },
    { 114, "scanner-0014",   4 },
    { 115, "scanner-0015",  11 },
    { 116, "scanner-0016",  18 },
    { 117, "scanner-0017",  25 },
    { 118, "scanner-0018",  32 },
    { 119, "scanner-0019",  39 },
    { 120, "scanner-0020",  46 },
    { 121, "scanner-0021",  53 },
    { 122, "scanner-0022",  60 },
    { 123, "scanner-0023",  67 },
    { 124, "scanner-0024",  74 },
    { 125, "scanner-0025",  81 },
    { 126, "scanner-0026",  88 },
    { 127, "scanner-0027",  95 },
    { 128, "scanner-0028",   5 },
    { 129, "scanner-0029",  12 },
    { 130, "scanner-0030",  19 },
    { 131, "scanner-0031",  26 },
    { 132, "scanner-0032",  33 },
    { 133, "scanner-0033",  40 },
    { 134, "scanner-0034",  47 },
    { 135, "scanner-0035",  54 },
    { 136, "scanner-0036",  61 },
    { 137, "scanner-0037",  68 },
    { 138, "scanner-0038",  75 },
    { 139, "scanner-0039",  82 },
    { 140, "scanner-0040",  89 },
    { 141, "scanner-0041",  96 },
    { 142, "scanner-0042",   6 },
    { 143, "scanner-0043",  13 },
    { 144, "scanner-0044",  20 },
    { 145, "scanner-0045",  27 },
    { 146, "scanner-0046",  34 },
    { 147, "scanner-0047",  41 },
    { 148, "scanner-0048",  48 },
};

static struct scanner scanner_table[SCANNER_SLOTS];
static int scanner_used;

void scanner_reset(void)
{
    memset(scanner_table, 0, sizeof(scanner_table));
    scanner_used = 0;
}

int scanner_add(int key, const char *name, int sheets)
{
    struct scanner *slot;

    if (scanner_used >= SCANNER_SLOTS) {
        return -1;
    }

    slot = &scanner_table[scanner_used];
    slot->key = key;
    slot->sheets = sheets;
    strncpy(slot->name, name, SCANNER_NAME_SIZE - 1);
    slot->name[SCANNER_NAME_SIZE - 1] = '\0';
    scanner_used++;

    return scanner_used - 1;
}

struct scanner *scanner_find(int key)
{
    int i;

    for (i = 0; i < scanner_used; i++) {
        if (scanner_table[i].key == key) {
            return &scanner_table[i];
        }
    }

    return NULL;
}

int scanner_remove(int key)
{
    struct scanner *found;

    found = scanner_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = scanner_table[scanner_used - 1];
    scanner_used--;

    return 0;
}

int scanner_total_sheets(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < scanner_used; i++) {
        total += scanner_table[i].sheets;
    }

    return total;
}

void scanner_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(scanner_seed) / sizeof(scanner_seed[0]); i++) {
        scanner_add(scanner_seed[i].key, scanner_seed[i].name,
                    scanner_seed[i].sheets);
    }
}

void scanner_report(FILE *out)
{
    int i;

    fprintf(out, "%d scanners\n", scanner_used);
    for (i = 0; i < scanner_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", scanner_table[i].key,
                scanner_table[i].name, scanner_table[i].sheets);
    }
    fprintf(out, "  total sheets %d\n", scanner_total_sheets());
}

int main(void)
{
    scanner_reset();
    scanner_load_seed();
    scanner_remove(103);
    scanner_report(stdout);

    return 0;
}
