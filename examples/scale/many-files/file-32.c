/*
 * A fixed size checkpoint table.
 *
 * It holds up to CHECKPOINT_SLOTS checkpoints, each with a key, a short name, and
 * a lag. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define CHECKPOINT_SLOTS 96
#define CHECKPOINT_NAME_SIZE 24

struct checkpoint {
    int key;
    char name[CHECKPOINT_NAME_SIZE];
    int lag;
};

static const struct checkpoint checkpoint_seed[] = {
    { 101, "checkpoint-0001",  10 },
    { 102, "checkpoint-0002",  17 },
    { 103, "checkpoint-0003",  24 },
    { 104, "checkpoint-0004",  31 },
    { 105, "checkpoint-0005",  38 },
    { 106, "checkpoint-0006",  45 },
    { 107, "checkpoint-0007",  52 },
    { 108, "checkpoint-0008",  59 },
    { 109, "checkpoint-0009",  66 },
    { 110, "checkpoint-0010",  73 },
    { 111, "checkpoint-0011",  80 },
    { 112, "checkpoint-0012",  87 },
    { 113, "checkpoint-0013",  94 },
    { 114, "checkpoint-0014",   4 },
    { 115, "checkpoint-0015",  11 },
    { 116, "checkpoint-0016",  18 },
    { 117, "checkpoint-0017",  25 },
    { 118, "checkpoint-0018",  32 },
    { 119, "checkpoint-0019",  39 },
    { 120, "checkpoint-0020",  46 },
    { 121, "checkpoint-0021",  53 },
    { 122, "checkpoint-0022",  60 },
    { 123, "checkpoint-0023",  67 },
    { 124, "checkpoint-0024",  74 },
    { 125, "checkpoint-0025",  81 },
    { 126, "checkpoint-0026",  88 },
    { 127, "checkpoint-0027",  95 },
    { 128, "checkpoint-0028",   5 },
    { 129, "checkpoint-0029",  12 },
    { 130, "checkpoint-0030",  19 },
    { 131, "checkpoint-0031",  26 },
    { 132, "checkpoint-0032",  33 },
    { 133, "checkpoint-0033",  40 },
    { 134, "checkpoint-0034",  47 },
    { 135, "checkpoint-0035",  54 },
    { 136, "checkpoint-0036",  61 },
    { 137, "checkpoint-0037",  68 },
    { 138, "checkpoint-0038",  75 },
    { 139, "checkpoint-0039",  82 },
    { 140, "checkpoint-0040",  89 },
    { 141, "checkpoint-0041",  96 },
    { 142, "checkpoint-0042",   6 },
    { 143, "checkpoint-0043",  13 },
    { 144, "checkpoint-0044",  20 },
    { 145, "checkpoint-0045",  27 },
    { 146, "checkpoint-0046",  34 },
    { 147, "checkpoint-0047",  41 },
    { 148, "checkpoint-0048",  48 },
};

static struct checkpoint checkpoint_table[CHECKPOINT_SLOTS];
static int checkpoint_used;

void checkpoint_reset(void)
{
    memset(checkpoint_table, 0, sizeof(checkpoint_table));
    checkpoint_used = 0;
}

int checkpoint_add(int key, const char *name, int lag)
{
    struct checkpoint *slot;

    if (checkpoint_used >= CHECKPOINT_SLOTS) {
        return -1;
    }

    slot = &checkpoint_table[checkpoint_used];
    slot->key = key;
    slot->lag = lag;
    strncpy(slot->name, name, CHECKPOINT_NAME_SIZE - 1);
    slot->name[CHECKPOINT_NAME_SIZE - 1] = '\0';
    checkpoint_used++;

    return checkpoint_used - 1;
}

struct checkpoint *checkpoint_find(int key)
{
    int i;

    for (i = 0; i < checkpoint_used; i++) {
        if (checkpoint_table[i].key == key) {
            return &checkpoint_table[i];
        }
    }

    return NULL;
}

int checkpoint_remove(int key)
{
    struct checkpoint *found;

    found = checkpoint_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = checkpoint_table[checkpoint_used - 1];
    checkpoint_used--;

    return 0;
}

int checkpoint_total_lag(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < checkpoint_used; i++) {
        total += checkpoint_table[i].lag;
    }

    return total;
}

void checkpoint_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(checkpoint_seed) / sizeof(checkpoint_seed[0]); i++) {
        checkpoint_add(checkpoint_seed[i].key, checkpoint_seed[i].name,
                    checkpoint_seed[i].lag);
    }
}

void checkpoint_report(FILE *out)
{
    int i;

    fprintf(out, "%d checkpoints\n", checkpoint_used);
    for (i = 0; i < checkpoint_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", checkpoint_table[i].key,
                checkpoint_table[i].name, checkpoint_table[i].lag);
    }
    fprintf(out, "  total lag %d\n", checkpoint_total_lag());
}

int main(void)
{
    checkpoint_reset();
    checkpoint_load_seed();
    checkpoint_remove(103);
    checkpoint_report(stdout);

    return 0;
}
