/*
 * A fixed size pipeline table.
 *
 * It holds up to PIPELINE_SLOTS pipelines, each with a key, a short name, and
 * a stages. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define PIPELINE_SLOTS 64
#define PIPELINE_NAME_SIZE 24

struct pipeline {
    int key;
    char name[PIPELINE_NAME_SIZE];
    int stages;
};

static const struct pipeline pipeline_seed[] = {
    { 101, "pipeline-0001",  10 },
    { 102, "pipeline-0002",  17 },
    { 103, "pipeline-0003",  24 },
    { 104, "pipeline-0004",  31 },
    { 105, "pipeline-0005",  38 },
    { 106, "pipeline-0006",  45 },
    { 107, "pipeline-0007",  52 },
    { 108, "pipeline-0008",  59 },
    { 109, "pipeline-0009",  66 },
    { 110, "pipeline-0010",  73 },
    { 111, "pipeline-0011",  80 },
    { 112, "pipeline-0012",  87 },
    { 113, "pipeline-0013",  94 },
    { 114, "pipeline-0014",   4 },
    { 115, "pipeline-0015",  11 },
    { 116, "pipeline-0016",  18 },
    { 117, "pipeline-0017",  25 },
    { 118, "pipeline-0018",  32 },
    { 119, "pipeline-0019",  39 },
    { 120, "pipeline-0020",  46 },
    { 121, "pipeline-0021",  53 },
    { 122, "pipeline-0022",  60 },
    { 123, "pipeline-0023",  67 },
    { 124, "pipeline-0024",  74 },
    { 125, "pipeline-0025",  81 },
    { 126, "pipeline-0026",  88 },
    { 127, "pipeline-0027",  95 },
    { 128, "pipeline-0028",   5 },
    { 129, "pipeline-0029",  12 },
    { 130, "pipeline-0030",  19 },
    { 131, "pipeline-0031",  26 },
    { 132, "pipeline-0032",  33 },
    { 133, "pipeline-0033",  40 },
    { 134, "pipeline-0034",  47 },
    { 135, "pipeline-0035",  54 },
    { 136, "pipeline-0036",  61 },
    { 137, "pipeline-0037",  68 },
    { 138, "pipeline-0038",  75 },
    { 139, "pipeline-0039",  82 },
    { 140, "pipeline-0040",  89 },
    { 141, "pipeline-0041",  96 },
    { 142, "pipeline-0042",   6 },
    { 143, "pipeline-0043",  13 },
    { 144, "pipeline-0044",  20 },
    { 145, "pipeline-0045",  27 },
    { 146, "pipeline-0046",  34 },
    { 147, "pipeline-0047",  41 },
    { 148, "pipeline-0048",  48 },
};

static struct pipeline pipeline_table[PIPELINE_SLOTS];
static int pipeline_used;

void pipeline_reset(void)
{
    memset(pipeline_table, 0, sizeof(pipeline_table));
    pipeline_used = 0;
}

int pipeline_add(int key, const char *name, int stages)
{
    struct pipeline *slot;

    if (pipeline_used >= PIPELINE_SLOTS) {
        return -1;
    }

    slot = &pipeline_table[pipeline_used];
    slot->key = key;
    slot->stages = stages;
    strncpy(slot->name, name, PIPELINE_NAME_SIZE - 1);
    slot->name[PIPELINE_NAME_SIZE - 1] = '\0';
    pipeline_used++;

    return pipeline_used - 1;
}

struct pipeline *pipeline_find(int key)
{
    int i;

    for (i = 0; i < pipeline_used; i++) {
        if (pipeline_table[i].key == key) {
            return &pipeline_table[i];
        }
    }

    return NULL;
}

int pipeline_remove(int key)
{
    struct pipeline *found;

    found = pipeline_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = pipeline_table[pipeline_used - 1];
    pipeline_used--;

    return 0;
}

int pipeline_total_stages(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < pipeline_used; i++) {
        total += pipeline_table[i].stages;
    }

    return total;
}

void pipeline_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(pipeline_seed) / sizeof(pipeline_seed[0]); i++) {
        pipeline_add(pipeline_seed[i].key, pipeline_seed[i].name,
                    pipeline_seed[i].stages);
    }
}

void pipeline_report(FILE *out)
{
    int i;

    fprintf(out, "%d pipelines\n", pipeline_used);
    for (i = 0; i < pipeline_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", pipeline_table[i].key,
                pipeline_table[i].name, pipeline_table[i].stages);
    }
    fprintf(out, "  total stages %d\n", pipeline_total_stages());
}

int main(void)
{
    pipeline_reset();
    pipeline_load_seed();
    pipeline_remove(103);
    pipeline_report(stdout);

    return 0;
}
