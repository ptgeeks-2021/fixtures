/*
 * A fixed size playlist table.
 *
 * It holds up to PLAYLIST_SLOTS playlists, each with a key, a short name, and
 * a tracks. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define PLAYLIST_SLOTS 64
#define PLAYLIST_NAME_SIZE 24

struct playlist {
    int key;
    char name[PLAYLIST_NAME_SIZE];
    int tracks;
};

static const struct playlist playlist_seed[] = {
    { 101, "playlist-0001",  10 },
    { 102, "playlist-0002",  17 },
    { 103, "playlist-0003",  24 },
    { 104, "playlist-0004",  31 },
    { 105, "playlist-0005",  38 },
    { 106, "playlist-0006",  45 },
    { 107, "playlist-0007",  52 },
    { 108, "playlist-0008",  59 },
    { 109, "playlist-0009",  66 },
    { 110, "playlist-0010",  73 },
    { 111, "playlist-0011",  80 },
    { 112, "playlist-0012",  87 },
    { 113, "playlist-0013",  94 },
    { 114, "playlist-0014",   4 },
    { 115, "playlist-0015",  11 },
    { 116, "playlist-0016",  18 },
    { 117, "playlist-0017",  25 },
    { 118, "playlist-0018",  32 },
    { 119, "playlist-0019",  39 },
    { 120, "playlist-0020",  46 },
    { 121, "playlist-0021",  53 },
    { 122, "playlist-0022",  60 },
    { 123, "playlist-0023",  67 },
    { 124, "playlist-0024",  74 },
    { 125, "playlist-0025",  81 },
    { 126, "playlist-0026",  88 },
    { 127, "playlist-0027",  95 },
    { 128, "playlist-0028",   5 },
    { 129, "playlist-0029",  12 },
    { 130, "playlist-0030",  19 },
    { 131, "playlist-0031",  26 },
    { 132, "playlist-0032",  33 },
    { 133, "playlist-0033",  40 },
    { 134, "playlist-0034",  47 },
    { 135, "playlist-0035",  54 },
    { 136, "playlist-0036",  61 },
    { 137, "playlist-0037",  68 },
    { 138, "playlist-0038",  75 },
    { 139, "playlist-0039",  82 },
    { 140, "playlist-0040",  89 },
    { 141, "playlist-0041",  96 },
    { 142, "playlist-0042",   6 },
    { 143, "playlist-0043",  13 },
    { 144, "playlist-0044",  20 },
    { 145, "playlist-0045",  27 },
    { 146, "playlist-0046",  34 },
    { 147, "playlist-0047",  41 },
    { 148, "playlist-0048",  48 },
};

static struct playlist playlist_table[PLAYLIST_SLOTS];
static int playlist_used;

void playlist_reset(void)
{
    memset(playlist_table, 0, sizeof(playlist_table));
    playlist_used = 0;
}

int playlist_add(int key, const char *name, int tracks)
{
    struct playlist *slot;

    if (playlist_used >= PLAYLIST_SLOTS) {
        return -1;
    }

    slot = &playlist_table[playlist_used];
    slot->key = key;
    slot->tracks = tracks;
    strncpy(slot->name, name, PLAYLIST_NAME_SIZE - 1);
    slot->name[PLAYLIST_NAME_SIZE - 1] = '\0';
    playlist_used++;

    return playlist_used - 1;
}

struct playlist *playlist_find(int key)
{
    int i;

    for (i = 0; i < playlist_used; i++) {
        if (playlist_table[i].key == key) {
            return &playlist_table[i];
        }
    }

    return NULL;
}

int playlist_remove(int key)
{
    struct playlist *found;

    found = playlist_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = playlist_table[playlist_used - 1];
    playlist_used--;

    return 0;
}

int playlist_total_tracks(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < playlist_used; i++) {
        total += playlist_table[i].tracks;
    }

    return total;
}

void playlist_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(playlist_seed) / sizeof(playlist_seed[0]); i++) {
        playlist_add(playlist_seed[i].key, playlist_seed[i].name,
                    playlist_seed[i].tracks);
    }
}

void playlist_report(FILE *out)
{
    int i;

    fprintf(out, "%d playlists\n", playlist_used);
    for (i = 0; i < playlist_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", playlist_table[i].key,
                playlist_table[i].name, playlist_table[i].tracks);
    }
    fprintf(out, "  total tracks %d\n", playlist_total_tracks());
}

int main(void)
{
    playlist_reset();
    playlist_load_seed();
    playlist_remove(103);
    playlist_report(stdout);

    return 0;
}
