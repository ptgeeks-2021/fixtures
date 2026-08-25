/*
 * A fixed size shipment table.
 *
 * It holds up to SHIPMENT_SLOTS shipments, each with a key, a short name, and
 * a weight. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define SHIPMENT_SLOTS 64
#define SHIPMENT_NAME_SIZE 24

struct shipment {
    int key;
    char name[SHIPMENT_NAME_SIZE];
    int weight;
};

static const struct shipment shipment_seed[] = {
    { 101, "shipment-0001",  10 },
    { 102, "shipment-0002",  17 },
    { 103, "shipment-0003",  24 },
    { 104, "shipment-0004",  31 },
    { 105, "shipment-0005",  38 },
    { 106, "shipment-0006",  45 },
    { 107, "shipment-0007",  52 },
    { 108, "shipment-0008",  59 },
    { 109, "shipment-0009",  66 },
    { 110, "shipment-0010",  73 },
    { 111, "shipment-0011",  80 },
    { 112, "shipment-0012",  87 },
    { 113, "shipment-0013",  94 },
    { 114, "shipment-0014",   4 },
    { 115, "shipment-0015",  11 },
    { 116, "shipment-0016",  18 },
    { 117, "shipment-0017",  25 },
    { 118, "shipment-0018",  32 },
    { 119, "shipment-0019",  39 },
    { 120, "shipment-0020",  46 },
    { 121, "shipment-0021",  53 },
    { 122, "shipment-0022",  60 },
    { 123, "shipment-0023",  67 },
    { 124, "shipment-0024",  74 },
    { 125, "shipment-0025",  81 },
    { 126, "shipment-0026",  88 },
    { 127, "shipment-0027",  95 },
    { 128, "shipment-0028",   5 },
    { 129, "shipment-0029",  12 },
    { 130, "shipment-0030",  19 },
    { 131, "shipment-0031",  26 },
    { 132, "shipment-0032",  33 },
    { 133, "shipment-0033",  40 },
    { 134, "shipment-0034",  47 },
    { 135, "shipment-0035",  54 },
    { 136, "shipment-0036",  61 },
    { 137, "shipment-0037",  68 },
    { 138, "shipment-0038",  75 },
    { 139, "shipment-0039",  82 },
    { 140, "shipment-0040",  89 },
    { 141, "shipment-0041",  96 },
    { 142, "shipment-0042",   6 },
    { 143, "shipment-0043",  13 },
    { 144, "shipment-0044",  20 },
    { 145, "shipment-0045",  27 },
    { 146, "shipment-0046",  34 },
    { 147, "shipment-0047",  41 },
    { 148, "shipment-0048",  48 },
};

static struct shipment shipment_table[SHIPMENT_SLOTS];
static int shipment_used;

void shipment_reset(void)
{
    memset(shipment_table, 0, sizeof(shipment_table));
    shipment_used = 0;
}

int shipment_add(int key, const char *name, int weight)
{
    struct shipment *slot;

    if (shipment_used >= SHIPMENT_SLOTS) {
        return -1;
    }

    slot = &shipment_table[shipment_used];
    slot->key = key;
    slot->weight = weight;
    strncpy(slot->name, name, SHIPMENT_NAME_SIZE - 1);
    slot->name[SHIPMENT_NAME_SIZE - 1] = '\0';
    shipment_used++;

    return shipment_used - 1;
}

struct shipment *shipment_find(int key)
{
    int i;

    for (i = 0; i < shipment_used; i++) {
        if (shipment_table[i].key == key) {
            return &shipment_table[i];
        }
    }

    return NULL;
}

int shipment_remove(int key)
{
    struct shipment *found;

    found = shipment_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = shipment_table[shipment_used - 1];
    shipment_used--;

    return 0;
}

int shipment_total_weight(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < shipment_used; i++) {
        total += shipment_table[i].weight;
    }

    return total;
}

void shipment_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(shipment_seed) / sizeof(shipment_seed[0]); i++) {
        shipment_add(shipment_seed[i].key, shipment_seed[i].name,
                    shipment_seed[i].weight);
    }
}

void shipment_report(FILE *out)
{
    int i;

    fprintf(out, "%d shipments\n", shipment_used);
    for (i = 0; i < shipment_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", shipment_table[i].key,
                shipment_table[i].name, shipment_table[i].weight);
    }
    fprintf(out, "  total weight %d\n", shipment_total_weight());
}

int main(void)
{
    shipment_reset();
    shipment_load_seed();
    shipment_remove(103);
    shipment_report(stdout);

    return 0;
}
