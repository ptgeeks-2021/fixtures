/*
 * A fixed size reservation table.
 *
 * It holds up to RESERVATION_SLOTS reservations, each with a key, a short name, and
 * a nights. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define RESERVATION_SLOTS 96
#define RESERVATION_NAME_SIZE 24

struct reservation {
    int key;
    char name[RESERVATION_NAME_SIZE];
    int nights;
};

static const struct reservation reservation_seed[] = {
    { 101, "reservation-0001",  10 },
    { 102, "reservation-0002",  17 },
    { 103, "reservation-0003",  24 },
    { 104, "reservation-0004",  31 },
    { 105, "reservation-0005",  38 },
    { 106, "reservation-0006",  45 },
    { 107, "reservation-0007",  52 },
    { 108, "reservation-0008",  59 },
    { 109, "reservation-0009",  66 },
    { 110, "reservation-0010",  73 },
    { 111, "reservation-0011",  80 },
    { 112, "reservation-0012",  87 },
    { 113, "reservation-0013",  94 },
    { 114, "reservation-0014",   4 },
    { 115, "reservation-0015",  11 },
    { 116, "reservation-0016",  18 },
    { 117, "reservation-0017",  25 },
    { 118, "reservation-0018",  32 },
    { 119, "reservation-0019",  39 },
    { 120, "reservation-0020",  46 },
    { 121, "reservation-0021",  53 },
    { 122, "reservation-0022",  60 },
    { 123, "reservation-0023",  67 },
    { 124, "reservation-0024",  74 },
    { 125, "reservation-0025",  81 },
    { 126, "reservation-0026",  88 },
    { 127, "reservation-0027",  95 },
    { 128, "reservation-0028",   5 },
    { 129, "reservation-0029",  12 },
    { 130, "reservation-0030",  19 },
    { 131, "reservation-0031",  26 },
    { 132, "reservation-0032",  33 },
    { 133, "reservation-0033",  40 },
    { 134, "reservation-0034",  47 },
    { 135, "reservation-0035",  54 },
    { 136, "reservation-0036",  61 },
    { 137, "reservation-0037",  68 },
    { 138, "reservation-0038",  75 },
    { 139, "reservation-0039",  82 },
    { 140, "reservation-0040",  89 },
    { 141, "reservation-0041",  96 },
    { 142, "reservation-0042",   6 },
    { 143, "reservation-0043",  13 },
    { 144, "reservation-0044",  20 },
    { 145, "reservation-0045",  27 },
    { 146, "reservation-0046",  34 },
    { 147, "reservation-0047",  41 },
    { 148, "reservation-0048",  48 },
};

static struct reservation reservation_table[RESERVATION_SLOTS];
static int reservation_used;

void reservation_reset(void)
{
    memset(reservation_table, 0, sizeof(reservation_table));
    reservation_used = 0;
}

int reservation_add(int key, const char *name, int nights)
{
    struct reservation *slot;

    if (reservation_used >= RESERVATION_SLOTS) {
        return -1;
    }

    slot = &reservation_table[reservation_used];
    slot->key = key;
    slot->nights = nights;
    strncpy(slot->name, name, RESERVATION_NAME_SIZE - 1);
    slot->name[RESERVATION_NAME_SIZE - 1] = '\0';
    reservation_used++;

    return reservation_used - 1;
}

struct reservation *reservation_find(int key)
{
    int i;

    for (i = 0; i < reservation_used; i++) {
        if (reservation_table[i].key == key) {
            return &reservation_table[i];
        }
    }

    return NULL;
}

int reservation_remove(int key)
{
    struct reservation *found;

    found = reservation_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = reservation_table[reservation_used - 1];
    reservation_used--;

    return 0;
}

int reservation_total_nights(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < reservation_used; i++) {
        total += reservation_table[i].nights;
    }

    return total;
}

void reservation_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(reservation_seed) / sizeof(reservation_seed[0]); i++) {
        reservation_add(reservation_seed[i].key, reservation_seed[i].name,
                    reservation_seed[i].nights);
    }
}

void reservation_report(FILE *out)
{
    int i;

    fprintf(out, "%d reservations\n", reservation_used);
    for (i = 0; i < reservation_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", reservation_table[i].key,
                reservation_table[i].name, reservation_table[i].nights);
    }
    fprintf(out, "  total nights %d\n", reservation_total_nights());
}

int main(void)
{
    reservation_reset();
    reservation_load_seed();
    reservation_remove(103);
    reservation_report(stdout);

    return 0;
}
