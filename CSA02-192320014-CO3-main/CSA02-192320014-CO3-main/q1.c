/* Q1 - Searching a Library Catalogue */
#include <stdio.h>
#include <stdlib.h>

int steps; /* comparisons made by the last search */

int linear_search(const int a[], int n, int key)
{
    steps = 0;
    for (int i = 0; i < n; i++) {
        steps++;
        if (a[i] == key) return i;
    }
    return -1;
}

/* a[] is sorted in ascending order. Returns the index of key, or -1. */
int binary_search(const int a[], int n, int key)
{
    int low = 0, high = n - 1;
    steps = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;   /* no overflow */
        steps++;                            /* one comparison with a[mid] */
        if (a[mid] == key) return mid;
        if (a[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

/* Student B: experiment on n books (odd numbers, so even keys are missing) */
static void experiment(int n)
{
    int *a = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) a[i] = 2 * i + 1;

    int stride = n / 1000; if (stride < 1) stride = 1;
    long long bh = 0, bm = 0, lh = 0, lm = 0;
    int bh_max = 0, bm_max = 0, samples = 0;

    for (int i = 0; i < n; i += stride) {
        binary_search(a, n, a[i]);     bh += steps; if (steps > bh_max) bh_max = steps;
        binary_search(a, n, a[i] + 1); bm += steps; if (steps > bm_max) bm_max = steps;
        linear_search(a, n, a[i]);     lh += steps;
        linear_search(a, n, a[i] + 1); lm += steps;
        samples++;
    }
    int pred = 0, t = n;
    while (t > 0) { pred++; t /= 2; }   /* floor(log2 n) + 1 */

    printf("n=%6d | binary found avg=%.2f max=%d | binary missing avg=%.2f max=%d | predicted worst=%d\n",
           n, (double)bh / samples, bh_max, (double)bm / samples, bm_max, pred);
    printf("         | linear found avg=%.1f | linear missing avg=%.1f\n",
           (double)lh / samples, (double)lm / samples);
    free(a);
}

int main(void)
{
    int accession[] = {1021, 1048, 1103, 1190, 1222, 1305, 1377, 1402, 1460, 1533};
    int n = sizeof accession / sizeof accession[0];
    int keys[] = {1021, 1305, 1533, 1400};

    printf("--- Binary search (given test) ---\n");
    for (int k = 0; k < 4; k++) {
        int pos = binary_search(accession, n, keys[k]);
        printf("Book %d -> index %d, comparisons = %d\n", keys[k], pos, steps);
    }

    printf("\n--- Linear search on the same data ---\n");
    for (int k = 0; k < 4; k++) {
        int pos = linear_search(accession, n, keys[k]);
        printf("Book %d -> index %d, comparisons = %d\n", keys[k], pos, steps);
    }

    printf("\n--- Experiment (Student B) ---\n");
    experiment(1000);
    experiment(10000);
    experiment(100000);
    return 0;
}
