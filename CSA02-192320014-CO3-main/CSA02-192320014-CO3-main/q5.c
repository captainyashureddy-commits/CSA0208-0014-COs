/* Q5 - Processing Text Without the Library (Part 1) and With It (Part 2) */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

/* ================= PART 1: without string.h ================= */

/* Counts words separated by one or more whitespace characters (space, tab...) */
int word_count(const char s[])
{
    int count = 0, inside = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (isspace((unsigned char)s[i])) inside = 0;
        else if (!inside) { inside = 1; count++; }
    }
    return count;
}

void to_upper(char s[])
{
    for (int i = 0; s[i] != '\0'; i++)
        if (s[i] >= 'a' && s[i] <= 'z') s[i] = s[i] - 'a' + 'A';
}

void reverse_str(char s[])
{
    int left = 0, right = 0;
    while (s[right] != '\0') right++;
    right--;
    while (left < right) { char t = s[left]; s[left] = s[right]; s[right] = t; left++; right--; }
}

/* Returns 1 if s reads the same both ways, ignoring case and non-letters. */
int is_palindrome(const char s[])
{
    int left = 0, right = 0;
    while (s[right] != '\0') right++;
    right--;
    while (left < right) {
        while (left < right && !isalpha((unsigned char)s[left]))  left++;
        while (left < right && !isalpha((unsigned char)s[right])) right--;
        if (tolower((unsigned char)s[left]) != tolower((unsigned char)s[right])) return 0;
        left++; right--;
    }
    return 1;
}

/* ================= PART 2: with string.h ================= */

void to_upper_lib(char s[])
{
    for (size_t i = 0, n = strlen(s); i < n; i++) s[i] = (char)toupper((unsigned char)s[i]);
}

void reverse_lib(char s[])
{
    size_t n = strlen(s);
    for (size_t i = 0; i < n / 2; i++) { char t = s[i]; s[i] = s[n - 1 - i]; s[n - 1 - i] = t; }
}

int is_palindrome_lib(const char s[])
{
    char clean[256], rev[256];
    size_t k = 0;
    for (size_t i = 0, n = strlen(s); i < n && k < 255; i++)
        if (isalpha((unsigned char)s[i])) clean[k++] = (char)tolower((unsigned char)s[i]);
    clean[k] = '\0';
    strcpy(rev, clean);
    reverse_lib(rev);
    return strcmp(clean, rev) == 0;
}

int main(void)
{
    printf("=== PART 1 (no string.h) ===\n");
    printf("Words: %d\n", word_count(" Meet at the library at 5 PM "));
    printf("%d %d\n", is_palindrome("Madam, I'm Adam"), is_palindrome("Hello"));

    printf("\nTab test      : %d words (expected 3)\n", word_count("Hello\tworld  again"));
    printf("Empty message : %d words (expected 0)\n", word_count(""));
    printf("Only spaces   : %d words (expected 0)\n", word_count("     "));
    printf("Punctuation   : %d words (expected 3)\n", word_count("Great, fun... lab!"));

    char a[50] = "hello 123 World";
    to_upper(a);   printf("Upper   : %s\n", a);
    reverse_str(a); printf("Reversed: %s\n", a);

    printf("\n=== PART 2 (with string.h) ===\n");
    char b[50] = "hello 123 World";
    to_upper_lib(b);   printf("Upper   : %s\n", b);
    reverse_lib(b);    printf("Reversed: %s\n", b);
    printf("%d %d\n", is_palindrome_lib("Madam, I'm Adam"), is_palindrome_lib("Hello"));

    printf("\n=== Safety demo (Student B) ===\n");
    const char *longmsg = "This is a very long feedback message";
    char small[8];
    strncpy(small, longmsg, sizeof small - 1);
    small[sizeof small - 1] = '\0';
    printf("strncpy into char[8] : \"%s\"\n", small);
    snprintf(small, sizeof small, "%s", longmsg);
    printf("snprintf into char[8]: \"%s\"\n", small);
    /* strcpy(small, longmsg);  <-- UNSAFE: writes past the array (buffer overflow). Do not run. */

    printf("\n=== strlen in loop condition (Alternative analysis) ===\n");
    static char big[50001];
    memset(big, 'a', 50000); big[50000] = '\0';
    long c1 = 0, c2 = 0;
    clock_t t0 = clock();
    for (size_t i = 0; i < strlen(big); i++) c1 += big[i];        /* O(n^2) */
    double slow = (clock() - t0) * 1000.0 / CLOCKS_PER_SEC;
    t0 = clock();
    for (size_t i = 0, n = strlen(big); i < n; i++) c2 += big[i]; /* O(n)   */
    double fast = (clock() - t0) * 1000.0 / CLOCKS_PER_SEC;
    printf("strlen in condition: %.2f ms | strlen once: %.2f ms | same result: %d\n", slow, fast, c1 == c2);

    printf("\n=== Your own message (type a line, then Enter) ===\n");
    char msg[256];
    if (fgets(msg, sizeof msg, stdin)) {
        msg[strcspn(msg, "\n")] = '\0';
        printf("Words     : %d\n", word_count(msg));
        printf("Palindrome: %d\n", is_palindrome(msg));
        to_upper(msg);   printf("Upper     : %s\n", msg);
        reverse_str(msg); printf("Reversed  : %s\n", msg);
    }
    return 0;
}
