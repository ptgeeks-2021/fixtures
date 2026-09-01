/*
 * A fixed size voucher table.
 *
 * It holds up to VOUCHER_SLOTS vouchers, each with a key, a short name, and
 * a discount. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define VOUCHER_SLOTS 64
#define VOUCHER_NAME_SIZE 24

struct voucher {
    int key;
    char name[VOUCHER_NAME_SIZE];
    int discount;
};

static const struct voucher voucher_seed[] = {
    { 101, "voucher-0001",  10 },
    { 102, "voucher-0002",  17 },
    { 103, "voucher-0003",  24 },
    { 104, "voucher-0004",  31 },
    { 105, "voucher-0005",  38 },
    { 106, "voucher-0006",  45 },
    { 107, "voucher-0007",  52 },
    { 108, "voucher-0008",  59 },
    { 109, "voucher-0009",  66 },
    { 110, "voucher-0010",  73 },
    { 111, "voucher-0011",  80 },
    { 112, "voucher-0012",  87 },
    { 113, "voucher-0013",  94 },
    { 114, "voucher-0014",   4 },
    { 115, "voucher-0015",  11 },
    { 116, "voucher-0016",  18 },
    { 117, "voucher-0017",  25 },
    { 118, "voucher-0018",  32 },
    { 119, "voucher-0019",  39 },
    { 120, "voucher-0020",  46 },
    { 121, "voucher-0021",  53 },
    { 122, "voucher-0022",  60 },
    { 123, "voucher-0023",  67 },
    { 124, "voucher-0024",  74 },
    { 125, "voucher-0025",  81 },
    { 126, "voucher-0026",  88 },
    { 127, "voucher-0027",  95 },
    { 128, "voucher-0028",   5 },
    { 129, "voucher-0029",  12 },
    { 130, "voucher-0030",  19 },
    { 131, "voucher-0031",  26 },
    { 132, "voucher-0032",  33 },
    { 133, "voucher-0033",  40 },
    { 134, "voucher-0034",  47 },
    { 135, "voucher-0035",  54 },
    { 136, "voucher-0036",  61 },
    { 137, "voucher-0037",  68 },
    { 138, "voucher-0038",  75 },
    { 139, "voucher-0039",  82 },
    { 140, "voucher-0040",  89 },
    { 141, "voucher-0041",  96 },
    { 142, "voucher-0042",   6 },
    { 143, "voucher-0043",  13 },
    { 144, "voucher-0044",  20 },
    { 145, "voucher-0045",  27 },
    { 146, "voucher-0046",  34 },
    { 147, "voucher-0047",  41 },
    { 148, "voucher-0048",  48 },
};

static struct voucher voucher_table[VOUCHER_SLOTS];
static int voucher_used;

void voucher_reset(void)
{
    memset(voucher_table, 0, sizeof(voucher_table));
    voucher_used = 0;
}

int voucher_add(int key, const char *name, int discount)
{
    struct voucher *slot;

    if (voucher_used >= VOUCHER_SLOTS) {
        return -1;
    }

    slot = &voucher_table[voucher_used];
    slot->key = key;
    slot->discount = discount;
    strncpy(slot->name, name, VOUCHER_NAME_SIZE - 1);
    slot->name[VOUCHER_NAME_SIZE - 1] = '\0';
    voucher_used++;

    return voucher_used - 1;
}

struct voucher *voucher_find(int key)
{
    int i;

    for (i = 0; i < voucher_used; i++) {
        if (voucher_table[i].key == key) {
            return &voucher_table[i];
        }
    }

    return NULL;
}

int voucher_remove(int key)
{
    struct voucher *found;

    found = voucher_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = voucher_table[voucher_used - 1];
    voucher_used--;

    return 0;
}

int voucher_total_discount(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < voucher_used; i++) {
        total += voucher_table[i].discount;
    }

    return total;
}

void voucher_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(voucher_seed) / sizeof(voucher_seed[0]); i++) {
        voucher_add(voucher_seed[i].key, voucher_seed[i].name,
                    voucher_seed[i].discount);
    }
}

void voucher_report(FILE *out)
{
    int i;

    fprintf(out, "%d vouchers\n", voucher_used);
    for (i = 0; i < voucher_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", voucher_table[i].key,
                voucher_table[i].name, voucher_table[i].discount);
    }
    fprintf(out, "  total discount %d\n", voucher_total_discount());
}

int main(void)
{
    voucher_reset();
    voucher_load_seed();
    voucher_remove(103);
    voucher_report(stdout);

    return 0;
}
