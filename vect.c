/***********************************************
 * Filename: vect.c
 * Author: Konrad Swierczek
 * Compile: gcc vectormain.c ui.c vect.c vectstore.c csvio.c -o minimat
 * Run: ./minimat
 ***********************************************/

/*
 * vect.c
 * Operations layer: pure math, no I/O and no storage access.
 * Results are returned with an empty name; the caller names them.
 */
#include "vect.h"

vect add(vect a, vect b)
{
    vect r = {"", 0.0, 0.0, 0.0};
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

vect sub(vect a, vect b)
{
    vect r = {"", 0.0, 0.0, 0.0};
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

vect scale(vect a, double s)
{
    vect r = {"", 0.0, 0.0, 0.0};
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
    return r;
}

