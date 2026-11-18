/* Q3 - Merging Two Merit Lists (descending order) */
#include <stdio.h>
#include <stdlib.h>

static long long mcmp;   /* comparisons made by merge */
static long long qcmp;   /* comparisons made by qsort */

/* a[] and b[] are merit lists sorted in DESCENDING order of score.
   Copies both into out[] in descending order and returns the total count. */
int merge(const int a[], int na, const int b[], int nb, int out[])
{
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {
        mcmp++;
        if (a[i] >= b[j]) out[k++] = a[i++];   /* ties: campus A first (stable) */
        else              out[k++] = b[j++];
    }
    while (i < na) out[k++] = a[i++];
    while (j < nb) out[k++] = b[j++];
    return k;
}

static int is_desc(const int x[], int n)
{
    for (int i = 1; i < n; i++) if (x[i - 1] < x[i]) return 0;
    return 1;
}

static void show(const char *label, const int x[], int n)
{
    printf("%s", label);
    for (int i = 0; i < n; i++) printf(" %d", x[i]);
    printf("\n");
}

static int desc_cmp(const void *p, const void *q)
{
    int x = *(const int *)p, y = *(const int *)q;
    qcmp++;
    return (y > x) - (y < x);
}

static void compare(int m)
{
    int *a = malloc(m * sizeof(int)), *b = malloc(m * sizeof(int));
    int *out = malloc(2 * m * sizeof(int));
    for (int i = 0; i < m; i++) { a[i] = 2 * (m - i); b[i] = 2 * (m - i) + 1; }

    mcmp = 0;
    int n = merge(a, m, b, m, out);
    long long merge_c = mcmp;

    for (int i = 0; i < m; i++) { out[i] = a[i]; out[m + i] = b[i]; }
    qcmp = 0;
    qsort(out, 2 * m, sizeof(int), desc_cmp);

    printf("m=n=%5d | merge comparisons=%8lld (max m+n-1=%d) | join+qsort comparisons=%8lld | merged ok=%d\n",
           m, merge_c, 2 * m - 1, qcmp, is_desc(out, n));
    free(a); free(b); free(out);
}

int main(void)
{
    int campusA[] = {98, 91, 85, 72};
    int campusB[] = {95, 91, 80};
    int merged[7];
    int n = merge(campusA, 4, campusB, 3, merged);
    for (int i = 0; i < n; i++) printf("%d ", merged[i]);
    printf("\n");

    printf("\n--- Edge tests ---\n");
    int e[1] = {0}, out[8];
    n = merge(e, 0, campusB, 3, out);   show("A empty  :", out, n);
    n = merge(campusA, 4, e, 0, out);   show("B empty  :", out, n);
    int eq1[] = {50, 50, 50}, eq2[] = {50, 50};
    n = merge(eq1, 3, eq2, 2, out);     show("all equal:", out, n);
    printf("Different lengths (4 and 3) handled: %d elements, sorted=%d\n", 7, is_desc(merged, 7));

    printf("\n--- Merge vs join + qsort ---\n");
    compare(1000);
    compare(10000);
    return 0;
}
