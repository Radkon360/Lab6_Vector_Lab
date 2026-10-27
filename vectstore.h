/***********************************************
 * Filename: vectstore.h
 * Author: Konrad Swierczek
 * Compile: gcc vectormain.c ui.c vect.c vectstore.c csvio.c -o minimat
 * Run: ./minimat
 ***********************************************/

/*
 * vectstore.h
 * Storage layer: holds named vectors in a dynamically allocated array
 * that grows as needed (limited only by available heap memory).
 * The array itself is static inside vectstore.c, so it is visible
 * only to these functions (no global variable).
 */
#ifndef VECTSTORE_H
#define VECTSTORE_H

#include "vect.h"

/* Add v, or replace the stored vector with the same name.
 * Returns 0 on success, -1 if memory could not be allocated. */
int addvect(vect v);

/* Copy the vector called name into *out.
 * Returns 0 if found, -1 if not found. */
int findvect(const char *name, vect *out);

/* Free all vector storage. Safe to call repeatedly (clear, and at exit). */
void clearvects(void);

/* Number of vectors currently stored. */
int vectcount(void);

/* Copy the vector in slot index (0 .. vectcount()-1) into *out.
 * Returns 0 on success, -1 if index is out of range.
 * Lets the UI and CSV code walk the list without touching the array. */
int getvect(int index, vect *out);

#endif
