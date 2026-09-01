/*
 * A fixed size tenant table.
 *
 * It holds up to TENANT_SLOTS tenants, each with a key, a short name, and
 * a quota. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define TENANT_SLOTS 64
#define TENANT_NAME_SIZE 24

struct tenant {
    int key;
    char name[TENANT_NAME_SIZE];
    int quota;
};

static const struct tenant tenant_seed[] = {
    { 101, "tenant-0001",  10 },
    { 102, "tenant-0002",  17 },
    { 103, "tenant-0003",  24 },
    { 104, "tenant-0004",  31 },
    { 105, "tenant-0005",  38 },
    { 106, "tenant-0006",  45 },
    { 107, "tenant-0007",  52 },
    { 108, "tenant-0008",  59 },
    { 109, "tenant-0009",  66 },
    { 110, "tenant-0010",  73 },
    { 111, "tenant-0011",  80 },
    { 112, "tenant-0012",  87 },
    { 113, "tenant-0013",  94 },
    { 114, "tenant-0014",   4 },
    { 115, "tenant-0015",  11 },
    { 116, "tenant-0016",  18 },
    { 117, "tenant-0017",  25 },
    { 118, "tenant-0018",  32 },
    { 119, "tenant-0019",  39 },
    { 120, "tenant-0020",  46 },
    { 121, "tenant-0021",  53 },
    { 122, "tenant-0022",  60 },
    { 123, "tenant-0023",  67 },
    { 124, "tenant-0024",  74 },
    { 125, "tenant-0025",  81 },
    { 126, "tenant-0026",  88 },
    { 127, "tenant-0027",  95 },
    { 128, "tenant-0028",   5 },
    { 129, "tenant-0029",  12 },
    { 130, "tenant-0030",  19 },
    { 131, "tenant-0031",  26 },
    { 132, "tenant-0032",  33 },
    { 133, "tenant-0033",  40 },
    { 134, "tenant-0034",  47 },
    { 135, "tenant-0035",  54 },
    { 136, "tenant-0036",  61 },
    { 137, "tenant-0037",  68 },
    { 138, "tenant-0038",  75 },
    { 139, "tenant-0039",  82 },
    { 140, "tenant-0040",  89 },
    { 141, "tenant-0041",  96 },
    { 142, "tenant-0042",   6 },
    { 143, "tenant-0043",  13 },
    { 144, "tenant-0044",  20 },
    { 145, "tenant-0045",  27 },
    { 146, "tenant-0046",  34 },
    { 147, "tenant-0047",  41 },
    { 148, "tenant-0048",  48 },
};

static struct tenant tenant_table[TENANT_SLOTS];
static int tenant_used;

void tenant_reset(void)
{
    memset(tenant_table, 0, sizeof(tenant_table));
    tenant_used = 0;
}

int tenant_add(int key, const char *name, int quota)
{
    struct tenant *slot;

    if (tenant_used >= TENANT_SLOTS) {
        return -1;
    }

    slot = &tenant_table[tenant_used];
    slot->key = key;
    slot->quota = quota;
    strncpy(slot->name, name, TENANT_NAME_SIZE - 1);
    slot->name[TENANT_NAME_SIZE - 1] = '\0';
    tenant_used++;

    return tenant_used - 1;
}

struct tenant *tenant_find(int key)
{
    int i;

    for (i = 0; i < tenant_used; i++) {
        if (tenant_table[i].key == key) {
            return &tenant_table[i];
        }
    }

    return NULL;
}

int tenant_remove(int key)
{
    struct tenant *found;

    found = tenant_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = tenant_table[tenant_used - 1];
    tenant_used--;

    return 0;
}

int tenant_total_quota(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < tenant_used; i++) {
        total += tenant_table[i].quota;
    }

    return total;
}

void tenant_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(tenant_seed) / sizeof(tenant_seed[0]); i++) {
        tenant_add(tenant_seed[i].key, tenant_seed[i].name,
                    tenant_seed[i].quota);
    }
}

void tenant_report(FILE *out)
{
    int i;

    fprintf(out, "%d tenants\n", tenant_used);
    for (i = 0; i < tenant_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", tenant_table[i].key,
                tenant_table[i].name, tenant_table[i].quota);
    }
    fprintf(out, "  total quota %d\n", tenant_total_quota());
}

int main(void)
{
    tenant_reset();
    tenant_load_seed();
    tenant_remove(103);
    tenant_report(stdout);

    return 0;
}
