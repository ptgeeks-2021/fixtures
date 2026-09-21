/*
 * A fixed size account table.
 *
 * It holds up to ACCOUNT_SLOTS accounts, each with a key, a short name, and
 * a balance. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define ACCOUNT_SLOTS 64
#define ACCOUNT_NAME_SIZE 24

struct account {
    int key;
    char name[ACCOUNT_NAME_SIZE];
    int balance;
};

static const struct account account_seed[] = {
    { 101, "account-0001",  10 },
    { 102, "account-0002",  17 },
    { 103, "account-0003",  24 },
    { 104, "account-0004",  31 },
    { 105, "account-0005",  38 },
    { 106, "account-0006",  45 },
    { 107, "account-0007",  52 },
    { 108, "account-0008",  59 },
    { 109, "account-0009",  66 },
    { 110, "account-0010",  73 },
    { 111, "account-0011",  80 },
    { 112, "account-0012",  87 },
    { 113, "account-0013",  94 },
    { 114, "account-0014",   4 },
    { 115, "account-0015",  11 },
    { 116, "account-0016",  18 },
    { 117, "account-0017",  25 },
    { 118, "account-0018",  32 },
    { 119, "account-0019",  39 },
    { 120, "account-0020",  46 },
    { 121, "account-0021",  53 },
    { 122, "account-0022",  60 },
    { 123, "account-0023",  67 },
    { 124, "account-0024",  74 },
    { 125, "account-0025",  81 },
    { 126, "account-0026",  88 },
    { 127, "account-0027",  95 },
    { 128, "account-0028",   5 },
    { 129, "account-0029",  12 },
    { 130, "account-0030",  19 },
    { 131, "account-0031",  26 },
    { 132, "account-0032",  33 },
    { 133, "account-0033",  40 },
    { 134, "account-0034",  47 },
    { 135, "account-0035",  54 },
    { 136, "account-0036",  61 },
    { 137, "account-0037",  68 },
    { 138, "account-0038",  75 },
    { 139, "account-0039",  82 },
    { 140, "account-0040",  89 },
    { 141, "account-0041",  96 },
    { 142, "account-0042",   6 },
    { 143, "account-0043",  13 },
    { 144, "account-0044",  20 },
    { 145, "account-0045",  27 },
    { 146, "account-0046",  34 },
    { 147, "account-0047",  41 },
    { 148, "account-0048",  48 },
};

static struct account account_table[ACCOUNT_SLOTS];
static int account_used;

void account_reset(void)
{
    memset(account_table, 0, sizeof(account_table));
    account_used = 0;
}

int account_add(int key, const char *name, int balance)
{
    struct account *slot;

    if (account_used >= ACCOUNT_SLOTS) {
        return -1;
    }

    slot = &account_table[account_used];
    slot->key = key;
    slot->balance = balance;
    strncpy(slot->name, name, ACCOUNT_NAME_SIZE - 1);
    slot->name[ACCOUNT_NAME_SIZE - 1] = '\0';
    account_used++;

    return account_used - 1;
}

struct account *account_find(int key)
{
    int i;

    for (i = 0; i < account_used; i++) {
        if (account_table[i].key == key) {
            return &account_table[i];
        }
    }

    return NULL;
}

int account_remove(int key)
{
    struct account *found;

    found = account_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = account_table[account_used - 1];
    account_used--;

    return 0;
}

int account_total_balance(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < account_used; i++) {
        total += account_table[i].balance;
    }

    return total;
}

void account_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(account_seed) / sizeof(account_seed[0]); i++) {
        account_add(account_seed[i].key, account_seed[i].name,
                    account_seed[i].balance);
    }
}

void account_report(FILE *out)
{
    int i;

    fprintf(out, "%d accounts\n", account_used);
    for (i = 0; i < account_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", account_table[i].key,
                account_table[i].name, account_table[i].balance);
    }
    fprintf(out, "  total balance %d\n", account_total_balance());
}

int main(void)
{
    account_reset();
    account_load_seed();
    account_remove(103);
    account_report(stdout);

    return 0;
}
