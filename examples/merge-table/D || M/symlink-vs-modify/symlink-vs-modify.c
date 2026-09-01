/*
 * A fixed size cluster table.
 *
 * It holds up to CLUSTER_SLOTS clusters, each with a key, a short name, and
 * a nodes. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define CLUSTER_SLOTS 64
#define CLUSTER_NAME_SIZE 24

struct cluster {
    int key;
    char name[CLUSTER_NAME_SIZE];
    int nodes;
};

static const struct cluster cluster_seed[] = {
    { 101, "cluster-0001",  10 },
    { 102, "cluster-0002",  17 },
    { 103, "cluster-0003",  24 },
    { 104, "cluster-0004",  31 },
    { 105, "cluster-0005",  38 },
    { 106, "cluster-0006",  45 },
    { 107, "cluster-0007",  52 },
    { 108, "cluster-0008",  59 },
    { 109, "cluster-0009",  66 },
    { 110, "cluster-0010",  73 },
    { 111, "cluster-0011",  80 },
    { 112, "cluster-0012",  87 },
    { 113, "cluster-0013",  94 },
    { 114, "cluster-0014",   4 },
    { 115, "cluster-0015",  11 },
    { 116, "cluster-0016",  18 },
    { 117, "cluster-0017",  25 },
    { 118, "cluster-0018",  32 },
    { 119, "cluster-0019",  39 },
    { 120, "cluster-0020",  46 },
    { 121, "cluster-0021",  53 },
    { 122, "cluster-0022",  60 },
    { 123, "cluster-0023",  67 },
    { 124, "cluster-0024",  74 },
    { 125, "cluster-0025",  81 },
    { 126, "cluster-0026",  88 },
    { 127, "cluster-0027",  95 },
    { 128, "cluster-0028",   5 },
    { 129, "cluster-0029",  12 },
    { 130, "cluster-0030",  19 },
    { 131, "cluster-0031",  26 },
    { 132, "cluster-0032",  33 },
    { 133, "cluster-0033",  40 },
    { 134, "cluster-0034",  47 },
    { 135, "cluster-0035",  54 },
    { 136, "cluster-0036",  61 },
    { 137, "cluster-0037",  68 },
    { 138, "cluster-0038",  75 },
    { 139, "cluster-0039",  82 },
    { 140, "cluster-0040",  89 },
    { 141, "cluster-0041",  96 },
    { 142, "cluster-0042",   6 },
    { 143, "cluster-0043",  13 },
    { 144, "cluster-0044",  20 },
    { 145, "cluster-0045",  27 },
    { 146, "cluster-0046",  34 },
    { 147, "cluster-0047",  41 },
    { 148, "cluster-0048",  48 },
};

static struct cluster cluster_table[CLUSTER_SLOTS];
static int cluster_used;

void cluster_reset(void)
{
    memset(cluster_table, 0, sizeof(cluster_table));
    cluster_used = 0;
}

int cluster_add(int key, const char *name, int nodes)
{
    struct cluster *slot;

    if (cluster_used >= CLUSTER_SLOTS) {
        return -1;
    }

    slot = &cluster_table[cluster_used];
    slot->key = key;
    slot->nodes = nodes;
    strncpy(slot->name, name, CLUSTER_NAME_SIZE - 1);
    slot->name[CLUSTER_NAME_SIZE - 1] = '\0';
    cluster_used++;

    return cluster_used - 1;
}

struct cluster *cluster_find(int key)
{
    int i;

    for (i = 0; i < cluster_used; i++) {
        if (cluster_table[i].key == key) {
            return &cluster_table[i];
        }
    }

    return NULL;
}

int cluster_remove(int key)
{
    struct cluster *found;

    found = cluster_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = cluster_table[cluster_used - 1];
    cluster_used--;

    return 0;
}

int cluster_total_nodes(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < cluster_used; i++) {
        total += cluster_table[i].nodes;
    }

    return total;
}

void cluster_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(cluster_seed) / sizeof(cluster_seed[0]); i++) {
        cluster_add(cluster_seed[i].key, cluster_seed[i].name,
                    cluster_seed[i].nodes);
    }
}

void cluster_report(FILE *out)
{
    int i;

    fprintf(out, "%d clusters\n", cluster_used);
    for (i = 0; i < cluster_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", cluster_table[i].key,
                cluster_table[i].name, cluster_table[i].nodes);
    }
    fprintf(out, "  total nodes %d\n", cluster_total_nodes());
}

int main(void)
{
    cluster_reset();
    cluster_load_seed();
    cluster_remove(103);
    cluster_report(stdout);

    return 0;
}
