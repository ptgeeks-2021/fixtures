/*
 * A fixed size subscriber table.
 *
 * It holds up to SUBSCRIBER_SLOTS subscribers, each with a key, a short name, and
 * a credits. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define SUBSCRIBER_SLOTS 96
#define SUBSCRIBER_NAME_SIZE 24

struct subscriber {
    int key;
    char name[SUBSCRIBER_NAME_SIZE];
    int credits;
};

static const struct subscriber subscriber_seed[] = {
    { 101, "subscriber-0001",  10 },
    { 102, "subscriber-0002",  17 },
    { 103, "subscriber-0003",  24 },
    { 104, "subscriber-0004",  31 },
    { 105, "subscriber-0005",  38 },
    { 106, "subscriber-0006",  45 },
    { 107, "subscriber-0007",  52 },
    { 108, "subscriber-0008",  59 },
    { 109, "subscriber-0009",  66 },
    { 110, "subscriber-0010",  73 },
    { 111, "subscriber-0011",  80 },
    { 112, "subscriber-0012",  87 },
    { 113, "subscriber-0013",  94 },
    { 114, "subscriber-0014",   4 },
    { 115, "subscriber-0015",  11 },
    { 116, "subscriber-0016",  18 },
    { 117, "subscriber-0017",  25 },
    { 118, "subscriber-0018",  32 },
    { 119, "subscriber-0019",  39 },
    { 120, "subscriber-0020",  46 },
    { 121, "subscriber-0021",  53 },
    { 122, "subscriber-0022",  60 },
    { 123, "subscriber-0023",  67 },
    { 124, "subscriber-0024",  74 },
    { 125, "subscriber-0025",  81 },
    { 126, "subscriber-0026",  88 },
    { 127, "subscriber-0027",  95 },
    { 128, "subscriber-0028",   5 },
    { 129, "subscriber-0029",  12 },
    { 130, "subscriber-0030",  19 },
    { 131, "subscriber-0031",  26 },
    { 132, "subscriber-0032",  33 },
    { 133, "subscriber-0033",  40 },
    { 134, "subscriber-0034",  47 },
    { 135, "subscriber-0035",  54 },
    { 136, "subscriber-0036",  61 },
    { 137, "subscriber-0037",  68 },
    { 138, "subscriber-0038",  75 },
    { 139, "subscriber-0039",  82 },
    { 140, "subscriber-0040",  89 },
    { 141, "subscriber-0041",  96 },
    { 142, "subscriber-0042",   6 },
    { 143, "subscriber-0043",  13 },
    { 144, "subscriber-0044",  20 },
    { 145, "subscriber-0045",  27 },
    { 146, "subscriber-0046",  34 },
    { 147, "subscriber-0047",  41 },
    { 148, "subscriber-0048",  48 },
};

static struct subscriber subscriber_table[SUBSCRIBER_SLOTS];
static int subscriber_used;

void subscriber_reset(void)
{
    memset(subscriber_table, 0, sizeof(subscriber_table));
    subscriber_used = 0;
}

int subscriber_add(int key, const char *name, int credits)
{
    struct subscriber *slot;

    if (subscriber_used >= SUBSCRIBER_SLOTS) {
        return -1;
    }

    slot = &subscriber_table[subscriber_used];
    slot->key = key;
    slot->credits = credits;
    strncpy(slot->name, name, SUBSCRIBER_NAME_SIZE - 1);
    slot->name[SUBSCRIBER_NAME_SIZE - 1] = '\0';
    subscriber_used++;

    return subscriber_used - 1;
}

struct subscriber *subscriber_find(int key)
{
    int i;

    for (i = 0; i < subscriber_used; i++) {
        if (subscriber_table[i].key == key) {
            return &subscriber_table[i];
        }
    }

    return NULL;
}

int subscriber_remove(int key)
{
    struct subscriber *found;

    found = subscriber_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = subscriber_table[subscriber_used - 1];
    subscriber_used--;

    return 0;
}

int subscriber_total_credits(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < subscriber_used; i++) {
        total += subscriber_table[i].credits;
    }

    return total;
}

void subscriber_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(subscriber_seed) / sizeof(subscriber_seed[0]); i++) {
        subscriber_add(subscriber_seed[i].key, subscriber_seed[i].name,
                    subscriber_seed[i].credits);
    }
}

void subscriber_report(FILE *out)
{
    int i;

    fprintf(out, "%d subscribers\n", subscriber_used);
    for (i = 0; i < subscriber_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", subscriber_table[i].key,
                subscriber_table[i].name, subscriber_table[i].credits);
    }
    fprintf(out, "  total credits %d\n", subscriber_total_credits());
}

int main(void)
{
    subscriber_reset();
    subscriber_load_seed();
    subscriber_remove(103);
    subscriber_report(stdout);

    return 0;
}
