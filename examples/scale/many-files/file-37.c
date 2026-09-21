/*
 * A fixed size recipe table.
 *
 * It holds up to RECIPE_SLOTS recipes, each with a key, a short name, and
 * a servings. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define RECIPE_SLOTS 64
#define RECIPE_NAME_SIZE 24

struct recipe {
    int key;
    char name[RECIPE_NAME_SIZE];
    int servings;
};

static const struct recipe recipe_seed[] = {
    { 101, "recipe-0001",  10 },
    { 102, "recipe-0002",  17 },
    { 103, "recipe-0003",  24 },
    { 104, "recipe-0004",  31 },
    { 105, "recipe-0005",  38 },
    { 106, "recipe-0006",  45 },
    { 107, "recipe-0007",  52 },
    { 108, "recipe-0008",  59 },
    { 109, "recipe-0009",  66 },
    { 110, "recipe-0010",  73 },
    { 111, "recipe-0011",  80 },
    { 112, "recipe-0012",  87 },
    { 113, "recipe-0013",  94 },
    { 114, "recipe-0014",   4 },
    { 115, "recipe-0015",  11 },
    { 116, "recipe-0016",  18 },
    { 117, "recipe-0017",  25 },
    { 118, "recipe-0018",  32 },
    { 119, "recipe-0019",  39 },
    { 120, "recipe-0020",  46 },
    { 121, "recipe-0021",  53 },
    { 122, "recipe-0022",  60 },
    { 123, "recipe-0023",  67 },
    { 124, "recipe-0024",  74 },
    { 125, "recipe-0025",  81 },
    { 126, "recipe-0026",  88 },
    { 127, "recipe-0027",  95 },
    { 128, "recipe-0028",   5 },
    { 129, "recipe-0029",  12 },
    { 130, "recipe-0030",  19 },
    { 131, "recipe-0031",  26 },
    { 132, "recipe-0032",  33 },
    { 133, "recipe-0033",  40 },
    { 134, "recipe-0034",  47 },
    { 135, "recipe-0035",  54 },
    { 136, "recipe-0036",  61 },
    { 137, "recipe-0037",  68 },
    { 138, "recipe-0038",  75 },
    { 139, "recipe-0039",  82 },
    { 140, "recipe-0040",  89 },
    { 141, "recipe-0041",  96 },
    { 142, "recipe-0042",   6 },
    { 143, "recipe-0043",  13 },
    { 144, "recipe-0044",  20 },
    { 145, "recipe-0045",  27 },
    { 146, "recipe-0046",  34 },
    { 147, "recipe-0047",  41 },
    { 148, "recipe-0048",  48 },
};

static struct recipe recipe_table[RECIPE_SLOTS];
static int recipe_used;

void recipe_reset(void)
{
    memset(recipe_table, 0, sizeof(recipe_table));
    recipe_used = 0;
}

int recipe_add(int key, const char *name, int servings)
{
    struct recipe *slot;

    if (recipe_used >= RECIPE_SLOTS) {
        return -1;
    }

    slot = &recipe_table[recipe_used];
    slot->key = key;
    slot->servings = servings;
    strncpy(slot->name, name, RECIPE_NAME_SIZE - 1);
    slot->name[RECIPE_NAME_SIZE - 1] = '\0';
    recipe_used++;

    return recipe_used - 1;
}

struct recipe *recipe_find(int key)
{
    int i;

    for (i = 0; i < recipe_used; i++) {
        if (recipe_table[i].key == key) {
            return &recipe_table[i];
        }
    }

    return NULL;
}

int recipe_remove(int key)
{
    struct recipe *found;

    found = recipe_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = recipe_table[recipe_used - 1];
    recipe_used--;

    return 0;
}

int recipe_total_servings(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < recipe_used; i++) {
        total += recipe_table[i].servings;
    }

    return total;
}

void recipe_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(recipe_seed) / sizeof(recipe_seed[0]); i++) {
        recipe_add(recipe_seed[i].key, recipe_seed[i].name,
                    recipe_seed[i].servings);
    }
}

void recipe_report(FILE *out)
{
    int i;

    fprintf(out, "%d recipes\n", recipe_used);
    for (i = 0; i < recipe_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", recipe_table[i].key,
                recipe_table[i].name, recipe_table[i].servings);
    }
    fprintf(out, "  total servings %d\n", recipe_total_servings());
}

int main(void)
{
    recipe_reset();
    recipe_load_seed();
    recipe_remove(103);
    recipe_report(stdout);

    return 0;
}
