/*
 * A fixed size terminal table.
 *
 * It holds up to TERMINAL_SLOTS terminals, each with a key, a short name, and
 * a lanes. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define TERMINAL_SLOTS 64
#define TERMINAL_NAME_SIZE 24

struct terminal {
    int key;
    char name[TERMINAL_NAME_SIZE];
    int lanes;
};

static const struct terminal terminal_seed[] = {
    { 101, "terminal-0001",  10 },
    { 102, "terminal-0002",  17 },
    { 103, "terminal-0003",  24 },
    { 104, "terminal-0004",  31 },
    { 105, "terminal-0005",  38 },
    { 106, "terminal-0006",  45 },
    { 107, "terminal-0007",  52 },
    { 108, "terminal-0008",  59 },
    { 109, "terminal-0009",  66 },
    { 110, "terminal-0010",  73 },
    { 111, "terminal-0011",  80 },
    { 112, "terminal-0012",  87 },
    { 113, "terminal-0013",  94 },
    { 114, "terminal-0014",   4 },
    { 115, "terminal-0015",  11 },
    { 116, "terminal-0016",  18 },
    { 117, "terminal-0017",  25 },
    { 118, "terminal-0018",  32 },
    { 119, "terminal-0019",  39 },
    { 120, "terminal-0020",  46 },
    { 121, "terminal-0021",  53 },
    { 122, "terminal-0022",  60 },
    { 123, "terminal-0023",  67 },
    { 124, "terminal-0024",  74 },
    { 125, "terminal-0025",  81 },
    { 126, "terminal-0026",  88 },
    { 127, "terminal-0027",  95 },
    { 128, "terminal-0028",   5 },
    { 129, "terminal-0029",  12 },
    { 130, "terminal-0030",  19 },
    { 131, "terminal-0031",  26 },
    { 132, "terminal-0032",  33 },
    { 133, "terminal-0033",  40 },
    { 134, "terminal-0034",  47 },
    { 135, "terminal-0035",  54 },
    { 136, "terminal-0036",  61 },
    { 137, "terminal-0037",  68 },
    { 138, "terminal-0038",  75 },
    { 139, "terminal-0039",  82 },
    { 140, "terminal-0040",  89 },
    { 141, "terminal-0041",  96 },
    { 142, "terminal-0042",   6 },
    { 143, "terminal-0043",  13 },
    { 144, "terminal-0044",  20 },
    { 145, "terminal-0045",  27 },
    { 146, "terminal-0046",  34 },
    { 147, "terminal-0047",  41 },
    { 148, "terminal-0048",  48 },
};

static struct terminal terminal_table[TERMINAL_SLOTS];
static int terminal_used;

void terminal_reset(void)
{
    memset(terminal_table, 0, sizeof(terminal_table));
    terminal_used = 0;
}

int terminal_add(int key, const char *name, int lanes)
{
    struct terminal *slot;

    if (terminal_used >= TERMINAL_SLOTS) {
        return -1;
    }

    slot = &terminal_table[terminal_used];
    slot->key = key;
    slot->lanes = lanes;
    strncpy(slot->name, name, TERMINAL_NAME_SIZE - 1);
    slot->name[TERMINAL_NAME_SIZE - 1] = '\0';
    terminal_used++;

    return terminal_used - 1;
}

struct terminal *terminal_find(int key)
{
    int i;

    for (i = 0; i < terminal_used; i++) {
        if (terminal_table[i].key == key) {
            return &terminal_table[i];
        }
    }

    return NULL;
}

int terminal_remove(int key)
{
    struct terminal *found;

    found = terminal_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = terminal_table[terminal_used - 1];
    terminal_used--;

    return 0;
}

int terminal_total_lanes(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < terminal_used; i++) {
        total += terminal_table[i].lanes;
    }

    return total;
}

void terminal_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(terminal_seed) / sizeof(terminal_seed[0]); i++) {
        terminal_add(terminal_seed[i].key, terminal_seed[i].name,
                    terminal_seed[i].lanes);
    }
}

void terminal_report(FILE *out)
{
    int i;

    fprintf(out, "%d terminals\n", terminal_used);
    for (i = 0; i < terminal_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", terminal_table[i].key,
                terminal_table[i].name, terminal_table[i].lanes);
    }
    fprintf(out, "  total lanes %d\n", terminal_total_lanes());
}

int main(void)
{
    terminal_reset();
    terminal_load_seed();
    terminal_remove(103);
    terminal_report(stdout);

    return 0;
}