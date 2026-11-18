/* Q2 - Sorting the Examination Hall Seating Register */
#include <stdio.h>
#include <stdlib.h>

typedef struct { long long cmp, swp; } Stats;
static int trace = 0;

static void print_arr(const char *label, const int a[], int n)
{
    printf("%s", label);
    for (int i = 0; i < n; i++) printf(" %d", a[i]);
    printf("\n");
}

/* Bubble sort with early-exit flag */
void bubble_sort(int a[], int n, Stats *s)
{
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            s->cmp++;
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
                s->swp++; swapped = 1;
            }
        }
        if (trace) { char lab[32]; snprintf(lab, sizeof lab, "  pass %d:", i + 1); print_arr(lab, a, n); }
        if (!swapped) break;            /* already sorted: stop early */
    }
}

void selection_sort(int a[], int n, Stats *s)
{
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++) {
            s->cmp++;
            if (a[j] < a[m]) m = j;
        }
        if (m != i) { int t = a[i]; a[i] = a[m]; a[m] = t; s->swp++; }
        if (trace) { char lab[32]; snprintf(lab, sizeof lab, "  pass %d:", i + 1); print_arr(lab, a, n); }
    }
}

/* swp counts shifts for insertion sort */
void insertion_sort(int a[], int n, Stats *s)
{
    for (int i = 1; i < n; i++) {
        int key = a[i], j = i - 1;
        while (j >= 0) {
            s->cmp++;
            if (a[j] > key) { a[j + 1] = a[j]; s->swp++; j--; }
            else break;
        }
        a[j + 1] = key;
        if (trace) { char lab[32]; snprintf(lab, sizeof lab, "  pass %d:", i); print_arr(lab, a, n); }
    }
}

/* Alternative: counting sort (values in 0..k), stable */
void counting_sort(const int a[], int n, int k, int out[])
{
    int *cnt = calloc(k + 1, sizeof(int));
    for (int i = 0; i < n; i++) cnt[a[i]]++;
    for (int v = 1; v <= k; v++) cnt[v] += cnt[v - 1];
    for (int i = n - 1; i >= 0; i--) out[--cnt[a[i]]] = a[i];
    free(cnt);
}

static unsigned long long seed = 12345;
static unsigned rnd(void)
{
    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    return (unsigned)(seed >> 33);
}

/* order: 0 sorted, 1 nearly sorted, 2 reverse, 3 random */
static void gen(int a[], int n, int order)
{
    for (int i = 0; i < n; i++) a[i] = i + 1;
    if (order == 1) {
        int k = n / 50; if (k < 1) k = 1;
        for (int t = 0; t < k; t++) {
            int i = rnd() % n, j = rnd() % n, x = a[i]; a[i] = a[j]; a[j] = x;
        }
    } else if (order == 2) {
        for (int i = 0; i < n; i++) a[i] = n - i;
    } else if (order == 3) {
        for (int i = 0; i < n; i++) a[i] = rnd() % (n * 10);
    }
}

int main(void)
{
    int reg[6] = {45, 12, 78, 23, 56, 9};
    int tmp[6];
    Stats s;

    printf("=== Trace on 6 register numbers ===\n");
    trace = 1;
    printf("Bubble:\n");    for (int i = 0; i < 6; i++) tmp[i] = reg[i]; s = (Stats){0, 0}; bubble_sort(tmp, 6, &s);
    printf("  comparisons=%lld swaps=%lld\n", s.cmp, s.swp);
    printf("Selection:\n"); for (int i = 0; i < 6; i++) tmp[i] = reg[i]; s = (Stats){0, 0}; selection_sort(tmp, 6, &s);
    printf("  comparisons=%lld swaps=%lld\n", s.cmp, s.swp);
    printf("Insertion:\n"); for (int i = 0; i < 6; i++) tmp[i] = reg[i]; s = (Stats){0, 0}; insertion_sort(tmp, 6, &s);
    printf("  comparisons=%lld shifts=%lld\n", s.cmp, s.swp);
    trace = 0;

    printf("\n=== Measurements (comparisons / swaps-or-shifts) ===\n");
    const char *names[] = {"sorted", "nearly", "reverse", "random"};
    int sizes[] = {100, 1000, 10000};
    printf("%-6s %-8s | %-19s | %-19s | %-19s | best\n", "n", "order", "Bubble", "Selection", "Insertion");
    for (int si = 0; si < 3; si++) {
        int n = sizes[si];
        int *base = malloc(n * sizeof(int)), *w = malloc(n * sizeof(int));
        for (int o = 0; o < 4; o++) {
            gen(base, n, o);
            Stats r[3] = {{0,0},{0,0},{0,0}};
            for (int i = 0; i < n; i++) w[i] = base[i]; bubble_sort(w, n, &r[0]);
            for (int i = 0; i < n; i++) w[i] = base[i]; selection_sort(w, n, &r[1]);
            for (int i = 0; i < n; i++) w[i] = base[i]; insertion_sort(w, n, &r[2]);
            int best = 0;
            for (int k = 1; k < 3; k++)
                if (r[k].cmp + r[k].swp < r[best].cmp + r[best].swp) best = k;
            const char *bn[] = {"Bubble", "Selection", "Insertion"};
            printf("%-6d %-8s | %9lld/%-9lld | %9lld/%-9lld | %9lld/%-9lld | %s\n",
                   n, names[o], r[0].cmp, r[0].swp, r[1].cmp, r[1].swp, r[2].cmp, r[2].swp, bn[best]);
        }
        free(base); free(w);
    }

    printf("\n=== Counting sort demo (values 0..9) ===\n");
    int c[8] = {4, 2, 2, 8, 3, 3, 1, 9}, o[8];
    counting_sort(c, 8, 9, o);
    print_arr("sorted:", o, 8);
    return 0;
}
