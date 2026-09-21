/*
 * A fixed size booking table.
 *
 * It holds up to BOOKING_SLOTS bookings, each with a key, a short name, and
 * a guests. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define BOOKING_SLOTS 96
#define BOOKING_NAME_SIZE 24

struct booking {
    int key;
    char name[BOOKING_NAME_SIZE];
    int guests;
};

static const struct booking booking_seed[] = {
    { 101, "booking-0001",  10 },
    { 102, "booking-0002",  17 },
    { 103, "booking-0003",  24 },
    { 104, "booking-0004",  31 },
    { 105, "booking-0005",  38 },
    { 106, "booking-0006",  45 },
    { 107, "booking-0007",  52 },
    { 108, "booking-0008",  59 },
    { 109, "booking-0009",  66 },
    { 110, "booking-0010",  73 },
    { 111, "booking-0011",  80 },
    { 112, "booking-0012",  87 },
    { 113, "booking-0013",  94 },
    { 114, "booking-0014",   4 },
    { 115, "booking-0015",  11 },
    { 116, "booking-0016",  18 },
    { 117, "booking-0017",  25 },
    { 118, "booking-0018",  32 },
    { 119, "booking-0019",  39 },
    { 120, "booking-0020",  46 },
    { 121, "booking-0021",  53 },
    { 122, "booking-0022",  60 },
    { 123, "booking-0023",  67 },
    { 124, "booking-0024",  74 },
    { 125, "booking-0025",  81 },
    { 126, "booking-0026",  88 },
    { 127, "booking-0027",  95 },
    { 128, "booking-0028",   5 },
    { 129, "booking-0029",  12 },
    { 130, "booking-0030",  19 },
    { 131, "booking-0031",  26 },
    { 132, "booking-0032",  33 },
    { 133, "booking-0033",  40 },
    { 134, "booking-0034",  47 },
    { 135, "booking-0035",  54 },
    { 136, "booking-0036",  61 },
    { 137, "booking-0037",  68 },
    { 138, "booking-0038",  75 },
    { 139, "booking-0039",  82 },
    { 140, "booking-0040",  89 },
    { 141, "booking-0041",  96 },
    { 142, "booking-0042",   6 },
    { 143, "booking-0043",  13 },
    { 144, "booking-0044",  20 },
    { 145, "booking-0045",  27 },
    { 146, "booking-0046",  34 },
    { 147, "booking-0047",  41 },
    { 148, "booking-0048",  48 },
};

static struct booking booking_table[BOOKING_SLOTS];
static int booking_used;

void booking_reset(void)
{
    memset(booking_table, 0, sizeof(booking_table));
    booking_used = 0;
}

int booking_add(int key, const char *name, int guests)
{
    struct booking *slot;

    if (booking_used >= BOOKING_SLOTS) {
        return -1;
    }

    slot = &booking_table[booking_used];
    slot->key = key;
    slot->guests = guests;
    strncpy(slot->name, name, BOOKING_NAME_SIZE - 1);
    slot->name[BOOKING_NAME_SIZE - 1] = '\0';
    booking_used++;

    return booking_used - 1;
}

struct booking *booking_find(int key)
{
    int i;

    for (i = 0; i < booking_used; i++) {
        if (booking_table[i].key == key) {
            return &booking_table[i];
        }
    }

    return NULL;
}

int booking_remove(int key)
{
    struct booking *found;

    found = booking_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = booking_table[booking_used - 1];
    booking_used--;

    return 0;
}

int booking_total_guests(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < booking_used; i++) {
        total += booking_table[i].guests;
    }

    return total;
}

void booking_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(booking_seed) / sizeof(booking_seed[0]); i++) {
        booking_add(booking_seed[i].key, booking_seed[i].name,
                    booking_seed[i].guests);
    }
}

void booking_report(FILE *out)
{
    int i;

    fprintf(out, "%d bookings\n", booking_used);
    for (i = 0; i < booking_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", booking_table[i].key,
                booking_table[i].name, booking_table[i].guests);
    }
    fprintf(out, "  total guests %d\n", booking_total_guests());
}

int main(void)
{
    booking_reset();
    booking_load_seed();
    booking_remove(103);
    booking_report(stdout);

    return 0;
}
