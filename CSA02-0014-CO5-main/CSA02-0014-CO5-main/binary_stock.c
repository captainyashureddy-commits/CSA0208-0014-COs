/* SIMATS C05 - Fixed-size binary records and safe in-place update.
   Build: gcc -std=c11 -Wall -Wextra -O2 binary_stock.c -o binary_stock
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int group_code;
    int units;
} Stock;

/* B3 TODO completed: validate index, seek/read, modify, seek back/write. */
int update_units(FILE *fp, int index, int units) {
    if (!fp || index < 0 || units < 0) return 0;

    Stock record;
    long offset = (long)index * (long)sizeof(Stock);

    if (fseek(fp, offset, SEEK_SET) != 0) return 0;
    if (fread(&record, sizeof record, 1, fp) != 1) return 0;

    record.units = units;

    if (fseek(fp, offset, SEEK_SET) != 0) return 0;
    if (fwrite(&record, sizeof record, 1, fp) != 1) return 0;
    return fflush(fp) == 0;
}

int main(void) {
    FILE *fp = tmpfile();
    if (!fp) { perror("tmpfile"); return 1; }

    Stock initial[5] = {{1, 40}, {2, 25}, {3, 12}, {4, 8}, {5, 30}};
    if (fwrite(initial, sizeof(Stock), 5, fp) != 5) {
        perror("fwrite"); fclose(fp); return 1;
    }

    printf("update record 3: %s\n", update_units(fp, 2, 20) ? "done" : "failed");
    printf("update record 9: %s\n", update_units(fp, 8, 20) ? "done" : "failed");

    rewind(fp);
    Stock s;
    while (fread(&s, sizeof s, 1, fp) == 1)
        printf("%d:%d ", s.group_code, s.units);
    putchar('\n');

    fclose(fp);
    return 0;
}
