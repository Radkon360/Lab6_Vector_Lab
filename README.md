# minimat – Vector Calculator

**Author:** Konrad Swierczek
**Course:** CPE-2600 Systems Programming, Fall 2026

## Description

`minimat` is an interactive command-line calculator for 3D vectors written in C.
Vectors are stored by name. At the `minimat>` prompt you create vectors, combine
them with `+`, `-`, and `*`, and view, save, or load them.

Each line is split into tokens (commas are treated as spaces), then the UI layer
decides whether it is an assignment, an expression, or a command. Spaces are
required around `=`, `+`, `-`, and `*`.

Version 2.0 adds:

- Dynamic memory storage for vectors (no fixed limit)
- Loading and saving vectors to and from CSV files

Version 1.0 (Lab 5) used a fixed-size storage array and no file I/O.

## Source Files

| File | Purpose |
| --- | --- |
| `vectormain.c` | Entry point, `-h` handling, exit cleanup |
| `ui.c` / `ui.h` | Prompt, parser, and all console I/O |
| `vect.c` / `vect.h` | Vector math (add, subtract, scale) |
| `vectstore.c` / `vectstore.h` | Dynamic vector storage |
| `csvio.c` / `csvio.h` | CSV load and save |

## Build

```
gcc vectormain.c ui.c vect.c vectstore.c csvio.c -o minimat
```

## Run

```
./minimat
```

Command line options:

- `-h` : print the help message and exit

Any other option prints an error and exits with status 1.

## Commands

1. `name = x y z` : create or replace a vector (commas also work, e.g. `a = 1,2,3`)
2. `name` : display a vector
3. `a + b` : add two vectors
4. `a - b` : subtract two vectors
5. `a * n` or `n * a` : multiply a vector by a number
6. `c = a + b` : store the result of an operation (also works with `-` and `*`)
7. `list` : list all stored vectors
8. `clear` : remove all stored vectors
9. `load <file>` : load vectors from a CSV file (format: `name,x,y,z`)
10. `save <file>` : save all vectors to a CSV file
11. `quit` : exit the program

Notes:

- An operation without an assignment is shown as `ans` and is not stored.
- Vector names start with a letter or underscore, contain only letters, digits,
  and underscores, and cannot be `quit`, `clear`, `list`, `load`, or `save`.
- File names cannot contain spaces.
- `load` merges into memory; a loaded vector with an existing name replaces it.
- `save` replaces an existing file.

## Dynamic Memory

Vectors are kept in a single heap-allocated array owned by `vectstore.c`.

- The pointer starts as `NULL` with a capacity of 0.
- The first `addvect()` call allocates room for 4 vectors with `realloc()`.
- Whenever the array is full, `addvect()` doubles the capacity with `realloc()`.
- The result of `realloc()` goes into a temporary pointer first, so a failed
  allocation leaves the original block valid and `addvect()` returns -1.
- Adding a vector whose name already exists replaces it in place, with no growth.
- `clearvects()` calls `free()` on the array and resets the pointer, count, and
  capacity, so storage can be reused afterward.
- `clear` calls `clearvects()` directly.
- `main()` registers `clearvects()` with `atexit()`, so memory is freed on
  `quit`, end of input (Ctrl-D), and Ctrl-C. A `SIGINT` handler only sets a
  flag, so the program leaves through the normal exit path.

## Version Tags

- `v1.0` : Lab 5 version (fixed-size storage, no file I/O)
- `v2.0` : Lab 7 version (dynamic memory and CSV load/save)

