/* Q4 - Two-Dimensional Arrays: District Rainfall Records */
#include <stdio.h>
#include <time.h>

static void row_totals(int r, int c, int a[r][c], long tot[])
{
    for (int i = 0; i < r; i++) {
        tot[i] = 0;
        for (int j = 0; j < c; j++) tot[i] += a[i][j];
    }
}

static void col_averages(int r, int c, int a[r][c], double avg[])
{
    for (int j = 0; j < c; j++) {
        long s = 0;
        for (int i = 0; i < r; i++) s += a[i][j];
        avg[j] = (double)s / r;
    }
}

static int max_cell(int r, int c, int a[r][c], int *mi, int *mj)
{
    int best = a[0][0]; *mi = 0; *mj = 0;
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            if (a[i][j] > best) { best = a[i][j]; *mi = i; *mj = j; }
    return best;
}

/* districts whose annual total is below 60% of the state average annual total */
static int deficient(int r, const long tot[], int out[])
{
    double mean = 0; int k = 0;
    for (int i = 0; i < r; i++) mean += tot[i];
    mean /= r;
    for (int i = 0; i < r; i++) if (tot[i] < 0.6 * mean) out[k++] = i;
    return k;
}

static void report(int r, int c, int a[r][c], int verbose_rows)
{
    long tot[r]; double avg[c]; int def[r];
    row_totals(r, c, a, tot);
    col_averages(r, c, a, avg);
    int mi, mj, mx = max_cell(r, c, a, &mi, &mj);
    int nd = deficient(r, tot, def);

    for (int i = 0; i < verbose_rows && i < r; i++) printf("District %d total = %ld\n", i, tot[i]);
    for (int j = 0; j < c; j++) printf("Month %d average = %.2f\n", j + 1, avg[j]);
    printf("Wettest: district %d, month %d, %d mm\n", mi, mj + 1, mx);
    printf("Deficient districts (<60%% of state average):");
    if (nd == 0) printf(" none");
    for (int k = 0; k < nd; k++) printf(" %d", def[k]);
    printf("\n");
}

static int big[2000][2000];

int main(void)
{
    /* ---- Test 1: small 3 x 4 array, checked by hand ---- */
    int small[3][4] = {{10, 20, 30, 40}, {5, 5, 5, 5}, {50, 60, 70, 80}};
    printf("=== 3 x 4 hand-check ===\n");
    report(3, 4, small, 3);

    /* ---- Test 2: full 38 x 12 array ---- */
    static int rain[38][12];
    for (int i = 0; i < 38; i++)
        for (int j = 0; j < 12; j++)
            rain[i][j] = 20 + (i * 7 + j * 13) % 90;
    rain[5][6] = 400;                       /* a very wet district-month */
    for (int j = 0; j < 12; j++) rain[17][j] = 3;   /* a very dry district */
    printf("\n=== 38 x 12 rainfall ===\n");
    report(38, 12, rain, 3);

    /* ---- Memory layout (row-major) ---- */
    printf("\n=== Row-major check for rain[5][7] ===\n");
    int *base = &rain[0][0];
    printf("rain[5][7]=%d, *(base + 5*12 + 7)=%d, byte offset=%ld (expected %ld)\n",
           rain[5][7], *(base + 5 * 12 + 7),
           (long)((char *)&rain[5][7] - (char *)base), (long)((5 * 12 + 7) * sizeof(int)));

    /* ---- Timing: row-wise vs column-wise on 2000 x 2000 ---- */
    for (int i = 0; i < 2000; i++)
        for (int j = 0; j < 2000; j++) big[i][j] = (i + j) % 10;

    clock_t t0 = clock(); long sum1 = 0;
    for (int rep = 0; rep < 5; rep++)
        for (int i = 0; i < 2000; i++)
            for (int j = 0; j < 2000; j++) sum1 += big[i][j];
    double row_ms = (clock() - t0) * 1000.0 / CLOCKS_PER_SEC;

    t0 = clock(); long sum2 = 0;
    for (int rep = 0; rep < 5; rep++)
        for (int j = 0; j < 2000; j++)
            for (int i = 0; i < 2000; i++) sum2 += big[i][j];
    double col_ms = (clock() - t0) * 1000.0 / CLOCKS_PER_SEC;

    printf("\n=== Traversal time, 2000 x 2000, 5 passes ===\n");
    printf("row-wise   : %.1f ms (sum=%ld)\n", row_ms, sum1);
    printf("column-wise: %.1f ms (sum=%ld)\n", col_ms, sum2);
    return 0;
}
