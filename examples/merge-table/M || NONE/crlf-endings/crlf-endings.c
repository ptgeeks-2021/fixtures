/*
 * A fixed size reactor table.
 *
 * It holds up to REACTOR_SLOTS reactors, each with a key, a short name, and
 * a pressure. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define REACTOR_SLOTS 64
#define REACTOR_NAME_SIZE 24

struct reactor {
    int key;
    char name[REACTOR_NAME_SIZE];
    int pressure;
};

static const struct reactor reactor_seed[] = {
    { 101, "reactor-0001",  10 },
    { 102, "reactor-0002",  17 },
    { 103, "reactor-0003",  24 },
    { 104, "reactor-0004",  31 },
    { 105, "reactor-0005",  38 },
    { 106, "reactor-0006",  45 },
    { 107, "reactor-0007",  52 },
    { 108, "reactor-0008",  59 },
    { 109, "reactor-0009",  66 },
    { 110, "reactor-0010",  73 },
    { 111, "reactor-0011",  80 },
    { 112, "reactor-0012",  87 },
    { 113, "reactor-0013",  94 },
    { 114, "reactor-0014",   4 },
    { 115, "reactor-0015",  11 },
    { 116, "reactor-0016",  18 },
    { 117, "reactor-0017",  25 },
    { 118, "reactor-0018",  32 },
    { 119, "reactor-0019",  39 },
    { 120, "reactor-0020",  46 },
    { 121, "reactor-0021",  53 },
    { 122, "reactor-0022",  60 },
    { 123, "reactor-0023",  67 },
    { 124, "reactor-0024",  74 },
    { 125, "reactor-0025",  81 },
    { 126, "reactor-0026",  88 },
    { 127, "reactor-0027",  95 },
    { 128, "reactor-0028",   5 },
    { 129, "reactor-0029",  12 },
    { 130, "reactor-0030",  19 },
    { 131, "reactor-0031",  26 },
    { 132, "reactor-0032",  33 },
    { 133, "reactor-0033",  40 },
    { 134, "reactor-0034",  47 },
    { 135, "reactor-0035",  54 },
    { 136, "reactor-0036",  61 },
    { 137, "reactor-0037",  68 },
    { 138, "reactor-0038",  75 },
    { 139, "reactor-0039",  82 },
    { 140, "reactor-0040",  89 },
    { 141, "reactor-0041",  96 },
    { 142, "reactor-0042",   6 },
    { 143, "reactor-0043",  13 },
    { 144, "reactor-0044",  20 },
    { 145, "reactor-0045",  27 },
    { 146, "reactor-0046",  34 },
    { 147, "reactor-0047",  41 },
    { 148, "reactor-0048",  48 },
};

static struct reactor reactor_table[REACTOR_SLOTS];
static int reactor_used;

void reactor_reset(void)
{
    memset(reactor_table, 0, sizeof(reactor_table));
    reactor_used = 0;
}

int reactor_add(int key, const char *name, int pressure)
{
    struct reactor *slot;

    if (reactor_used >= REACTOR_SLOTS) {
        return -1;
    }

    slot = &reactor_table[reactor_used];
    slot->key = key;
    slot->pressure = pressure;
    strncpy(slot->name, name, REACTOR_NAME_SIZE - 1);
    slot->name[REACTOR_NAME_SIZE - 1] = '\0';
    reactor_used++;

    return reactor_used - 1;
}

struct reactor *reactor_find(int key)
{
    int i;

    for (i = 0; i < reactor_used; i++) {
        if (reactor_table[i].key == key) {
            return &reactor_table[i];
        }
    }

    return NULL;
}

int reactor_remove(int key)
{
    struct reactor *found;

    found = reactor_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = reactor_table[reactor_used - 1];
    reactor_used--;

    return 0;
}

int reactor_total_pressure(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < reactor_used; i++) {
        total += reactor_table[i].pressure;
    }

    return total;
}

void reactor_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(reactor_seed) / sizeof(reactor_seed[0]); i++) {
        reactor_add(reactor_seed[i].key, reactor_seed[i].name,
                    reactor_seed[i].pressure);
    }
}

void reactor_report(FILE *out)
{
    int i;

    fprintf(out, "%d reactors\n", reactor_used);
    for (i = 0; i < reactor_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", reactor_table[i].key,
                reactor_table[i].name, reactor_table[i].pressure);
    }
    fprintf(out, "  total pressure %d\n", reactor_total_pressure());
}

int main(void)
{
    reactor_reset();
    reactor_load_seed();
    reactor_remove(103);
    reactor_report(stdout);

    return 0;
}
