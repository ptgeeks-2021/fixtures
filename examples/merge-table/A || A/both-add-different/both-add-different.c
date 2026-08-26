/*
 * A fixed size article table.
 *
 * It holds up to ARTICLE_SLOTS articles, each with a key, a short name, and
 * a words. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define ARTICLE_SLOTS 96
#define ARTICLE_NAME_SIZE 24

struct article {
    int key;
    char name[ARTICLE_NAME_SIZE];
    int words;
};

static const struct article article_seed[] = {
    { 101, "article-0001",  10 },
    { 102, "article-0002",  17 },
    { 103, "article-0003",  24 },
    { 104, "article-0004",  31 },
    { 105, "article-0005",  38 },
    { 106, "article-0006",  45 },
    { 107, "article-0007",  52 },
    { 108, "article-0008",  59 },
    { 109, "article-0009",  66 },
    { 110, "article-0010",  73 },
    { 111, "article-0011",  80 },
    { 112, "article-0012",  87 },
    { 113, "article-0013",  94 },
    { 114, "article-0014",   4 },
    { 115, "article-0015",  11 },
    { 116, "article-0016",  18 },
    { 117, "article-0017",  25 },
    { 118, "article-0018",  32 },
    { 119, "article-0019",  39 },
    { 120, "article-0020",  46 },
    { 121, "article-0021",  53 },
    { 122, "article-0022",  60 },
    { 123, "article-0023",  67 },
    { 124, "article-0024",  74 },
    { 125, "article-0025",  81 },
    { 126, "article-0026",  88 },
    { 127, "article-0027",  95 },
    { 128, "article-0028",   5 },
    { 129, "article-0029",  12 },
    { 130, "article-0030",  19 },
    { 131, "article-0031",  26 },
    { 132, "article-0032",  33 },
    { 133, "article-0033",  40 },
    { 134, "article-0034",  47 },
    { 135, "article-0035",  54 },
    { 136, "article-0036",  61 },
    { 137, "article-0037",  68 },
    { 138, "article-0038",  75 },
    { 139, "article-0039",  82 },
    { 140, "article-0040",  89 },
    { 141, "article-0041",  96 },
    { 142, "article-0042",   6 },
    { 143, "article-0043",  13 },
    { 144, "article-0044",  20 },
    { 145, "article-0045",  27 },
    { 146, "article-0046",  34 },
    { 147, "article-0047",  41 },
    { 148, "article-0048",  48 },
};

static struct article article_table[ARTICLE_SLOTS];
static int article_used;

void article_reset(void)
{
    memset(article_table, 0, sizeof(article_table));
    article_used = 0;
}

int article_add(int key, const char *name, int words)
{
    struct article *slot;

    if (article_used >= ARTICLE_SLOTS) {
        return -1;
    }

    slot = &article_table[article_used];
    slot->key = key;
    slot->words = words;
    strncpy(slot->name, name, ARTICLE_NAME_SIZE - 1);
    slot->name[ARTICLE_NAME_SIZE - 1] = '\0';
    article_used++;

    return article_used - 1;
}

struct article *article_find(int key)
{
    int i;

    for (i = 0; i < article_used; i++) {
        if (article_table[i].key == key) {
            return &article_table[i];
        }
    }

    return NULL;
}

int article_remove(int key)
{
    struct article *found;

    found = article_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = article_table[article_used - 1];
    article_used--;

    return 0;
}

int article_total_words(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < article_used; i++) {
        total += article_table[i].words;
    }

    return total;
}

void article_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(article_seed) / sizeof(article_seed[0]); i++) {
        article_add(article_seed[i].key, article_seed[i].name,
                    article_seed[i].words);
    }
}

void article_report(FILE *out)
{
    int i;

    fprintf(out, "%d articles\n", article_used);
    for (i = 0; i < article_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", article_table[i].key,
                article_table[i].name, article_table[i].words);
    }
    fprintf(out, "  total words %d\n", article_total_words());
}

int main(void)
{
    article_reset();
    article_load_seed();
    article_remove(103);
    article_report(stdout);

    return 0;
}
