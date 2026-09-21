/*
 * A fixed size lesson table.
 *
 * It holds up to LESSON_SLOTS lessons, each with a key, a short name, and
 * a minutes. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define LESSON_SLOTS 64
#define LESSON_NAME_SIZE 24

struct lesson {
    int key;
    char name[LESSON_NAME_SIZE];
    int minutes;
};

static const struct lesson lesson_seed[] = {
    { 101, "lesson-0001",  10 },
    { 102, "lesson-0002",  17 },
    { 103, "lesson-0003",  24 },
    { 104, "lesson-0004",  31 },
    { 105, "lesson-0005",  38 },
    { 106, "lesson-0006",  45 },
    { 107, "lesson-0007",  52 },
    { 108, "lesson-0008",  59 },
    { 109, "lesson-0009",  66 },
    { 110, "lesson-0010",  73 },
    { 111, "lesson-0011",  80 },
    { 112, "lesson-0012",  87 },
    { 113, "lesson-0013",  94 },
    { 114, "lesson-0014",   4 },
    { 115, "lesson-0015",  11 },
    { 116, "lesson-0016",  18 },
    { 117, "lesson-0017",  25 },
    { 118, "lesson-0018",  32 },
    { 119, "lesson-0019",  39 },
    { 120, "lesson-0020",  46 },
    { 121, "lesson-0021",  53 },
    { 122, "lesson-0022",  60 },
    { 123, "lesson-0023",  67 },
    { 124, "lesson-0024",  74 },
    { 125, "lesson-0025",  81 },
    { 126, "lesson-0026",  88 },
    { 127, "lesson-0027",  95 },
    { 128, "lesson-0028",   5 },
    { 129, "lesson-0029",  12 },
    { 130, "lesson-0030",  19 },
    { 131, "lesson-0031",  26 },
    { 132, "lesson-0032",  33 },
    { 133, "lesson-0033",  40 },
    { 134, "lesson-0034",  47 },
    { 135, "lesson-0035",  54 },
    { 136, "lesson-0036",  61 },
    { 137, "lesson-0037",  68 },
    { 138, "lesson-0038",  75 },
    { 139, "lesson-0039",  82 },
    { 140, "lesson-0040",  89 },
    { 141, "lesson-0041",  96 },
    { 142, "lesson-0042",   6 },
    { 143, "lesson-0043",  13 },
    { 144, "lesson-0044",  20 },
    { 145, "lesson-0045",  27 },
    { 146, "lesson-0046",  34 },
    { 147, "lesson-0047",  41 },
    { 148, "lesson-0048",  48 },
};

static struct lesson lesson_table[LESSON_SLOTS];
static int lesson_used;

void lesson_reset(void)
{
    memset(lesson_table, 0, sizeof(lesson_table));
    lesson_used = 0;
}

int lesson_add(int key, const char *name, int minutes)
{
    struct lesson *slot;

    if (lesson_used >= LESSON_SLOTS) {
        return -1;
    }

    slot = &lesson_table[lesson_used];
    slot->key = key;
    slot->minutes = minutes;
    strncpy(slot->name, name, LESSON_NAME_SIZE - 1);
    slot->name[LESSON_NAME_SIZE - 1] = '\0';
    lesson_used++;

    return lesson_used - 1;
}

struct lesson *lesson_find(int key)
{
    int i;

    for (i = 0; i < lesson_used; i++) {
        if (lesson_table[i].key == key) {
            return &lesson_table[i];
        }
    }

    return NULL;
}

int lesson_remove(int key)
{
    struct lesson *found;

    found = lesson_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = lesson_table[lesson_used - 1];
    lesson_used--;

    return 0;
}

int lesson_total_minutes(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < lesson_used; i++) {
        total += lesson_table[i].minutes;
    }

    return total;
}

void lesson_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(lesson_seed) / sizeof(lesson_seed[0]); i++) {
        lesson_add(lesson_seed[i].key, lesson_seed[i].name,
                    lesson_seed[i].minutes);
    }
}

void lesson_report(FILE *out)
{
    int i;

    fprintf(out, "%d lessons\n", lesson_used);
    for (i = 0; i < lesson_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", lesson_table[i].key,
                lesson_table[i].name, lesson_table[i].minutes);
    }
    fprintf(out, "  total minutes %d\n", lesson_total_minutes());
}

int main(void)
{
    lesson_reset();
    lesson_load_seed();
    lesson_remove(103);
    lesson_report(stdout);

    return 0;
}
