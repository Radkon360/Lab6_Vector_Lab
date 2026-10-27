# MiniMat - 3D Vector Calculator

## Author
Konrad Swierczek

## Course
CPE-2600 Systems Programming

## Project Description

MiniMat is an interactive command-line vector calculator written in C. The program allows users to create, store, display, and manipulate three-dimensional vectors.

Vectors may be assigned names and stored in memory for later use. Supported operations include vector addition, vector subtraction, and scalar multiplication.

Beginning with Lab 7, vector storage uses dynamic memory allocation, allowing the program to store an arbitrary number of vectors until available memory is exhausted. Support has also been added for loading vectors from CSV files and saving vectors to CSV files.

## Features

### Vector Storage
- Create named vectors
- Replace existing vectors
- Display stored vectors
- List all vectors
- Clear all vectors

### Vector Operations
- Vector addition
- Vector subtraction
- Scalar multiplication
- Assignment of operation results

### Dynamic Memory
- Dynamically expands storage as vectors are added
- Releases all allocated memory when cleared
- Releases all allocated memory on program exit

### File I/O
- Load vectors from CSV files
- Save vectors to CSV files
- Handles missing files gracefully
- Handles malformed CSV entries gracefully

## Building

Compile the project:

    make

Remove object files and executable:

    make clean

## Running

Run the program:

    ./minimat

Display help information:

    ./minimat -h

## Commands

Create a Vector:

    a = 1 2 3

or

    a = 1,2,3

Display a Vector:

    a

Add Vectors:

    a + b

Subtract Vectors:

    a - b

Scalar Multiplication:

    a * 2

or

    2 * a

Assignment:

    c = a + b

    d = c * 3

List Vectors:

    list

Clear Vectors:

    clear

Load CSV File:

    load vectors.csv

Save CSV File:

    save output.csv

Exit Program:

    quit

## CSV File Format

Example:

    a,1,2,3
    b,4.4,5.5,6.6
    c,8.8,11,13.2

Format:

    name,x,y,z

## Source Files

vectormain.c
- Program entry point and command-line argument handling.

ui.c / ui.h
- User interface, command parsing, and console I/O.

vect.c / vect.h
- Vector math operations including addition, subtraction, and scalar multiplication.

vectstore.c / vectstore.h
- Vector storage management, dynamic memory allocation, and vector lookup functions.

Makefile
- Build automation.

## Testing

The program was tested for:

- Vector creation
- Vector replacement
- Vector addition
- Vector subtraction
- Scalar multiplication
- List command
- Clear command
- CSV loading
- CSV saving
- Invalid file handling
- Malformed CSV handling
- Memory leak detection using Valgrind

Example:

    valgrind --leak-check=full ./minimat

Expected result:

    All heap blocks were freed -- no leaks are possible

## Example Session

    minimat> a = 1 2 3
    a = 1 2 3

    minimat> b = 4 5 6
    b = 4 5 6

    minimat> c = a + b
    c = 5 7 9

    minimat> d = c * 2
    d = 10 14 18

    minimat> save vectors.csv

    minimat> quit
