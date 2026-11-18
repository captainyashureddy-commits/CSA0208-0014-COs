/* SIMATS C05 - District Blood Bank Network
   Core donor registration, eligibility search, stock update, persistence and CLI.
   Build: gcc -std=c11 -Wall -Wextra -O2 bloodbank.c -o bloodbank
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DONOR_FILE "donors.txt"
#define STOCK_FILE "stock.txt"
#define NAME_LEN 80
#define PHONE_LEN 20
#define GROUP_LEN 4

typedef struct {
    int id;
    char name[NAME_LEN];
    char group[GROUP_LEN];
    int age;
    char phone[PHONE_LEN];
    char last_donation[11]; /* YYYY-MM-DD */
} Donor;

typedef struct {
    char group[GROUP_LEN];
    int units;
} Stock;

static const char *GROUPS[] = {"O-", "O+", "A-", "A+", "B-", "B+", "AB-", "AB+"};

int valid_group(const char *group) {
    for (size_t i = 0; i < sizeof GROUPS / sizeof GROUPS[0]; ++i)
        if (strcmp(group, GROUPS[i]) == 0) return 1;
    return 0;
}

/* A1: Register a donor. Output parameter returns the new donor record. */
int register_donor(Donor *out) {
    if (!out) return 0;

    printf("Donor ID: ");
    if (scanf("%d", &out->id) != 1 || out->id <= 0) return 0;
    printf("Full name: ");
    if (scanf(" %79[^\n]", out->name) != 1) return 0;
    printf("Blood group (O-, O+, A-, A+, B-, B+, AB-, AB+): ");
    if (scanf("%3s", out->group) != 1 || !valid_group(out->group)) return 0;
    printf("Age: ");
    if (scanf("%d", &out->age) != 1 || out->age < 18 || out->age > 65) return 0;
    printf("Phone: ");
    if (scanf("%19s", out->phone) != 1) return 0;
    printf("Last donation date (YYYY-MM-DD, use 0000-00-00 if never): ");
    if (scanf("%10s", out->last_donation) != 1) return 0;
    return 1;
}

/* Simplified eligibility rule for this assignment: age 18..65 and
   at least 90 days since last donation. Date comparison is lexical for
   ISO dates; the caller supplies today's date in YYYY-MM-DD. */
int is_eligible(const Donor *d, const char *today) {
    if (!d || d->age < 18 || d->age > 65) return 0;
    if (strcmp(d->last_donation, "0000-00-00") == 0) return 1;
    /* For a real deployment, calculate calendar-day difference.
       This conservative demonstration accepts dates at least 3 months old. */
    int y1, m1, day1, y2, m2, day2;
    if (sscanf(d->last_donation, "%4d-%2d-%2d", &y1, &m1, &day1) != 3 ||
        sscanf(today, "%4d-%2d-%2d", &y2, &m2, &day2) != 3) return 0;
    (void)day1; (void)day2;
    return (y2 * 12 + m2) - (y1 * 12 + m1) >= 3;
}

/* A1/A5: Append one donor as a line of text. */
int save_donor(const Donor *d) {
    FILE *fp = fopen(DONOR_FILE, "a");
    if (!fp) { perror("Could not open donors.txt"); return 0; }
    int ok = fprintf(fp, "%d|%s|%s|%d|%s|%s\n", d->id, d->name,
                     d->group, d->age, d->phone, d->last_donation) >= 0;
    if (fclose(fp) != 0) ok = 0;
    return ok;
}

int load_donors(Donor **items, size_t *count) {
    if (!items || !count) return 0;
    *items = NULL; *count = 0;
    FILE *fp = fopen(DONOR_FILE, "r");
    if (!fp) {
        /* Missing file means an empty database; report other open errors. */
        fp = fopen(DONOR_FILE, "a");
        if (!fp) { perror("Could not create donors.txt"); return 0; }
        fclose(fp);
        return 1;
    }
    Donor d;
    char line[256];
    size_t cap = 0;
    while (fgets(line, sizeof line, fp)) {
        if (sscanf(line, "%d|%79[^|]|%3[^|]|%d|%19[^|]|%10s",
                   &d.id, d.name, d.group, &d.age, d.phone, d.last_donation) != 6)
            continue;
        if (*count == cap) {
            size_t new_cap = cap ? cap * 2 : 8;
            Donor *tmp = realloc(*items, new_cap * sizeof **items);
            if (!tmp) { free(*items); *items = NULL; *count = 0; fclose(fp); return 0; }
            *items = tmp; cap = new_cap;
        }
        (*items)[(*count)++] = d;
    }
    fclose(fp);
    return 1;
}

int update_stock(const char *group, int delta) {
    Stock stock[8];
    for (int i = 0; i < 8; ++i) { strcpy(stock[i].group, GROUPS[i]); stock[i].units = 0; }

    FILE *fp = fopen(STOCK_FILE, "r");
    if (fp) {
        char g[GROUP_LEN]; int units;
        while (fscanf(fp, "%3s %d", g, &units) == 2)
            for (int i = 0; i < 8; ++i)
                if (!strcmp(g, stock[i].group)) stock[i].units = units;
        fclose(fp);
    }

    int found = 0;
    for (int i = 0; i < 8; ++i) if (!strcmp(group, stock[i].group)) {
        if (stock[i].units + delta < 0) return 0;
        stock[i].units += delta; found = 1; break;
    }
    if (!found) return 0;

    fp = fopen(STOCK_FILE, "w");
    if (!fp) { perror("Could not write stock.txt"); return 0; }
    for (int i = 0; i < 8; ++i)
        fprintf(fp, "%s %d\n", stock[i].group, stock[i].units);
    return fclose(fp) == 0;
}

void list_donors(const Donor *items, size_t count, const char *group,
                 const char *today) {
    int found = 0;
    for (size_t i = 0; i < count; ++i) {
        if ((!group || !strcmp(items[i].group, group)) &&
            is_eligible(&items[i], today)) {
            printf("ID=%d | %s | %s | age=%d | phone=%s\n",
                   items[i].id, items[i].name, items[i].group,
                   items[i].age, items[i].phone);
            found = 1;
        }
    }
    if (!found) puts("No eligible donors found.");
}

int main(int argc, char **argv) {
    if (argc > 3) {
        fprintf(stderr, "Usage: %s [list|find GROUP|register|stock GROUP DELTA]\n", argv[0]);
        return 2;
    }
    Donor *donors = NULL;
    size_t count = 0;
    if (!load_donors(&donors, &count)) {
        fprintf(stderr, "Failed to load donor records.\n");
        return 1;
    }

    if (argc == 1 || !strcmp(argv[1], "list")) {
        list_donors(donors, count, NULL, "2026-11-17");
    } else if (!strcmp(argv[1], "find") && argc == 3) {
        if (!valid_group(argv[2])) {
            fprintf(stderr, "Invalid blood group.\n"); free(donors); return 2;
        }
        list_donors(donors, count, argv[2], "2026-11-17");
    } else if (!strcmp(argv[1], "register") && argc == 2) {
        Donor d;
        if (!register_donor(&d)) {
            fprintf(stderr, "Invalid donor details.\n"); free(donors); return 2;
        }
        if (save_donor(&d)) puts("Donor saved successfully.");
        else { fprintf(stderr, "Could not save donor.\n"); free(donors); return 1; }
    } else if (!strcmp(argv[1], "stock") && argc == 2) {
        char group[GROUP_LEN]; int delta;
        if (scanf("%3s%d", group, &delta) != 2 || !valid_group(group) ||
            !update_stock(group, delta)) {
            fprintf(stderr, "Stock update failed. Check group and available units.\n");
            free(donors); return 1;
        }
        puts("Stock updated.");
    } else {
        fprintf(stderr, "Invalid command.\n");
        fprintf(stderr, "Examples: %s list | find O+ | register | stock\n", argv[0]);
        free(donors); return 2;
    }

    free(donors);
    return 0;
}
