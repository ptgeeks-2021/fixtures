/*
 * A fixed size channel table.
 *
 * It holds up to CHANNEL_SLOTS channels, each with a key, a short name, and
 * a bitrate. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define CHANNEL_SLOTS 128
#define CHANNEL_NAME_SIZE 24

struct channel {
    int key;
    char name[CHANNEL_NAME_SIZE];
    int bitrate;
};

static const struct channel channel_seed[] = {
    { 101, "channel-0001",  10 },
    { 102, "channel-0002",  17 },
    { 103, "channel-0003",  24 },
    { 104, "channel-0004",  31 },
    { 105, "channel-0005",  38 },
    { 106, "channel-0006",  45 },
    { 107, "channel-0007",  52 },
    { 108, "channel-0008",  59 },
    { 109, "channel-0009",  66 },
    { 110, "channel-0010",  73 },
    { 111, "channel-0011",  80 },
    { 112, "channel-0012",  87 },
    { 113, "channel-0013",  94 },
    { 114, "channel-0014",   4 },
    { 115, "channel-0015",  11 },
    { 116, "channel-0016",  18 },
    { 117, "channel-0017",  25 },
    { 118, "channel-0018",  32 },
    { 119, "channel-0019",  39 },
    { 120, "channel-0020",  46 },
    { 121, "channel-0021",  53 },
    { 122, "channel-0022",  60 },
    { 123, "channel-0023",  67 },
    { 124, "channel-0024",  74 },
    { 125, "channel-0025",  81 },
    { 126, "channel-0026",  88 },
    { 127, "channel-0027",  95 },
    { 128, "channel-0028",   5 },
    { 129, "channel-0029",  12 },
    { 130, "channel-0030",  19 },
    { 131, "channel-0031",  26 },
    { 132, "channel-0032",  33 },
    { 133, "channel-0033",  40 },
    { 134, "channel-0034",  47 },
    { 135, "channel-0035",  54 },
    { 136, "channel-0036",  61 },
    { 137, "channel-0037",  68 },
    { 138, "channel-0038",  75 },
    { 139, "channel-0039",  82 },
    { 140, "channel-0040",  89 },
    { 141, "channel-0041",  96 },
    { 142, "channel-0042",   6 },
    { 143, "channel-0043",  13 },
    { 144, "channel-0044",  20 },
    { 145, "channel-0045",  27 },
    { 146, "channel-0046",  34 },
    { 147, "channel-0047",  41 },
    { 148, "channel-0048",  48 },
};

static struct channel channel_table[CHANNEL_SLOTS];
static int channel_used;

void channel_reset(void)
{
    memset(channel_table, 0, sizeof(channel_table));
    channel_used = 0;
}

int channel_add(int key, const char *name, int bitrate)
{
    struct channel *slot;

    if (channel_used >= CHANNEL_SLOTS) {
        return -1;
    }

    slot = &channel_table[channel_used];
    slot->key = key;
    slot->bitrate = bitrate;
    strncpy(slot->name, name, CHANNEL_NAME_SIZE - 1);
    slot->name[CHANNEL_NAME_SIZE - 1] = '\0';
    channel_used++;

    return channel_used - 1;
}

struct channel *channel_find(int key)
{
    int i;

    for (i = 0; i < channel_used; i++) {
        if (channel_table[i].key == key) {
            return &channel_table[i];
        }
    }

    return NULL;
}

int channel_remove(int key)
{
    struct channel *found;

    found = channel_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = channel_table[channel_used - 1];
    channel_used--;

    return 0;
}

int channel_total_bitrate(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < channel_used; i++) {
        total += channel_table[i].bitrate;
    }

    return total;
}

void channel_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(channel_seed) / sizeof(channel_seed[0]); i++) {
        channel_add(channel_seed[i].key, channel_seed[i].name,
                    channel_seed[i].bitrate);
    }
}

void channel_report(FILE *out)
{
    int i;

    fprintf(out, "%d channels\n", channel_used);
    for (i = 0; i < channel_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", channel_table[i].key,
                channel_table[i].name, channel_table[i].bitrate);
    }
    fprintf(out, "  total bitrate %d\n", channel_total_bitrate());
}

int main(void)
{
    channel_reset();
    channel_load_seed();
    channel_remove(103);
    channel_report(stdout);

    return 0;
}
