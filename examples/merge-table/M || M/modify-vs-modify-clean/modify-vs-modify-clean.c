/*
 * A fixed size album table.
 *
 * It holds up to ALBUM_SLOTS albums, each with a key, a short name, and
 * a photos. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define ALBUM_SLOTS 96
#define ALBUM_NAME_SIZE 24

struct album {
    int key;
    char name[ALBUM_NAME_SIZE];
    int photos;
};

static const struct album album_seed[] = {
    { 101, "album-0001",  10 },
    { 102, "album-0002",  17 },
    { 103, "album-0003",  24 },
    { 104, "album-0004",  31 },
    { 105, "album-0005",  38 },
    { 106, "album-0006",  45 },
    { 107, "album-0007",  52 },
    { 108, "album-0008",  59 },
    { 109, "album-0009",  66 },
    { 110, "album-0010",  73 },
    { 111, "album-0011",  80 },
    { 112, "album-0012",  87 },
    { 113, "album-0013",  94 },
    { 114, "album-0014",   4 },
    { 115, "album-0015",  11 },
    { 116, "album-0016",  18 },
    { 117, "album-0017",  25 },
    { 118, "album-0018",  32 },
    { 119, "album-0019",  39 },
    { 120, "album-0020",  46 },
    { 121, "album-0021",  53 },
    { 122, "album-0022",  60 },
    { 123, "album-0023",  67 },
    { 124, "album-0024",  74 },
    { 125, "album-0025",  81 },
    { 126, "album-0026",  88 },
    { 127, "album-0027",  95 },
    { 128, "album-0028",   5 },
    { 129, "album-0029",  12 },
    { 130, "album-0030",  19 },
    { 131, "album-0031",  26 },
    { 132, "album-0032",  33 },
    { 133, "album-0033",  40 },
    { 134, "album-0034",  47 },
    { 135, "album-0035",  54 },
    { 136, "album-0036",  61 },
    { 137, "album-0037",  68 },
    { 138, "album-0038",  75 },
    { 139, "album-0039",  82 },
    { 140, "album-0040",  89 },
    { 141, "album-0041",  96 },
    { 142, "album-0042",   6 },
    { 143, "album-0043",  13 },
    { 144, "album-0044",  20 },
    { 145, "album-0045",  27 },
    { 146, "album-0046",  34 },
    { 147, "album-0047",  41 },
    { 148, "album-0048",  48 },
};

static struct album album_table[ALBUM_SLOTS];
static int album_used;

void album_reset(void)
{
    memset(album_table, 0, sizeof(album_table));
    album_used = 0;
}

int album_add(int key, const char *name, int photos)
{
    struct album *slot;

    if (album_used >= ALBUM_SLOTS) {
        return -1;
    }

    slot = &album_table[album_used];
    slot->key = key;
    slot->photos = photos;
    strncpy(slot->name, name, ALBUM_NAME_SIZE - 1);
    slot->name[ALBUM_NAME_SIZE - 1] = '\0';
    album_used++;

    return album_used - 1;
}

struct album *album_find(int key)
{
    int i;

    for (i = 0; i < album_used; i++) {
        if (album_table[i].key == key) {
            return &album_table[i];
        }
    }

    return NULL;
}

int album_remove(int key)
{
    struct album *found;

    found = album_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = album_table[album_used - 1];
    album_used--;

    return 0;
}

int album_total_photos(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < album_used; i++) {
        total += album_table[i].photos;
    }

    return total;
}

void album_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(album_seed) / sizeof(album_seed[0]); i++) {
        album_add(album_seed[i].key, album_seed[i].name,
                    album_seed[i].photos);
    }
}

void album_report(FILE *out)
{
    int i;

    fprintf(out, "%d albums\n", album_used);
    for (i = 0; i < album_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", album_table[i].key,
                album_table[i].name, album_table[i].photos);
    }
    fprintf(out, "  total photos %d\n", album_total_photos());
}

int main(void)
{
    album_reset();
    album_load_seed();
    album_remove(103);
    album_report(stdout);

    return 0;
}
