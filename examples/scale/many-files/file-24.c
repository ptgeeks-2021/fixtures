/*
 * A fixed size campaign table.
 *
 * It holds up to CAMPAIGN_SLOTS campaigns, each with a key, a short name, and
 * a clicks. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define CAMPAIGN_SLOTS 96
#define CAMPAIGN_NAME_SIZE 24

struct campaign {
    int key;
    char name[CAMPAIGN_NAME_SIZE];
    int clicks;
};

static const struct campaign campaign_seed[] = {
    { 101, "campaign-0001",  10 },
    { 102, "campaign-0002",  17 },
    { 103, "campaign-0003",  24 },
    { 104, "campaign-0004",  31 },
    { 105, "campaign-0005",  38 },
    { 106, "campaign-0006",  45 },
    { 107, "campaign-0007",  52 },
    { 108, "campaign-0008",  59 },
    { 109, "campaign-0009",  66 },
    { 110, "campaign-0010",  73 },
    { 111, "campaign-0011",  80 },
    { 112, "campaign-0012",  87 },
    { 113, "campaign-0013",  94 },
    { 114, "campaign-0014",   4 },
    { 115, "campaign-0015",  11 },
    { 116, "campaign-0016",  18 },
    { 117, "campaign-0017",  25 },
    { 118, "campaign-0018",  32 },
    { 119, "campaign-0019",  39 },
    { 120, "campaign-0020",  46 },
    { 121, "campaign-0021",  53 },
    { 122, "campaign-0022",  60 },
    { 123, "campaign-0023",  67 },
    { 124, "campaign-0024",  74 },
    { 125, "campaign-0025",  81 },
    { 126, "campaign-0026",  88 },
    { 127, "campaign-0027",  95 },
    { 128, "campaign-0028",   5 },
    { 129, "campaign-0029",  12 },
    { 130, "campaign-0030",  19 },
    { 131, "campaign-0031",  26 },
    { 132, "campaign-0032",  33 },
    { 133, "campaign-0033",  40 },
    { 134, "campaign-0034",  47 },
    { 135, "campaign-0035",  54 },
    { 136, "campaign-0036",  61 },
    { 137, "campaign-0037",  68 },
    { 138, "campaign-0038",  75 },
    { 139, "campaign-0039",  82 },
    { 140, "campaign-0040",  89 },
    { 141, "campaign-0041",  96 },
    { 142, "campaign-0042",   6 },
    { 143, "campaign-0043",  13 },
    { 144, "campaign-0044",  20 },
    { 145, "campaign-0045",  27 },
    { 146, "campaign-0046",  34 },
    { 147, "campaign-0047",  41 },
    { 148, "campaign-0048",  48 },
};

static struct campaign campaign_table[CAMPAIGN_SLOTS];
static int campaign_used;

void campaign_reset(void)
{
    memset(campaign_table, 0, sizeof(campaign_table));
    campaign_used = 0;
}

int campaign_add(int key, const char *name, int clicks)
{
    struct campaign *slot;

    if (campaign_used >= CAMPAIGN_SLOTS) {
        return -1;
    }

    slot = &campaign_table[campaign_used];
    slot->key = key;
    slot->clicks = clicks;
    strncpy(slot->name, name, CAMPAIGN_NAME_SIZE - 1);
    slot->name[CAMPAIGN_NAME_SIZE - 1] = '\0';
    campaign_used++;

    return campaign_used - 1;
}

struct campaign *campaign_find(int key)
{
    int i;

    for (i = 0; i < campaign_used; i++) {
        if (campaign_table[i].key == key) {
            return &campaign_table[i];
        }
    }

    return NULL;
}

int campaign_remove(int key)
{
    struct campaign *found;

    found = campaign_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = campaign_table[campaign_used - 1];
    campaign_used--;

    return 0;
}

int campaign_total_clicks(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < campaign_used; i++) {
        total += campaign_table[i].clicks;
    }

    return total;
}

void campaign_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(campaign_seed) / sizeof(campaign_seed[0]); i++) {
        campaign_add(campaign_seed[i].key, campaign_seed[i].name,
                    campaign_seed[i].clicks);
    }
}

void campaign_report(FILE *out)
{
    int i;

    fprintf(out, "%d campaigns\n", campaign_used);
    for (i = 0; i < campaign_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", campaign_table[i].key,
                campaign_table[i].name, campaign_table[i].clicks);
    }
    fprintf(out, "  total clicks %d\n", campaign_total_clicks());
}

int main(void)
{
    campaign_reset();
    campaign_load_seed();
    campaign_remove(103);
    campaign_report(stdout);

    return 0;
}
