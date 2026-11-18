/* Q7 - Student Council Election Counting Program (designed from first principles) */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXC 12
#define MAXV 5000

static unsigned long long seed = 2024;
static unsigned rnd(void) { seed = seed * 6364136223846793005ULL + 1442695040888963407ULL; return (unsigned)(seed >> 33); }

int main(void)
{
    int c, v;
    int count[MAXC + 1] = {0};   /* count[i] = votes for candidate i (index 0 unused) */
    int valid = 0, invalid = 0;

    printf("Enter number of candidates (1-%d) and number of votes (0-%d): ", MAXC, MAXV);
    if (scanf("%d %d", &c, &v) != 2 || c < 1 || c > MAXC || v < 0 || v > MAXV) {
        printf("Invalid setup.\n");
        return 1;
    }
    printf("Enter %d votes:\n", v);
    for (int i = 0; i < v; i++) {
        int x;
        if (scanf("%d", &x) != 1) { printf("Input ended early.\n"); return 1; }
        if (x < 1 || x > c) invalid++;          /* 0, negative or above c */
        else { count[x]++; valid++; }           /* one pass: count array indexed by candidate */
    }

    /* sort candidates by votes, descending (insertion sort, ties keep lower number first) */
    int order[MAXC];
    for (int i = 0; i < c; i++) order[i] = i + 1;
    for (int i = 1; i < c; i++) {
        int key = order[i], j = i - 1;
        while (j >= 0 && count[order[j]] < count[key]) { order[j + 1] = order[j]; j--; }
        order[j + 1] = key;
    }

    printf("\nValid votes  : %d\nInvalid votes: %d\n\nResults (descending):\n", valid, invalid);
    for (int i = 0; i < c; i++) printf("  Candidate %2d : %d\n", order[i], count[order[i]]);

    if (valid == 0) {
        printf("\nNo valid votes, no winner.\n");
    } else {
        int top = count[order[0]], tied = 0;
        for (int i = 0; i < c; i++) if (count[order[i]] == top) tied++;
        if (tied > 1) {
            printf("\nTIE between candidates:");
            for (int i = 0; i < tied; i++) printf(" %d", order[i]);
            printf(" with %d votes each\n", top);
        } else {
            printf("\nWinner: Candidate %d with %d votes\n", order[0], top);
        }
    }

    /* ---- Stage 3: timing with 5,000 and 500,000 generated votes ---- */
    printf("\n=== Timing (12 candidates, random votes incl. invalid) ===\n");
    int sizes[] = {5000, 500000}, reps[] = {20000, 200};
    for (int s = 0; s < 2; s++) {
        long long check = 0;
        clock_t t0 = clock();
        for (int r = 0; r < reps[s]; r++) {
            int cnt[13] = {0};
            for (int i = 0; i < sizes[s]; i++) {
                int x = (int)(rnd() % 16) - 2;      /* -2 .. 13 */
                if (x >= 1 && x <= 12) cnt[x]++;
            }
            check += cnt[1];
        }
        double ms = (clock() - t0) * 1000.0 / CLOCKS_PER_SEC / reps[s];
        printf("%7d votes: %.4f ms per count (checksum %lld)\n", sizes[s], ms, check);
    }
    return 0;
}
