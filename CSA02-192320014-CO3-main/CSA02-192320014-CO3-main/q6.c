/* Q6 - A Directory of Names: Sorting and Searching Strings */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXN 500
#define LEN  30

static long long cmp_calls, chars_copied;

static int name_cmp(const char *a, const char *b) { cmp_calls++; return strcmp(a, b); }
static void name_copy(char *d, const char *s) { chars_copied += (long long)strlen(s) + 1; strcpy(d, s); }

/* Selection sort on the names themselves (swap = 3 strcpy calls) */
static void sort_names(char (*list)[LEN], int n)
{
    char tmp[LEN];
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++) if (name_cmp(list[j], list[m]) < 0) m = j;
        if (m != i) { name_copy(tmp, list[i]); name_copy(list[i], list[m]); name_copy(list[m], tmp); }
    }
}

/* Selection sort on an index array: names never move */
static void sort_indices(char (*list)[LEN], int idx[], int n)
{
    for (int i = 0; i < n; i++) idx[i] = i;
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++) if (name_cmp(list[idx[j]], list[idx[m]]) < 0) m = j;
        if (m != i) { int t = idx[i]; idx[i] = idx[m]; idx[m] = t; }
    }
}

/* Binary search on sorted names; returns index or -1, steps = comparisons */
static int search_name(char (*list)[LEN], int n, const char *key, int *steps)
{
    int low = 0, high = n - 1; *steps = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2, c = strcmp(key, list[mid]);
        (*steps)++;
        if (c == 0) return mid;
        if (c < 0) high = mid - 1; else low = mid + 1;
    }
    return -1;
}

/* Case-insensitive comparison */
static int ci_cmp(const char *a, const char *b)
{
    while (*a && *b) {
        int x = tolower((unsigned char)*a), y = tolower((unsigned char)*b);
        if (x != y) return x - y;
        a++; b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

static void remove_newline(char *s) { s[strcspn(s, "\n")] = '\0'; }

static unsigned long long seed = 99;
static unsigned rnd(void) { seed = seed * 6364136223846793005ULL + 1442695040888963407ULL; return (unsigned)(seed >> 33); }

static void make_names(char (*list)[LEN], int n)
{
    for (int i = 0; i < n; i++) {
        int len = 3 + rnd() % 8;
        for (int k = 0; k < len; k++) list[i][k] = (k == 0 ? 'A' : 'a') + rnd() % 26;
        list[i][len] = '\0';
    }
}

static void fgets_length_test(int L)
{
    char name[64], buf[LEN];
    for (int i = 0; i < L; i++) name[i] = 'A' + i % 26;
    name[L] = '\0';
    FILE *f = tmpfile();
    fprintf(f, "%s\n", name);
    rewind(f);
    fgets(buf, LEN, f);
    int has_nl = strchr(buf, '\n') != NULL, left = 0;
    while (fgetc(f) != EOF) left++;
    fclose(f);
    printf("length %d: stored %zu chars, newline read=%s, chars left in input=%d%s\n",
           L, strlen(buf) - has_nl, has_nl ? "yes" : "no", left,
           L >= 29 ? "  <-- name does not fit cleanly" : "");
}

int main(void)
{
    static char names[MAXN][LEN];
    int n = 0;
    char line[128];

    printf("Enter number of names, then one name per line:\n");
    if (fgets(line, sizeof line, stdin)) n = atoi(line);
    if (n < 1 || n > MAXN) {
        const char *demo[] = {"Ravi", "Anita", "Kiran", "Anita", "Zoya", "bob", "Meena", "Kiran"};
        n = 8;
        for (int i = 0; i < n; i++) strcpy(names[i], demo[i]);
        printf("(no valid input, using built-in sample of 8 names)\n");
    } else {
        for (int i = 0; i < n; i++) {
            if (!fgets(names[i], LEN, stdin)) { n = i; break; }
            remove_newline(names[i]);
        }
    }

    cmp_calls = 0; chars_copied = 0;
    sort_names(names, n);
    printf("\nSorted list:\n");
    for (int i = 0; i < n; i++) printf("%d: %s\n", i, names[i]);
    printf("strcmp calls=%lld, characters copied=%lld\n", cmp_calls, chars_copied);

    printf("\nDuplicates:\n");
    int dup = 0;
    for (int i = 1; i < n; i++)
        if (strcmp(names[i], names[i - 1]) == 0) { printf("  %s (positions %d and %d)\n", names[i], i - 1, i); dup++; }
    if (!dup) printf("  none\n");

    printf("\nBinary search:\n");
    const char *keys[] = {"Meena", "Anita", "Sunil"};
    for (int k = 0; k < 3; k++) {
        int st, p = search_name(names, n, keys[k], &st);
        printf("  %-6s -> index %d, comparisons = %d\n", keys[k], p, st);
    }

    printf("\n=== strcmp and case ===\n");
    printf("strcmp(\"apple\",\"Apple\") = %d  (>0 because 'a'=97 comes after 'A'=65)\n", strcmp("apple", "Apple") > 0 ? 1 : strcmp("apple", "Apple"));
    printf("ci_cmp (\"apple\",\"Apple\") = %d  (0 means equal ignoring case)\n", ci_cmp("apple", "Apple"));

    printf("\n=== fgets with char names[500][30] ===\n");
    fgets_length_test(28);
    fgets_length_test(29);
    fgets_length_test(30);

    printf("\n=== Measurement (Student A): names vs index sort ===\n");
    printf("%-6s | %-30s | %-30s\n", "n", "sort names (cmp / chars copied)", "sort indices (cmp / chars copied)");
    int sizes[] = {100, 500, 5000};
    for (int s = 0; s < 3; s++) {
        int m = sizes[s];
        char (*list)[LEN] = malloc((size_t)m * LEN);
        char (*list2)[LEN] = malloc((size_t)m * LEN);
        int *idx = malloc(m * sizeof(int));
        make_names(list, m);
        memcpy(list2, list, (size_t)m * LEN);
        cmp_calls = 0; chars_copied = 0; sort_names(list, m);
        long long c1 = cmp_calls, w1 = chars_copied;
        cmp_calls = 0; chars_copied = 0; sort_indices(list2, idx, m);
        printf("%-6d | %12lld / %-14lld | %12lld / %-14lld\n", m, c1, w1, cmp_calls, chars_copied);
        free(list); free(list2); free(idx);
    }
    return 0;
}
