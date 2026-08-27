/*
 * A fixed size patient table.
 *
 * It holds up to PATIENT_SLOTS patients, each with a key, a short name, and
 * a age. Lookups walk the table, which is fine at this size.
 */

#include <stdio.h>
#include <string.h>

#define PATIENT_SLOTS 96
#define PATIENT_NAME_SIZE 24

struct patient {
    int key;
    char name[PATIENT_NAME_SIZE];
    int age;
};

static const struct patient patient_seed[] = {
    { 101, "patient-0001",  10 },
    { 102, "patient-0002",  17 },
    { 103, "patient-0003",  24 },
    { 104, "patient-0004",  31 },
    { 105, "patient-0005",  38 },
    { 106, "patient-0006",  45 },
    { 107, "patient-0007",  52 },
    { 108, "patient-0008",  59 },
    { 109, "patient-0009",  66 },
    { 110, "patient-0010",  73 },
    { 111, "patient-0011",  80 },
    { 112, "patient-0012",  87 },
    { 113, "patient-0013",  94 },
    { 114, "patient-0014",   4 },
    { 115, "patient-0015",  11 },
    { 116, "patient-0016",  18 },
    { 117, "patient-0017",  25 },
    { 118, "patient-0018",  32 },
    { 119, "patient-0019",  39 },
    { 120, "patient-0020",  46 },
    { 121, "patient-0021",  53 },
    { 122, "patient-0022",  60 },
    { 123, "patient-0023",  67 },
    { 124, "patient-0024",  74 },
    { 125, "patient-0025",  81 },
    { 126, "patient-0026",  88 },
    { 127, "patient-0027",  95 },
    { 128, "patient-0028",   5 },
    { 129, "patient-0029",  12 },
    { 130, "patient-0030",  19 },
    { 131, "patient-0031",  26 },
    { 132, "patient-0032",  33 },
    { 133, "patient-0033",  40 },
    { 134, "patient-0034",  47 },
    { 135, "patient-0035",  54 },
    { 136, "patient-0036",  61 },
    { 137, "patient-0037",  68 },
    { 138, "patient-0038",  75 },
    { 139, "patient-0039",  82 },
    { 140, "patient-0040",  89 },
    { 141, "patient-0041",  96 },
    { 142, "patient-0042",   6 },
    { 143, "patient-0043",  13 },
    { 144, "patient-0044",  20 },
    { 145, "patient-0045",  27 },
    { 146, "patient-0046",  34 },
    { 147, "patient-0047",  41 },
    { 148, "patient-0048",  48 },
};

static struct patient patient_table[PATIENT_SLOTS];
static int patient_used;

void patient_reset(void)
{
    memset(patient_table, 0, sizeof(patient_table));
    patient_used = 0;
}

int patient_add(int key, const char *name, int age)
{
    struct patient *slot;

    if (patient_used >= PATIENT_SLOTS) {
        return -1;
    }

    slot = &patient_table[patient_used];
    slot->key = key;
    slot->age = age;
    strncpy(slot->name, name, PATIENT_NAME_SIZE - 1);
    slot->name[PATIENT_NAME_SIZE - 1] = '\0';
    patient_used++;

    return patient_used - 1;
}

struct patient *patient_find(int key)
{
    int i;

    for (i = 0; i < patient_used; i++) {
        if (patient_table[i].key == key) {
            return &patient_table[i];
        }
    }

    return NULL;
}

int patient_remove(int key)
{
    struct patient *found;

    found = patient_find(key);
    if (found == NULL) {
        return -1;
    }

    *found = patient_table[patient_used - 1];
    patient_used--;

    return 0;
}

int patient_total_age(void)
{
    int i;
    int total;

    total = 0;
    for (i = 0; i < patient_used; i++) {
        total += patient_table[i].age;
    }

    return total;
}

void patient_load_seed(void)
{
    size_t i;

    for (i = 0; i < sizeof(patient_seed) / sizeof(patient_seed[0]); i++) {
        patient_add(patient_seed[i].key, patient_seed[i].name,
                    patient_seed[i].age);
    }
}

void patient_report(FILE *out)
{
    int i;

    fprintf(out, "%d patients\n", patient_used);
    for (i = 0; i < patient_used; i++) {
        fprintf(out, "  %4d %-24s %6d\n", patient_table[i].key,
                patient_table[i].name, patient_table[i].age);
    }
    fprintf(out, "  total age %d\n", patient_total_age());
}

int main(void)
{
    patient_reset();
    patient_load_seed();
    patient_remove(103);
    patient_report(stdout);

    return 0;
}
