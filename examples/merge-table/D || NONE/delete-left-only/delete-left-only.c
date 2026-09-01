/*
 * A fixed size contract table.
 *
 * It holds up to CONTRACT_SLOTS contracts, each with a key, a short name, and
 * a months. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define CONTRACT_SLOTS 64
#define CONTRACT_NAME_SIZE 24

struct contract {
    int key;
    char name[CONTRACT_NAME_SIZE];
    int months;
};

static const struct contract contract_seed[] = {
    { 101, "contract-0001",  10 },
    { 102, "contract-0002",  17 },
    { 103, "contract-0003",  24 },
    { 104, "contract-0004",  31 },
    { 105, "contract-0005",  38 },
    { 106, "contract-0006",  45 },
    { 107, "contract-0007",  52 },
    { 108, "contract-0008",  59 },
    { 109, "contract-0009",  66 },
    { 110, "contract-0010",  73 },
    { 111, "contract-0011",  80 },
    { 112, "contract-0012",  87 },
    { 113, "contract-0013",  94 },
    { 114, "contract-0014",   4 },
    { 115, "contract-0015",  11 },
    { 116, "contract-0016",  18 },
    { 117, "contract-0017",  25 },
    { 118, "contract-0018",  32 },
    { 119, "contract-0019",  39 },
    { 120, "contract-0020",  46 },
    { 121, "contract-0021",  53 },
    { 122, "contract-0022",  60 },
    { 123, "contract-0023",  67 },
    { 124, "contract-0024",  74 },
    { 125, "contract-0025",  81 },
    { 126, "contract-0026",  88 },
    { 127, "contract-0027",  95 },
    { 128, "contract-0028",   5 },
    { 129, "contract-0029",  12 },
    { 130, "contract-0030",  19 },
    { 131, "contract-0031",  26 },
    { 132, "contract-0032",  33 },
    { 133, "contract-0033",  40 },
    { 134, "contract-0034",  47 },
    { 135, "contract-0035",  54 },
    { 136, "contract-0036",  61 },
    { 137, "contract-0037",  68 },
    { 138, "contract-0038",  75 },
    { 139, "contract-0039",  82 },
    { 140, "contract-0040",  89 },
    { 141, "contract-0041",  96 },
    { 142, "contract-0042",   6 },
    { 143, "contract-0043",  13 },
    { 144, "contract-0044",  20 },
    { 145, "contract-0045",  27 },
    { 146, "contract-0046",  34 },
    { 147, "contract-0047",  41 },
    { 148, "contract-0048",  48 },
};

static struct contract contract_table[CONTRACT_SLOTS];
static int contract_used;

void contract_reset(void)
{
    memset(contract_table, 0, sizeof(contract_table));
    contract_used = 0;
}

int contract_add(int key, const char *name, int months)
{
    struct contract *slot;

    if (contract_used >= CONTRACT_SLOTS) {
        return -1;
    }

    slot = &contract_table[contract_used];
    slot->key = key;
    slot->months = months;
    strncpy(slot->name, name, CONTRACT_NAME_SIZE - 1);
    slot->name[CONTRACT_NAME_SIZE - 1] = '\0';
    contract_used++;

    return contract_used - 1;
}

struct contract *contract_find(int key)
{
    int i;

    for (i = 0; i < contract_used; i++) {
        if (contract_table[i].key == key) {
            return &contract_table[i];
        }
    }

    return NULL;
}

int contract_remove(int key)
{
    struct contract *found;

    found = contract_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = contract_table[contract_used - 1];
    contract_used--;

    return 0;
}

int contract_total_months(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < contract_used; i++) {
        total += contract_table[i].months;
    }

    return total;
}

void contract_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(contract_seed) / sizeof(contract_seed[0]); i++) {
        contract_add(contract_seed[i].key, contract_seed[i].name,
                    contract_seed[i].months);
    }
}

void contract_report(FILE *out)
{
    int i;

    fprintf(out, "%d contracts\n", contract_used);
    for (i = 0; i < contract_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", contract_table[i].key,
                contract_table[i].name, contract_table[i].months);
    }
    fprintf(out, "  total months %d\n", contract_total_months());
}

int main(void)
{
    contract_reset();
    contract_load_seed();
    contract_remove(103);
    contract_report(stdout);

    return 0;
}
