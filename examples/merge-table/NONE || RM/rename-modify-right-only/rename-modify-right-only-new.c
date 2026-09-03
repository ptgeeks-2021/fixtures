/*
 * A fixed size session table.
 *
 * It holds up to SESSION_SLOTS sessions, each with a key, a short name, and
 * a duration. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define SESSION_SLOTS 128
#define SESSION_NAME_SIZE 24

struct session {
    int key;
    char name[SESSION_NAME_SIZE];
    int duration;
};

static const struct session session_seed[] = {
    { 101, "session-0001",  10 },
    { 102, "session-0002",  17 },
    { 103, "session-0003",  24 },
    { 104, "session-0004",  31 },
    { 105, "session-0005",  38 },
    { 106, "session-0006",  45 },
    { 107, "session-0007",  52 },
    { 108, "session-0008",  59 },
    { 109, "session-0009",  66 },
    { 110, "session-0010",  73 },
    { 111, "session-0011",  80 },
    { 112, "session-0012",  87 },
    { 113, "session-0013",  94 },
    { 114, "session-0014",   4 },
    { 115, "session-0015",  11 },
    { 116, "session-0016",  18 },
    { 117, "session-0017",  25 },
    { 118, "session-0018",  32 },
    { 119, "session-0019",  39 },
    { 120, "session-0020",  46 },
    { 121, "session-0021",  53 },
    { 122, "session-0022",  60 },
    { 123, "session-0023",  67 },
    { 124, "session-0024",  74 },
    { 125, "session-0025",  81 },
    { 126, "session-0026",  88 },
    { 127, "session-0027",  95 },
    { 128, "session-0028",   5 },
    { 129, "session-0029",  12 },
    { 130, "session-0030",  19 },
    { 131, "session-0031",  26 },
    { 132, "session-0032",  33 },
    { 133, "session-0033",  40 },
    { 134, "session-0034",  47 },
    { 135, "session-0035",  54 },
    { 136, "session-0036",  61 },
    { 137, "session-0037",  68 },
    { 138, "session-0038",  75 },
    { 139, "session-0039",  82 },
    { 140, "session-0040",  89 },
    { 141, "session-0041",  96 },
    { 142, "session-0042",   6 },
    { 143, "session-0043",  13 },
    { 144, "session-0044",  20 },
    { 145, "session-0045",  27 },
    { 146, "session-0046",  34 },
    { 147, "session-0047",  41 },
    { 148, "session-0048",  48 },
};

static struct session session_table[SESSION_SLOTS];
static int session_used;

void session_reset(void)
{
    memset(session_table, 0, sizeof(session_table));
    session_used = 0;
}

int session_add(int key, const char *name, int duration)
{
    struct session *slot;

    if (session_used >= SESSION_SLOTS) {
        return -1;
    }

    slot = &session_table[session_used];
    slot->key = key;
    slot->duration = duration;
    strncpy(slot->name, name, SESSION_NAME_SIZE - 1);
    slot->name[SESSION_NAME_SIZE - 1] = '\0';
    session_used++;

    return session_used - 1;
}

struct session *session_find(int key)
{
    int i;

    for (i = 0; i < session_used; i++) {
        if (session_table[i].key == key) {
            return &session_table[i];
        }
    }

    return NULL;
}

int session_remove(int key)
{
    struct session *found;

    found = session_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = session_table[session_used - 1];
    session_used--;

    return 0;
}

int session_total_duration(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < session_used; i++) {
        total += session_table[i].duration;
    }

    return total;
}

void session_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(session_seed) / sizeof(session_seed[0]); i++) {
        session_add(session_seed[i].key, session_seed[i].name,
                    session_seed[i].duration);
    }
}

void session_report(FILE *out)
{
    int i;

    fprintf(out, "%d sessions\n", session_used);
    for (i = 0; i < session_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", session_table[i].key,
                session_table[i].name, session_table[i].duration);
    }
    fprintf(out, "  total duration %d\n", session_total_duration());
}

int main(void)
{
    session_reset();
    session_load_seed();
    session_remove(103);
    session_report(stdout);

    return 0;
}
