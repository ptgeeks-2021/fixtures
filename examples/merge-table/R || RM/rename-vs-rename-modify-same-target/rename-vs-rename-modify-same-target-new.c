/*
 * A fixed size router table.
 *
 * It holds up to ROUTER_SLOTS routers, each with a key, a short name, and
 * a packets. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define ROUTER_SLOTS 128
#define ROUTER_NAME_SIZE 24

struct router {
    int key;
    char name[ROUTER_NAME_SIZE];
    int packets;
};

static const struct router router_seed[] = {
    { 101, "router-0001",  10 },
    { 102, "router-0002",  17 },
    { 103, "router-0003",  24 },
    { 104, "router-0004",  31 },
    { 105, "router-0005",  38 },
    { 106, "router-0006",  45 },
    { 107, "router-0007",  52 },
    { 108, "router-0008",  59 },
    { 109, "router-0009",  66 },
    { 110, "router-0010",  73 },
    { 111, "router-0011",  80 },
    { 112, "router-0012",  87 },
    { 113, "router-0013",  94 },
    { 114, "router-0014",   4 },
    { 115, "router-0015",  11 },
    { 116, "router-0016",  18 },
    { 117, "router-0017",  25 },
    { 118, "router-0018",  32 },
    { 119, "router-0019",  39 },
    { 120, "router-0020",  46 },
    { 121, "router-0021",  53 },
    { 122, "router-0022",  60 },
    { 123, "router-0023",  67 },
    { 124, "router-0024",  74 },
    { 125, "router-0025",  81 },
    { 126, "router-0026",  88 },
    { 127, "router-0027",  95 },
    { 128, "router-0028",   5 },
    { 129, "router-0029",  12 },
    { 130, "router-0030",  19 },
    { 131, "router-0031",  26 },
    { 132, "router-0032",  33 },
    { 133, "router-0033",  40 },
    { 134, "router-0034",  47 },
    { 135, "router-0035",  54 },
    { 136, "router-0036",  61 },
    { 137, "router-0037",  68 },
    { 138, "router-0038",  75 },
    { 139, "router-0039",  82 },
    { 140, "router-0040",  89 },
    { 141, "router-0041",  96 },
    { 142, "router-0042",   6 },
    { 143, "router-0043",  13 },
    { 144, "router-0044",  20 },
    { 145, "router-0045",  27 },
    { 146, "router-0046",  34 },
    { 147, "router-0047",  41 },
    { 148, "router-0048",  48 },
};

static struct router router_table[ROUTER_SLOTS];
static int router_used;

void router_reset(void)
{
    memset(router_table, 0, sizeof(router_table));
    router_used = 0;
}

int router_add(int key, const char *name, int packets)
{
    struct router *slot;

    if (router_used >= ROUTER_SLOTS) {
        return -1;
    }

    slot = &router_table[router_used];
    slot->key = key;
    slot->packets = packets;
    strncpy(slot->name, name, ROUTER_NAME_SIZE - 1);
    slot->name[ROUTER_NAME_SIZE - 1] = '\0';
    router_used++;

    return router_used - 1;
}

struct router *router_find(int key)
{
    int i;

    for (i = 0; i < router_used; i++) {
        if (router_table[i].key == key) {
            return &router_table[i];
        }
    }

    return NULL;
}

int router_remove(int key)
{
    struct router *found;

    found = router_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = router_table[router_used - 1];
    router_used--;

    return 0;
}

int router_total_packets(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < router_used; i++) {
        total += router_table[i].packets;
    }

    return total;
}

void router_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(router_seed) / sizeof(router_seed[0]); i++) {
        router_add(router_seed[i].key, router_seed[i].name,
                    router_seed[i].packets);
    }
}

void router_report(FILE *out)
{
    int i;

    fprintf(out, "%d routers\n", router_used);
    for (i = 0; i < router_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", router_table[i].key,
                router_table[i].name, router_table[i].packets);
    }
    fprintf(out, "  total packets %d\n", router_total_packets());
}

int main(void)
{
    router_reset();
    router_load_seed();
    router_remove(103);
    router_report(stdout);

    return 0;
}
