/***********************************************
 * Filename: vectstore.c
 * Author: Konrad Swierczek
 * Compile: gcc main.c ui.c vect.c vectstore.c -o minimat
 * Run: ./minimat
***********************************************/

/*
 * vectstore.c
 * Storage layer implementation.
 */
#include <string.h>
#include "vectstore.h"

static vect store[MAX_VECTS];
static int used[MAX_VECTS];

int addvect(vect v)
{
	int empty = -1;

	for (int i = 0; i < MAX_VECTS; i++) {
		if (used[i]) {
			if (strcmp(store[i].name, v.name) == 0) {
				store[i] = v;      /* same name: replace in place */
				return 0;
			}
		} else if (empty < 0) {
			empty = i;                 /* remember first free slot */
		}
	}

	if (empty < 0) {
		return -1;                         /* full */
	}
	store[empty] = v;
	used[empty] = 1;
	return 0;
}

int findvect(const char *name, vect *out)
{
	for (int i = 0; i < MAX_VECTS; i++) {
		if (used[i] && strcmp(store[i].name, name) == 0) {
			*out = store[i];
			return 0;
		}
	}
	return -1;
}

void clearvects(void)
{
	memset(store, 0, sizeof(store));
	memset(used, 0, sizeof(used));
}

int getvect(int index, vect *out)
{
	if (index < 0 || index >= MAX_VECTS || !used[index]) {
		return -1;
	}
	*out = store[index];
	return 0;
}
