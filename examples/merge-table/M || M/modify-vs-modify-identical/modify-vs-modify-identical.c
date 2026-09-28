/*
 * A fixed size podcast table.
 *
 * It holds up to PODCAST_SLOTS podcasts, each with a key, a short name, and
 * a episodes. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define PODCAST_SLOTS 256
#define PODCAST_NAME_SIZE 24

struct podcast {
    int key;
    char name[PODCAST_NAME_SIZE];
    int episodes;
};

static const struct podcast podcast_seed[] = {
    { 101, "podcast-0001",  10 },
    { 102, "podcast-0002",  17 },
    { 103, "podcast-0003",  24 },
    { 104, "podcast-0004",  31 },
    { 105, "podcast-0005",  38 },
    { 106, "podcast-0006",  45 },
    { 107, "podcast-0007",  52 },
    { 108, "podcast-0008",  59 },
    { 109, "podcast-0009",  66 },
    { 110, "podcast-0010",  73 },
    { 111, "podcast-0011",  80 },
    { 112, "podcast-0012",  87 },
    { 113, "podcast-0013",  94 },
    { 114, "podcast-0014",   4 },
    { 115, "podcast-0015",  11 },
    { 116, "podcast-0016",  18 },
    { 117, "podcast-0017",  25 },
    { 118, "podcast-0018",  32 },
    { 119, "podcast-0019",  39 },
    { 120, "podcast-0020",  46 },
    { 121, "podcast-0021",  53 },
    { 122, "podcast-0022",  60 },
    { 123, "podcast-0023",  67 },
    { 124, "podcast-0024",  74 },
    { 125, "podcast-0025",  81 },
    { 126, "podcast-0026",  88 },
    { 127, "podcast-0027",  95 },
    { 128, "podcast-0028",   5 },
    { 129, "podcast-0029",  12 },
    { 130, "podcast-0030",  19 },
    { 131, "podcast-0031",  26 },
    { 132, "podcast-0032",  33 },
    { 133, "podcast-0033",  40 },
    { 134, "podcast-0034",  47 },
    { 135, "podcast-0035",  54 },
    { 136, "podcast-0036",  61 },
    { 137, "podcast-0037",  68 },
    { 138, "podcast-0038",  75 },
    { 139, "podcast-0039",  82 },
    { 140, "podcast-0040",  89 },
    { 141, "podcast-0041",  96 },
    { 142, "podcast-0042",   6 },
    { 143, "podcast-0043",  13 },
    { 144, "podcast-0044",  20 },
    { 145, "podcast-0045",  27 },
    { 146, "podcast-0046",  34 },
    { 147, "podcast-0047",  41 },
    { 148, "podcast-0048",  48 },
};

static struct podcast podcast_table[PODCAST_SLOTS];
static int podcast_used;

void podcast_reset(void)
{
    memset(podcast_table, 0, sizeof(podcast_table));
    podcast_used = 0;
}

int podcast_add(int key, const char *name, int episodes)
{
    struct podcast *slot;

    if (podcast_used >= PODCAST_SLOTS) {
        return -1;
    }

    slot = &podcast_table[podcast_used];
    slot->key = key;
    slot->episodes = episodes;
    strncpy(slot->name, name, PODCAST_NAME_SIZE - 1);
    slot->name[PODCAST_NAME_SIZE - 1] = '\0';
    podcast_used++;

    return podcast_used - 1;
}

struct podcast *podcast_find(int key)
{
    int i;

    for (i = 0; i < podcast_used; i++) {
        if (podcast_table[i].key == key) {
            return &podcast_table[i];
        }
    }

    return NULL;
}

int podcast_remove(int key)
{
    struct podcast *found;

    found = podcast_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = podcast_table[podcast_used - 1];
    podcast_used--;

    return 0;
}

int podcast_total_episodes(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < podcast_used; i++) {
        total += podcast_table[i].episodes;
    }

    return total;
}

void podcast_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(podcast_seed) / sizeof(podcast_seed[0]); i++) {
        podcast_add(podcast_seed[i].key, podcast_seed[i].name,
                    podcast_seed[i].episodes);
    }
}

void podcast_report(FILE *out)
{
    int i;

    fprintf(out, "%d podcasts\n", podcast_used);
    for (i = 0; i < podcast_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", podcast_table[i].key,
                podcast_table[i].name, podcast_table[i].episodes);
    }
    fprintf(out, "  total episodes %d\n", podcast_total_episodes());
}

int main(void)
{
    podcast_reset();
    podcast_load_seed();
    podcast_remove(103);
    podcast_report(stdout);

    return 0;
}
