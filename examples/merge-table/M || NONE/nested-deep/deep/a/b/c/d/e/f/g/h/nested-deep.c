/*
 * A fixed size register table.
 *
 * It holds up to REGISTER_SLOTS registers, each with a key, a short name, and
 * a entries. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define REGISTER_SLOTS 64
#define REGISTER_NAME_SIZE 24

struct register {
    int key;
    char name[REGISTER_NAME_SIZE];
    int entries;
};

static const struct register register_seed[] = {
    { 101, "register-0001",  10 },
    { 102, "register-0002",  17 },
    { 103, "register-0003",  24 },
    { 104, "register-0004",  31 },
    { 105, "register-0005",  38 },
    { 106, "register-0006",  45 },
    { 107, "register-0007",  52 },
    { 108, "register-0008",  59 },
    { 109, "register-0009",  66 },
    { 110, "register-0010",  73 },
    { 111, "register-0011",  80 },
    { 112, "register-0012",  87 },
    { 113, "register-0013",  94 },
    { 114, "register-0014",   4 },
    { 115, "register-0015",  11 },
    { 116, "register-0016",  18 },
    { 117, "register-0017",  25 },
    { 118, "register-0018",  32 },
    { 119, "register-0019",  39 },
    { 120, "register-0020",  46 },
    { 121, "register-0021",  53 },
    { 122, "register-0022",  60 },
    { 123, "register-0023",  67 },
    { 124, "register-0024",  74 },
    { 125, "register-0025",  81 },
    { 126, "register-0026",  88 },
    { 127, "register-0027",  95 },
    { 128, "register-0028",   5 },
    { 129, "register-0029",  12 },
    { 130, "register-0030",  19 },
    { 131, "register-0031",  26 },
    { 132, "register-0032",  33 },
    { 133, "register-0033",  40 },
    { 134, "register-0034",  47 },
    { 135, "register-0035",  54 },
    { 136, "register-0036",  61 },
    { 137, "register-0037",  68 },
    { 138, "register-0038",  75 },
    { 139, "register-0039",  82 },
    { 140, "register-0040",  89 },
    { 141, "register-0041",  96 },
    { 142, "register-0042",   6 },
    { 143, "register-0043",  13 },
    { 144, "register-0044",  20 },
    { 145, "register-0045",  27 },
    { 146, "register-0046",  34 },
    { 147, "register-0047",  41 },
    { 148, "register-0048",  48 },
};

static struct register register_table[REGISTER_SLOTS];
static int register_used;

void register_reset(void)
{
    memset(register_table, 0, sizeof(register_table));
    register_used = 0;
}

int register_add(int key, const char *name, int entries)
{
    struct register *slot;

    if (register_used >= REGISTER_SLOTS) {
        return -1;
    }

    slot = &register_table[register_used];
    slot->key = key;
    slot->entries = entries;
    strncpy(slot->name, name, REGISTER_NAME_SIZE - 1);
    slot->name[REGISTER_NAME_SIZE - 1] = '\0';
    register_used++;

    return register_used - 1;
}

struct register *register_find(int key)
{
    int i;

    for (i = 0; i < register_used; i++) {
        if (register_table[i].key == key) {
            return &register_table[i];
        }
    }

    return NULL;
}

int register_remove(int key)
{
    struct register *found;

    found = register_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = register_table[register_used - 1];
    register_used--;

    return 0;
}

int register_total_entries(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < register_used; i++) {
        total += register_table[i].entries;
    }

    return total;
}

void register_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(register_seed) / sizeof(register_seed[0]); i++) {
        register_add(register_seed[i].key, register_seed[i].name,
                    register_seed[i].entries);
    }
}

void register_report(FILE *out)
{
    int i;

    fprintf(out, "%d registers\n", register_used);
    for (i = 0; i < register_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", register_table[i].key,
                register_table[i].name, register_table[i].entries);
    }
    fprintf(out, "  total entries %d\n", register_total_entries());
}

int main(void)
{
    register_reset();
    register_load_seed();
    register_remove(103);
    register_report(stdout);

    return 0;
}
