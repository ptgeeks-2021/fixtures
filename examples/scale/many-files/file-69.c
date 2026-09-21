/*
 * A fixed size funicular table.
 *
 * It holds up to FUNICULAR_SLOTS funiculars, each with a key, a short name, and
 * a cabins. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define FUNICULAR_SLOTS 64
#define FUNICULAR_NAME_SIZE 24

struct funicular {
    int key;
    char name[FUNICULAR_NAME_SIZE];
    int cabins;
};

static const struct funicular funicular_seed[] = {
    { 101, "funicular-0001",  10 },
    { 102, "funicular-0002",  17 },
    { 103, "funicular-0003",  24 },
    { 104, "funicular-0004",  31 },
    { 105, "funicular-0005",  38 },
    { 106, "funicular-0006",  45 },
    { 107, "funicular-0007",  52 },
    { 108, "funicular-0008",  59 },
    { 109, "funicular-0009",  66 },
    { 110, "funicular-0010",  73 },
    { 111, "funicular-0011",  80 },
    { 112, "funicular-0012",  87 },
    { 113, "funicular-0013",  94 },
    { 114, "funicular-0014",   4 },
    { 115, "funicular-0015",  11 },
    { 116, "funicular-0016",  18 },
    { 117, "funicular-0017",  25 },
    { 118, "funicular-0018",  32 },
    { 119, "funicular-0019",  39 },
    { 120, "funicular-0020",  46 },
    { 121, "funicular-0021",  53 },
    { 122, "funicular-0022",  60 },
    { 123, "funicular-0023",  67 },
    { 124, "funicular-0024",  74 },
    { 125, "funicular-0025",  81 },
    { 126, "funicular-0026",  88 },
    { 127, "funicular-0027",  95 },
    { 128, "funicular-0028",   5 },
    { 129, "funicular-0029",  12 },
    { 130, "funicular-0030",  19 },
    { 131, "funicular-0031",  26 },
    { 132, "funicular-0032",  33 },
    { 133, "funicular-0033",  40 },
    { 134, "funicular-0034",  47 },
    { 135, "funicular-0035",  54 },
    { 136, "funicular-0036",  61 },
    { 137, "funicular-0037",  68 },
    { 138, "funicular-0038",  75 },
    { 139, "funicular-0039",  82 },
    { 140, "funicular-0040",  89 },
    { 141, "funicular-0041",  96 },
    { 142, "funicular-0042",   6 },
    { 143, "funicular-0043",  13 },
    { 144, "funicular-0044",  20 },
    { 145, "funicular-0045",  27 },
    { 146, "funicular-0046",  34 },
    { 147, "funicular-0047",  41 },
    { 148, "funicular-0048",  48 },
};

static struct funicular funicular_table[FUNICULAR_SLOTS];
static int funicular_used;

void funicular_reset(void)
{
    memset(funicular_table, 0, sizeof(funicular_table));
    funicular_used = 0;
}

int funicular_add(int key, const char *name, int cabins)
{
    struct funicular *slot;

    if (funicular_used >= FUNICULAR_SLOTS) {
        return -1;
    }

    slot = &funicular_table[funicular_used];
    slot->key = key;
    slot->cabins = cabins;
    strncpy(slot->name, name, FUNICULAR_NAME_SIZE - 1);
    slot->name[FUNICULAR_NAME_SIZE - 1] = '\0';
    funicular_used++;

    return funicular_used - 1;
}

struct funicular *funicular_find(int key)
{
    int i;

    for (i = 0; i < funicular_used; i++) {
        if (funicular_table[i].key == key) {
            return &funicular_table[i];
        }
    }

    return NULL;
}

int funicular_remove(int key)
{
    struct funicular *found;

    found = funicular_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = funicular_table[funicular_used - 1];
    funicular_used--;

    return 0;
}

int funicular_total_cabins(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < funicular_used; i++) {
        total += funicular_table[i].cabins;
    }

    return total;
}

void funicular_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(funicular_seed) / sizeof(funicular_seed[0]); i++) {
        funicular_add(funicular_seed[i].key, funicular_seed[i].name,
                    funicular_seed[i].cabins);
    }
}

void funicular_report(FILE *out)
{
    int i;

    fprintf(out, "%d funiculars\n", funicular_used);
    for (i = 0; i < funicular_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", funicular_table[i].key,
                funicular_table[i].name, funicular_table[i].cabins);
    }
    fprintf(out, "  total cabins %d\n", funicular_total_cabins());
}

int main(void)
{
    funicular_reset();
    funicular_load_seed();
    funicular_remove(103);
    funicular_report(stdout);

    return 0;
}
