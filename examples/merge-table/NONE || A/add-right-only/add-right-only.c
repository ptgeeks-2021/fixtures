/*
 * A fixed size order table.
 *
 * It holds up to ORDER_SLOTS orders, each with a key, a short name, and
 * a quantity. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define ORDER_SLOTS 64
#define ORDER_NAME_SIZE 24

struct order {
    int key;
    char name[ORDER_NAME_SIZE];
    int quantity;
};

static const struct order order_seed[] = {
    { 101, "order-0001",  10 },
    { 102, "order-0002",  17 },
    { 103, "order-0003",  24 },
    { 104, "order-0004",  31 },
    { 105, "order-0005",  38 },
    { 106, "order-0006",  45 },
    { 107, "order-0007",  52 },
    { 108, "order-0008",  59 },
    { 109, "order-0009",  66 },
    { 110, "order-0010",  73 },
    { 111, "order-0011",  80 },
    { 112, "order-0012",  87 },
    { 113, "order-0013",  94 },
    { 114, "order-0014",   4 },
    { 115, "order-0015",  11 },
    { 116, "order-0016",  18 },
    { 117, "order-0017",  25 },
    { 118, "order-0018",  32 },
    { 119, "order-0019",  39 },
    { 120, "order-0020",  46 },
    { 121, "order-0021",  53 },
    { 122, "order-0022",  60 },
    { 123, "order-0023",  67 },
    { 124, "order-0024",  74 },
    { 125, "order-0025",  81 },
    { 126, "order-0026",  88 },
    { 127, "order-0027",  95 },
    { 128, "order-0028",   5 },
    { 129, "order-0029",  12 },
    { 130, "order-0030",  19 },
    { 131, "order-0031",  26 },
    { 132, "order-0032",  33 },
    { 133, "order-0033",  40 },
    { 134, "order-0034",  47 },
    { 135, "order-0035",  54 },
    { 136, "order-0036",  61 },
    { 137, "order-0037",  68 },
    { 138, "order-0038",  75 },
    { 139, "order-0039",  82 },
    { 140, "order-0040",  89 },
    { 141, "order-0041",  96 },
    { 142, "order-0042",   6 },
    { 143, "order-0043",  13 },
    { 144, "order-0044",  20 },
    { 145, "order-0045",  27 },
    { 146, "order-0046",  34 },
    { 147, "order-0047",  41 },
    { 148, "order-0048",  48 },
};

static struct order order_table[ORDER_SLOTS];
static int order_used;

void order_reset(void)
{
    memset(order_table, 0, sizeof(order_table));
    order_used = 0;
}

int order_add(int key, const char *name, int quantity)
{
    struct order *slot;

    if (order_used >= ORDER_SLOTS) {
        return -1;
    }

    slot = &order_table[order_used];
    slot->key = key;
    slot->quantity = quantity;
    strncpy(slot->name, name, ORDER_NAME_SIZE - 1);
    slot->name[ORDER_NAME_SIZE - 1] = '\0';
    order_used++;

    return order_used - 1;
}

struct order *order_find(int key)
{
    int i;

    for (i = 0; i < order_used; i++) {
        if (order_table[i].key == key) {
            return &order_table[i];
        }
    }

    return NULL;
}

int order_remove(int key)
{
    struct order *found;

    found = order_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = order_table[order_used - 1];
    order_used--;

    return 0;
}

int order_total_quantity(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < order_used; i++) {
        total += order_table[i].quantity;
    }

    return total;
}

void order_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(order_seed) / sizeof(order_seed[0]); i++) {
        order_add(order_seed[i].key, order_seed[i].name,
                    order_seed[i].quantity);
    }
}

void order_report(FILE *out)
{
    int i;

    fprintf(out, "%d orders\n", order_used);
    for (i = 0; i < order_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", order_table[i].key,
                order_table[i].name, order_table[i].quantity);
    }
    fprintf(out, "  total quantity %d\n", order_total_quantity());
}

int main(void)
{
    order_reset();
    order_load_seed();
    order_remove(103);
    order_report(stdout);

    return 0;
}
