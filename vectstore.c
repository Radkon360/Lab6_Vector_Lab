/***********************************************
 * Filename: vectstore.c
 * Author: Konrad Swierczek
 * Compile: gcc vectormain.c ui.c vect.c vectstore.c csvio.c -o minimat
 * Run: ./minimat
 ***********************************************/

/*
 * vectstore.c
 * Storage layer implementation: dynamic array that starts small and
 * doubles with realloc() whenever it fills.
 */
#include <stdlib.h>
#include <string.h>
#include "vectstore.h"

#define INITIAL_CAP 4

static vect *store = NULL;
static int count = 0;
static int capacity = 0;

int addvect(vect v)
{
    for (int i = 0; i < count; i++) {
        if (strcmp(store[i].name, v.name) == 0) {
            store[i] = v; /* same name: replace in place */
            return 0;
        }
    }

    if (count == capacity) { /* full: grow the block */
        int newcap = (capacity == 0) ? INITIAL_CAP : capacity * 2;
        vect *tmp = realloc(store, (size_t)newcap * sizeof(vect));

        if (tmp == NULL) {
            return -1; /* old block is still valid */
        }
        store = tmp;
        capacity = newcap;
    }
    store[count++] = v;
    return 0;
}

int findvect(const char *name, vect *out)
{
    for (int i = 0; i < count; i++) {
        if (strcmp(store[i].name, name) == 0) {
            *out = store[i];
            return 0;
        }
    }
    return -1;
}

void clearvects(void)
{
    free(store);
    store = NULL;
    count = 0;
    capacity = 0;
}

int vectcount(void)
{
    return count;
}

int getvect(int index, vect *out)
{
    if (index < 0 || index >= count) {
        return -1;
    }
    *out = store[index];
    return 0;
}

