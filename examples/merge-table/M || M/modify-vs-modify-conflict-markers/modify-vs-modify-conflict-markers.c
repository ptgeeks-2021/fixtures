/*
 * A fixed size workout table.
<<<<<<< left marker text
 * It holds up to WORKOUT_SLOTS workouts, each with a key, a short name, and
 * a reps. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define WORKOUT_SLOTS 64
#define WORKOUT_NAME_SIZE 24

struct workout {
    int key;
    char name[WORKOUT_NAME_SIZE];
    int reps;
};

static const struct workout workout_seed[] = {
    { 101, "workout-0001",  10 },
    { 102, "workout-0002",  17 },
    { 103, "workout-0003",  24 },
    { 104, "workout-0004",  31 },
    { 105, "workout-0005",  38 },
    { 106, "workout-0006",  45 },
    { 107, "workout-0007",  52 },
    { 108, "workout-0008",  59 },
    { 109, "workout-0009",  66 },
    { 110, "workout-0010",  73 },
    { 111, "workout-0011",  80 },
    { 112, "workout-0012",  87 },
    { 113, "workout-0013",  94 },
    { 114, "workout-0014",   4 },
    { 115, "workout-0015",  11 },
    { 116, "workout-0016",  18 },
    { 117, "workout-0017",  25 },
    { 118, "workout-0018",  32 },
    { 119, "workout-0019",  39 },
    { 120, "workout-0020",  46 },
    { 121, "workout-0021",  53 },
    { 122, "workout-0022",  60 },
    { 123, "workout-0023",  67 },
    { 124, "workout-0024",  74 },
    { 125, "workout-0025",  81 },
    { 126, "workout-0026",  88 },
    { 127, "workout-0027",  95 },
    { 128, "workout-0028",   5 },
    { 129, "workout-0029",  12 },
    { 130, "workout-0030",  19 },
    { 131, "workout-0031",  26 },
    { 132, "workout-0032",  33 },
    { 133, "workout-0033",  40 },
    { 134, "workout-0034",  47 },
    { 135, "workout-0035",  54 },
    { 136, "workout-0036",  61 },
    { 137, "workout-0037",  68 },
    { 138, "workout-0038",  75 },
    { 139, "workout-0039",  82 },
    { 140, "workout-0040",  89 },
    { 141, "workout-0041",  96 },
    { 142, "workout-0042",   6 },
    { 143, "workout-0043",  13 },
    { 144, "workout-0044",  20 },
    { 145, "workout-0045",  27 },
    { 146, "workout-0046",  34 },
    { 147, "workout-0047",  41 },
    { 148, "workout-0048",  48 },
};

static struct workout workout_table[WORKOUT_SLOTS];
static int workout_used;

void workout_reset(void)
{
    memset(workout_table, 0, sizeof(workout_table));
    workout_used = 0;
}

int workout_add(int key, const char *name, int reps)
{
    struct workout *slot;

    if (workout_used >= WORKOUT_SLOTS) {
        return -1;
    }

    slot = &workout_table[workout_used];
    slot->key = key;
    slot->reps = reps;
    strncpy(slot->name, name, WORKOUT_NAME_SIZE - 1);
    slot->name[WORKOUT_NAME_SIZE - 1] = '\0';
    workout_used++;

    return workout_used - 1;
}

struct workout *workout_find(int key)
{
    int i;

    for (i = 0; i < workout_used; i++) {
        if (workout_table[i].key == key) {
            return &workout_table[i];
        }
    }

    return NULL;
}

int workout_remove(int key)
{
    struct workout *found;

    found = workout_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = workout_table[workout_used - 1];
    workout_used--;

    return 0;
}

int workout_total_reps(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < workout_used; i++) {
        total += workout_table[i].reps;
    }

    return total;
}

void workout_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(workout_seed) / sizeof(workout_seed[0]); i++) {
        workout_add(workout_seed[i].key, workout_seed[i].name,
                    workout_seed[i].reps);
    }
}

void workout_report(FILE *out)
{
    int i;

    fprintf(out, "%d workouts\n", workout_used);
    for (i = 0; i < workout_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", workout_table[i].key,
                workout_table[i].name, workout_table[i].reps);
    }
    fprintf(out, "  total reps %d\n", workout_total_reps());
}

int main(void)
{
    workout_reset();
    workout_load_seed();
    workout_remove(103);
    workout_report(stdout);

    return 0;
}
