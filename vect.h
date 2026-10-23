/***********************************************
 * Filename: vect.h
 * Author: Konrad Swierczek
 * Compile: gcc main.c ui.c vect.c vectstore.c -o minimat
 * Run: ./minimat
***********************************************/

/*
 * vect.h
 * Operations layer: the vector struct and the math performed on it.
 */
#ifndef VECT_H
#define VECT_H

#define NAME_LEN 32

typedef struct {
	char name[NAME_LEN];
	double x;
	double y;
	double z;
} vect;

/* Element-by-element sum a + b */
vect add(vect a, vect b);

/* Element-by-element difference a - b. */
vect sub(vect a, vect b);

/* Each component of a multiplied by scalar s. */
vect scale(vect a, double s);

#endif
