/*
 * A fixed size drawbridge table.
 *
 * It holds up to DRAWBRIDGE_SLOTS drawbridges, each with a key, a short name, and
 * a spans. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define DRAWBRIDGE_SLOTS 96
#define DRAWBRIDGE_NAME_SIZE 24

struct drawbridge {
    int key;
    char name[DRAWBRIDGE_NAME_SIZE];
    int spans;
};

static const struct drawbridge drawbridge_seed[] = {
    { 101, "drawbridge-0001",  10 },
    { 102, "drawbridge-0002",  17 },
    { 103, "drawbridge-0003",  24 },
    { 104, "drawbridge-0004",  31 },
    { 105, "drawbridge-0005",  38 },
    { 106, "drawbridge-0006",  45 },
    { 107, "drawbridge-0007",  52 },
    { 108, "drawbridge-0008",  59 },
    { 109, "drawbridge-0009",  66 },
    { 110, "drawbridge-0010",  73 },
    { 111, "drawbridge-0011",  80 },
    { 112, "drawbridge-0012",  87 },
    { 113, "drawbridge-0013",  94 },
    { 114, "drawbridge-0014",   4 },
    { 115, "drawbridge-0015",  11 },
    { 116, "drawbridge-0016",  18 },
    { 117, "drawbridge-0017",  25 },
    { 118, "drawbridge-0018",  32 },
    { 119, "drawbridge-0019",  39 },
    { 120, "drawbridge-0020",  46 },
    { 121, "drawbridge-0021",  53 },
    { 122, "drawbridge-0022",  60 },
    { 123, "drawbridge-0023",  67 },
    { 124, "drawbridge-0024",  74 },
    { 125, "drawbridge-0025",  81 },
    { 126, "drawbridge-0026",  88 },
    { 127, "drawbridge-0027",  95 },
    { 128, "drawbridge-0028",   5 },
    { 129, "drawbridge-0029",  12 },
    { 130, "drawbridge-0030",  19 },
    { 131, "drawbridge-0031",  26 },
    { 132, "drawbridge-0032",  33 },
    { 133, "drawbridge-0033",  40 },
    { 134, "drawbridge-0034",  47 },
    { 135, "drawbridge-0035",  54 },
    { 136, "drawbridge-0036",  61 },
    { 137, "drawbridge-0037",  68 },
    { 138, "drawbridge-0038",  75 },
    { 139, "drawbridge-0039",  82 },
    { 140, "drawbridge-0040",  89 },
    { 141, "drawbridge-0041",  96 },
    { 142, "drawbridge-0042",   6 },
    { 143, "drawbridge-0043",  13 },
    { 144, "drawbridge-0044",  20 },
    { 145, "drawbridge-0045",  27 },
    { 146, "drawbridge-0046",  34 },
    { 147, "drawbridge-0047",  41 },
    { 148, "drawbridge-0048",  48 },
};

static struct drawbridge drawbridge_table[DRAWBRIDGE_SLOTS];
static int drawbridge_used;

void drawbridge_reset(void)
{
    memset(drawbridge_table, 0, sizeof(drawbridge_table));
    drawbridge_used = 0;
}

int drawbridge_add(int key, const char *name, int spans)
{
    struct drawbridge *slot;

    if (drawbridge_used >= DRAWBRIDGE_SLOTS) {
        return -1;
    }

    slot = &drawbridge_table[drawbridge_used];
    slot->key = key;
    slot->spans = spans;
    strncpy(slot->name, name, DRAWBRIDGE_NAME_SIZE - 1);
    slot->name[DRAWBRIDGE_NAME_SIZE - 1] = '\0';
    drawbridge_used++;

    return drawbridge_used - 1;
}

struct drawbridge *drawbridge_find(int key)
{
    int i;

    for (i = 0; i < drawbridge_used; i++) {
        if (drawbridge_table[i].key == key) {
            return &drawbridge_table[i];
        }
    }

    return NULL;
}

int drawbridge_remove(int key)
{
    struct drawbridge *found;

    found = drawbridge_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = drawbridge_table[drawbridge_used - 1];
    drawbridge_used--;

    return 0;
}

int drawbridge_total_spans(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < drawbridge_used; i++) {
        total += drawbridge_table[i].spans;
    }

    return total;
}

void drawbridge_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(drawbridge_seed) / sizeof(drawbridge_seed[0]); i++) {
        drawbridge_add(drawbridge_seed[i].key, drawbridge_seed[i].name,
                    drawbridge_seed[i].spans);
    }
}

void drawbridge_report(FILE *out)
{
    int i;

    fprintf(out, "%d drawbridges\n", drawbridge_used);
    for (i = 0; i < drawbridge_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", drawbridge_table[i].key,
                drawbridge_table[i].name, drawbridge_table[i].spans);
    }
    fprintf(out, "  total spans %d\n", drawbridge_total_spans());
}

int main(void)
{
    drawbridge_reset();
    drawbridge_load_seed();
    drawbridge_remove(103);
    drawbridge_report(stdout);

    return 0;
}
