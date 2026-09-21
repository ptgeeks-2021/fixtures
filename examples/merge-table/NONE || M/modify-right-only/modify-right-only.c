/*
 * A fixed size invoice table.
 *
 * It holds up to INVOICE_SLOTS invoices, each with a key, a short name, and
 * a amount. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define INVOICE_SLOTS 128
#define INVOICE_NAME_SIZE 24

struct invoice {
    int key;
    char name[INVOICE_NAME_SIZE];
    int amount;
};

static const struct invoice invoice_seed[] = {
    { 101, "invoice-0001",  10 },
    { 102, "invoice-0002",  17 },
    { 103, "invoice-0003",  24 },
    { 104, "invoice-0004",  31 },
    { 105, "invoice-0005",  38 },
    { 106, "invoice-0006",  45 },
    { 107, "invoice-0007",  52 },
    { 108, "invoice-0008",  59 },
    { 109, "invoice-0009",  66 },
    { 110, "invoice-0010",  73 },
    { 111, "invoice-0011",  80 },
    { 112, "invoice-0012",  87 },
    { 113, "invoice-0013",  94 },
    { 114, "invoice-0014",   4 },
    { 115, "invoice-0015",  11 },
    { 116, "invoice-0016",  18 },
    { 117, "invoice-0017",  25 },
    { 118, "invoice-0018",  32 },
    { 119, "invoice-0019",  39 },
    { 120, "invoice-0020",  46 },
    { 121, "invoice-0021",  53 },
    { 122, "invoice-0022",  60 },
    { 123, "invoice-0023",  67 },
    { 124, "invoice-0024",  74 },
    { 125, "invoice-0025",  81 },
    { 126, "invoice-0026",  88 },
    { 127, "invoice-0027",  95 },
    { 128, "invoice-0028",   5 },
    { 129, "invoice-0029",  12 },
    { 130, "invoice-0030",  19 },
    { 131, "invoice-0031",  26 },
    { 132, "invoice-0032",  33 },
    { 133, "invoice-0033",  40 },
    { 134, "invoice-0034",  47 },
    { 135, "invoice-0035",  54 },
    { 136, "invoice-0036",  61 },
    { 137, "invoice-0037",  68 },
    { 138, "invoice-0038",  75 },
    { 139, "invoice-0039",  82 },
    { 140, "invoice-0040",  89 },
    { 141, "invoice-0041",  96 },
    { 142, "invoice-0042",   6 },
    { 143, "invoice-0043",  13 },
    { 144, "invoice-0044",  20 },
    { 145, "invoice-0045",  27 },
    { 146, "invoice-0046",  34 },
    { 147, "invoice-0047",  41 },
    { 148, "invoice-0048",  48 },
};

static struct invoice invoice_table[INVOICE_SLOTS];
static int invoice_used;

void invoice_reset(void)
{
    memset(invoice_table, 0, sizeof(invoice_table));
    invoice_used = 0;
}

int invoice_add(int key, const char *name, int amount)
{
    struct invoice *slot;

    if (invoice_used >= INVOICE_SLOTS) {
        return -1;
    }

    slot = &invoice_table[invoice_used];
    slot->key = key;
    slot->amount = amount;
    strncpy(slot->name, name, INVOICE_NAME_SIZE - 1);
    slot->name[INVOICE_NAME_SIZE - 1] = '\0';
    invoice_used++;

    return invoice_used - 1;
}

struct invoice *invoice_find(int key)
{
    int i;

    for (i = 0; i < invoice_used; i++) {
        if (invoice_table[i].key == key) {
            return &invoice_table[i];
        }
    }

    return NULL;
}

int invoice_remove(int key)
{
    struct invoice *found;

    found = invoice_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = invoice_table[invoice_used - 1];
    invoice_used--;

    return 0;
}

int invoice_total_amount(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < invoice_used; i++) {
        total += invoice_table[i].amount;
    }

    return total;
}

void invoice_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(invoice_seed) / sizeof(invoice_seed[0]); i++) {
        invoice_add(invoice_seed[i].key, invoice_seed[i].name,
                    invoice_seed[i].amount);
    }
}

void invoice_report(FILE *out)
{
    int i;

    fprintf(out, "%d invoices\n", invoice_used);
    for (i = 0; i < invoice_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", invoice_table[i].key,
                invoice_table[i].name, invoice_table[i].amount);
    }
    fprintf(out, "  total amount %d\n", invoice_total_amount());
}

int main(void)
{
    invoice_reset();
    invoice_load_seed();
    invoice_remove(103);
    invoice_report(stdout);

    return 0;
}
