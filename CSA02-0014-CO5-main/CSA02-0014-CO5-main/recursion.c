/* SIMATS C05 - Recursive functions, call counts and iterative comparison.
   Build: gcc -std=c11 -Wall -Wextra -O2 recursion.c -o recursion
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static unsigned long long linear_calls;
static unsigned long long fast_calls;

int digital_root_recursive(unsigned long long n) {
    if (n < 10) return (int)n;                 /* base case */
    unsigned long long sum = 0;
    while (n) { sum += n % 10; n /= 10; }
    return digital_root_recursive(sum);        /* recursive case */
}

long long power_linear(long long base, int exp) {
    ++linear_calls;
    if (exp == 0) return 1;
    return base * power_linear(base, exp - 1);
}

/* TODO B1 completed: halve the exponent once, then square the half-result. */
long long power_fast(long long base, int exp) {
    ++fast_calls;
    if (exp == 0) return 1;
    long long half = power_fast(base, exp / 2);
    long long result = half * half;
    return (exp % 2 == 0) ? result : base * result;
}

long long power_iterative(long long base, unsigned int exp) {
    long long result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        exp >>= 1U;
        if (exp) base *= base;
    }
    return result;
}

int main(void) {
    unsigned long long number;
    long long base;
    int exp;

    printf("Enter a non-negative integer for digital root: ");
    if (scanf("%llu", &number) != 1) return 1;
    printf("Digital root = %d\n", digital_root_recursive(number));

    printf("Enter base and exponent (0..30): ");
    if (scanf("%lld%d", &base, &exp) != 2 || exp < 0 || exp > 30) {
        fprintf(stderr, "Invalid input. Exponent must be between 0 and 30.\n");
        return 1;
    }

    linear_calls = fast_calls = 0;
    long long linear_result = power_linear(base, exp);
    printf("Linear result: %lld in %llu calls\n", linear_result, linear_calls);
    long long fast_result = power_fast(base, exp);
    printf("Fast result:   %lld in %llu calls\n", fast_result, fast_calls);
    printf("Iterative result: %lld\n", power_iterative(base, (unsigned)exp));

    puts("\nCall-count note: linear recursion uses exp+1 calls;");
    puts("halving recursion uses floor(log2(exp))+1 calls.");
    puts("Avoid large exponents: integer overflow is not checked in this demo.");
    return 0;
}
