# SIMATS C05 – C Programming Assignment

This repository contains separate C programs for the implementation tasks visible in the assignment screenshots.

## Files

- `bloodbank.c` – donor registration, donor file persistence, eligibility filtering, blood-group stock updates and command-line interface.
- `recursion.c` – recursive digital root, linear power, divide-and-conquer power, iterative power and call counters.
- `binary_stock.c` – fixed-size binary stock records and safe in-place update using `fseek`, `fread` and `fwrite`.
- `growable_donors.c` – dynamic donor array using `realloc`, capacity doubling and allocation-failure handling.

## Compile

Use GCC:

```bash
gcc -std=c11 -Wall -Wextra -O2 bloodbank.c -o bloodbank
gcc -std=c11 -Wall -Wextra -O2 recursion.c -o recursion
gcc -std=c11 -Wall -Wextra -O2 binary_stock.c -o binary_stock
gcc -std=c11 -Wall -Wextra -O2 growable_donors.c -o growable_donors
```

## Run

```bash
./bloodbank list
./bloodbank register
./bloodbank find O+
./bloodbank stock
./recursion
./binary_stock
./growable_donors
```

For the stock command, enter a blood group and signed unit change when prompted, for example `O+ 5`.

On Windows, replace `./program` with `program.exe` after compiling.

## Important assignment notes

1. The blood-bank example uses a simplified eligibility rule: age 18–65 and at least three calendar months since the last donation. A production system must calculate the exact day interval and apply the current medical eligibility rules.
2. `recursion.c` demonstrates call counts. Large integer powers can overflow the `long long` type; the demo limits the exponent to 30 but does not implement arbitrary-precision arithmetic.
3. `binary_stock.c` uses `tmpfile()` so the example is self-contained. Replace it with `fopen("stock.bin", "r+b")` when working with a persistent binary file.
4. Binary files containing raw C structures are not guaranteed portable across different compilers or machine architectures because of padding, integer size and byte order.
5. The code prints observable results. For the assignment's performance sections, run tests at the requested data sizes, record timings on your own computer, and report those measured values rather than inventing benchmark numbers.
6. To test memory safety, compile with AddressSanitizer where supported:
   `gcc -std=c11 -g -fsanitize=address -fno-omit-frame-pointer growable_donors.c -o growable_donors_asan`
