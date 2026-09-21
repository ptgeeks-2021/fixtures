/*
 * A fixed size ticket table.
 *
 * It holds up to TICKET_SLOTS tickets, each with a key, a short name, and
 * a priority. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define TICKET_SLOTS 64
#define TICKET_NAME_SIZE 24

struct ticket {
    int key;
    char name[TICKET_NAME_SIZE];
    int priority;
};

static const struct ticket ticket_seed[] = {
    { 101, "ticket-0001",  10 },
    { 102, "ticket-0002",  17 },
    { 103, "ticket-0003",  24 },
    { 104, "ticket-0004",  31 },
    { 105, "ticket-0005",  38 },
    { 106, "ticket-0006",  45 },
    { 107, "ticket-0007",  52 },
    { 108, "ticket-0008",  59 },
    { 109, "ticket-0009",  66 },
    { 110, "ticket-0010",  73 },
    { 111, "ticket-0011",  80 },
    { 112, "ticket-0012",  87 },
    { 113, "ticket-0013",  94 },
    { 114, "ticket-0014",   4 },
    { 115, "ticket-0015",  11 },
    { 116, "ticket-0016",  18 },
    { 117, "ticket-0017",  25 },
    { 118, "ticket-0018",  32 },
    { 119, "ticket-0019",  39 },
    { 120, "ticket-0020",  46 },
    { 121, "ticket-0021",  53 },
    { 122, "ticket-0022",  60 },
    { 123, "ticket-0023",  67 },
    { 124, "ticket-0024",  74 },
    { 125, "ticket-0025",  81 },
    { 126, "ticket-0026",  88 },
    { 127, "ticket-0027",  95 },
    { 128, "ticket-0028",   5 },
    { 129, "ticket-0029",  12 },
    { 130, "ticket-0030",  19 },
    { 131, "ticket-0031",  26 },
    { 132, "ticket-0032",  33 },
    { 133, "ticket-0033",  40 },
    { 134, "ticket-0034",  47 },
    { 135, "ticket-0035",  54 },
    { 136, "ticket-0036",  61 },
    { 137, "ticket-0037",  68 },
    { 138, "ticket-0038",  75 },
    { 139, "ticket-0039",  82 },
    { 140, "ticket-0040",  89 },
    { 141, "ticket-0041",  96 },
    { 142, "ticket-0042",   6 },
    { 143, "ticket-0043",  13 },
    { 144, "ticket-0044",  20 },
    { 145, "ticket-0045",  27 },
    { 146, "ticket-0046",  34 },
    { 147, "ticket-0047",  41 },
    { 148, "ticket-0048",  48 },
};

static struct ticket ticket_table[TICKET_SLOTS];
static int ticket_used;

void ticket_reset(void)
{
    memset(ticket_table, 0, sizeof(ticket_table));
    ticket_used = 0;
}

int ticket_add(int key, const char *name, int priority)
{
    struct ticket *slot;

    if (ticket_used >= TICKET_SLOTS) {
        return -1;
    }

    slot = &ticket_table[ticket_used];
    slot->key = key;
    slot->priority = priority;
    strncpy(slot->name, name, TICKET_NAME_SIZE - 1);
    slot->name[TICKET_NAME_SIZE - 1] = '\0';
    ticket_used++;

    return ticket_used - 1;
}

struct ticket *ticket_find(int key)
{
    int i;

    for (i = 0; i < ticket_used; i++) {
        if (ticket_table[i].key == key) {
            return &ticket_table[i];
        }
    }

    return NULL;
}

int ticket_remove(int key)
{
    struct ticket *found;

    found = ticket_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = ticket_table[ticket_used - 1];
    ticket_used--;

    return 0;
}

int ticket_total_priority(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < ticket_used; i++) {
        total += ticket_table[i].priority;
    }

    return total;
}

void ticket_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(ticket_seed) / sizeof(ticket_seed[0]); i++) {
        ticket_add(ticket_seed[i].key, ticket_seed[i].name,
                    ticket_seed[i].priority);
    }
}

void ticket_report(FILE *out)
{
    int i;

    fprintf(out, "%d tickets\n", ticket_used);
    for (i = 0; i < ticket_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", ticket_table[i].key,
                ticket_table[i].name, ticket_table[i].priority);
    }
    fprintf(out, "  total priority %d\n", ticket_total_priority());
}

int main(void)
{
    ticket_reset();
    ticket_load_seed();
    ticket_remove(103);
    ticket_report(stdout);

    return 0;
}
