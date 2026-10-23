/***********************************************
 * Filename: vectstore.h
 * Author: Konrad Swierczek
 * Compile: gcc main.c ui.c vect.c vectstore.c -o minimat
 * Run: ./minimat
***********************************************/

/*
 * vectstore.h
 * Storage layer: holds up to MAX_VECTS named vectors.
 * The array itself is static inside vectstore.c, so it is visible
 * only to these functions (no global variable).
 */
#ifndef VECTSTORE_H
#define VECTSTORE_H

#include "vect.h"

#define MAX_VECTS 10

/* Add v, or replace the stored vector with the same name.
 * Returns 0 on success, -1 if the storage is full. */
int addvect(vect v);

/* Copy the vector called name into *out.
 * Returns 0 if found, -1 if not found. */
int findvect(const char *name, vect *out);

/* Empty the storage array. */
void clearvects(void);

/* Copy the vector in slot index into *out.
 * Returns 0 if the slot is occupied, -1 if empty or out of range.
 * Lets the UI later print the list without touching the array. */
int getvect(int index, vect *out);

#endif
