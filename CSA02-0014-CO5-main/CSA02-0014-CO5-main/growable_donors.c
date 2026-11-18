/* SIMATS C05 - Growable donor array; B4 TODO completed.
   Build: gcc -std=c11 -Wall -Wextra -O2 growable_donors.c -o growable_donors
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int id; char group[4]; } Donor;
typedef struct { Donor *data; size_t size; size_t capacity; } DonorList;

/* Appends a donor, doubling capacity when full.
   A temporary pointer preserves the old allocation if realloc fails. */
int add_donor(DonorList *list, int id, const char *group) {
    if (!list || !group) return 0;

    if (list->size == list->capacity) {
        size_t new_capacity = (list->capacity == 0) ? 2 : list->capacity * 2;
        if (new_capacity < list->capacity ||
            new_capacity > ((size_t)-1) / sizeof(Donor)) return 0;

        Donor *tmp = realloc(list->data, new_capacity * sizeof(Donor));
        if (!tmp) return 0;
        list->data = tmp;
        list->capacity = new_capacity;
    }

    list->data[list->size].id = id;
    strncpy(list->data[list->size].group, group, sizeof list->data[list->size].group - 1);
    list->data[list->size].group[sizeof list->data[list->size].group - 1] = '\0';
    list->size++;
    return 1;
}

int main(void) {
    DonorList list = {NULL, 0, 0};
    const char *groups[] = {"O+", "A+", "B-", "AB+", "O-"};

    for (int i = 0; i < 5; ++i) {
        if (!add_donor(&list, 7001 + i, groups[i])) {
            fprintf(stderr, "Memory allocation failed.\n");
            free(list.data);
            return 1;
        }
        printf("added %d (%s): size = %zu, capacity = %zu\n",
               list.data[i].id, list.data[i].group, list.size, list.capacity);
    }

    free(list.data);
    list.data = NULL; list.size = list.capacity = 0;
    return 0;
}
