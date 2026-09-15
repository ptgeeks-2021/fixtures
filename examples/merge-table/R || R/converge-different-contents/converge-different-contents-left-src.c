/*
 * A fixed size printer table.
 *
 * It holds up to PRINTER_SLOTS printers, each with a key, a short name, and
 * a pages. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define PRINTER_SLOTS 96
#define PRINTER_NAME_SIZE 24

struct printer {
    int key;
    char name[PRINTER_NAME_SIZE];
    int pages;
};

static const struct printer printer_seed[] = {
    { 101, "printer-0001",  10 },
    { 102, "printer-0002",  17 },
    { 103, "printer-0003",  24 },
    { 104, "printer-0004",  31 },
    { 105, "printer-0005",  38 },
    { 106, "printer-0006",  45 },
    { 107, "printer-0007",  52 },
    { 108, "printer-0008",  59 },
    { 109, "printer-0009",  66 },
    { 110, "printer-0010",  73 },
    { 111, "printer-0011",  80 },
    { 112, "printer-0012",  87 },
    { 113, "printer-0013",  94 },
    { 114, "printer-0014",   4 },
    { 115, "printer-0015",  11 },
    { 116, "printer-0016",  18 },
    { 117, "printer-0017",  25 },
    { 118, "printer-0018",  32 },
    { 119, "printer-0019",  39 },
    { 120, "printer-0020",  46 },
    { 121, "printer-0021",  53 },
    { 122, "printer-0022",  60 },
    { 123, "printer-0023",  67 },
    { 124, "printer-0024",  74 },
    { 125, "printer-0025",  81 },
    { 126, "printer-0026",  88 },
    { 127, "printer-0027",  95 },
    { 128, "printer-0028",   5 },
    { 129, "printer-0029",  12 },
    { 130, "printer-0030",  19 },
    { 131, "printer-0031",  26 },
    { 132, "printer-0032",  33 },
    { 133, "printer-0033",  40 },
    { 134, "printer-0034",  47 },
    { 135, "printer-0035",  54 },
    { 136, "printer-0036",  61 },
    { 137, "printer-0037",  68 },
    { 138, "printer-0038",  75 },
    { 139, "printer-0039",  82 },
    { 140, "printer-0040",  89 },
    { 141, "printer-0041",  96 },
    { 142, "printer-0042",   6 },
    { 143, "printer-0043",  13 },
    { 144, "printer-0044",  20 },
    { 145, "printer-0045",  27 },
    { 146, "printer-0046",  34 },
    { 147, "printer-0047",  41 },
    { 148, "printer-0048",  48 },
};

static struct printer printer_table[PRINTER_SLOTS];
static int printer_used;

void printer_reset(void)
{
    memset(printer_table, 0, sizeof(printer_table));
    printer_used = 0;
}

int printer_add(int key, const char *name, int pages)
{
    struct printer *slot;

    if (printer_used >= PRINTER_SLOTS) {
        return -1;
    }

    slot = &printer_table[printer_used];
    slot->key = key;
    slot->pages = pages;
    strncpy(slot->name, name, PRINTER_NAME_SIZE - 1);
    slot->name[PRINTER_NAME_SIZE - 1] = '\0';
    printer_used++;

    return printer_used - 1;
}

struct printer *printer_find(int key)
{
    int i;

    for (i = 0; i < printer_used; i++) {
        if (printer_table[i].key == key) {
            return &printer_table[i];
        }
    }

    return NULL;
}

int printer_remove(int key)
{
    struct printer *found;

    found = printer_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = printer_table[printer_used - 1];
    printer_used--;

    return 0;
}

int printer_total_pages(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < printer_used; i++) {
        total += printer_table[i].pages;
    }

    return total;
}

void printer_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(printer_seed) / sizeof(printer_seed[0]); i++) {
        printer_add(printer_seed[i].key, printer_seed[i].name,
                    printer_seed[i].pages);
    }
}

void printer_report(FILE *out)
{
    int i;

    fprintf(out, "%d printers\n", printer_used);
    for (i = 0; i < printer_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", printer_table[i].key,
                printer_table[i].name, printer_table[i].pages);
    }
    fprintf(out, "  total pages %d\n", printer_total_pages());
}

int main(void)
{
    printer_reset();
    printer_load_seed();
    printer_remove(103);
    printer_report(stdout);

    return 0;
}
