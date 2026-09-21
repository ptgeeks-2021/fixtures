/*
 * A fixed size payment table.
 *
 * It holds up to PAYMENT_SLOTS payments, each with a key, a short name, and
 * a cents. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define PAYMENT_SLOTS 96
#define PAYMENT_NAME_SIZE 24

struct payment {
    int key;
    char name[PAYMENT_NAME_SIZE];
    int cents;
};

static const struct payment payment_seed[] = {
    { 101, "payment-0001",  10 },
    { 102, "payment-0002",  17 },
    { 103, "payment-0003",  24 },
    { 104, "payment-0004",  31 },
    { 105, "payment-0005",  38 },
    { 106, "payment-0006",  45 },
    { 107, "payment-0007",  52 },
    { 108, "payment-0008",  59 },
    { 109, "payment-0009",  66 },
    { 110, "payment-0010",  73 },
    { 111, "payment-0011",  80 },
    { 112, "payment-0012",  87 },
    { 113, "payment-0013",  94 },
    { 114, "payment-0014",   4 },
    { 115, "payment-0015",  11 },
    { 116, "payment-0016",  18 },
    { 117, "payment-0017",  25 },
    { 118, "payment-0018",  32 },
    { 119, "payment-0019",  39 },
    { 120, "payment-0020",  46 },
    { 121, "payment-0021",  53 },
    { 122, "payment-0022",  60 },
    { 123, "payment-0023",  67 },
    { 124, "payment-0024",  74 },
    { 125, "payment-0025",  81 },
    { 126, "payment-0026",  88 },
    { 127, "payment-0027",  95 },
    { 128, "payment-0028",   5 },
    { 129, "payment-0029",  12 },
    { 130, "payment-0030",  19 },
    { 131, "payment-0031",  26 },
    { 132, "payment-0032",  33 },
    { 133, "payment-0033",  40 },
    { 134, "payment-0034",  47 },
    { 135, "payment-0035",  54 },
    { 136, "payment-0036",  61 },
    { 137, "payment-0037",  68 },
    { 138, "payment-0038",  75 },
    { 139, "payment-0039",  82 },
    { 140, "payment-0040",  89 },
    { 141, "payment-0041",  96 },
    { 142, "payment-0042",   6 },
    { 143, "payment-0043",  13 },
    { 144, "payment-0044",  20 },
    { 145, "payment-0045",  27 },
    { 146, "payment-0046",  34 },
    { 147, "payment-0047",  41 },
    { 148, "payment-0048",  48 },
};

static struct payment payment_table[PAYMENT_SLOTS];
static int payment_used;

void payment_reset(void)
{
    memset(payment_table, 0, sizeof(payment_table));
    payment_used = 0;
}

int payment_add(int key, const char *name, int cents)
{
    struct payment *slot;

    if (payment_used >= PAYMENT_SLOTS) {
        return -1;
    }

    slot = &payment_table[payment_used];
    slot->key = key;
    slot->cents = cents;
    strncpy(slot->name, name, PAYMENT_NAME_SIZE - 1);
    slot->name[PAYMENT_NAME_SIZE - 1] = '\0';
    payment_used++;

    return payment_used - 1;
}

struct payment *payment_find(int key)
{
    int i;

    for (i = 0; i < payment_used; i++) {
        if (payment_table[i].key == key) {
            return &payment_table[i];
        }
    }

    return NULL;
}

int payment_remove(int key)
{
    struct payment *found;

    found = payment_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = payment_table[payment_used - 1];
    payment_used--;

    return 0;
}

int payment_total_cents(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < payment_used; i++) {
        total += payment_table[i].cents;
    }

    return total;
}

void payment_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(payment_seed) / sizeof(payment_seed[0]); i++) {
        payment_add(payment_seed[i].key, payment_seed[i].name,
                    payment_seed[i].cents);
    }
}

void payment_report(FILE *out)
{
    int i;

    fprintf(out, "%d payments\n", payment_used);
    for (i = 0; i < payment_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", payment_table[i].key,
                payment_table[i].name, payment_table[i].cents);
    }
    fprintf(out, "  total cents %d\n", payment_total_cents());
}

int main(void)
{
    payment_reset();
    payment_load_seed();
    payment_remove(103);
    payment_report(stdout);

    return 0;
}
