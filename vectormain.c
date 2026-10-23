/***********************************************
 * Filename: vectormain.c
 * Author: Konrad Swierczek
 * Compile: gcc main.c ui.c vect.c vectstore.c -o minimat
 * Run: ./minimat
***********************************************/

/*
 * main.c
 * Entry point: handle -h, otherwise hand control to the UI layer.
 */
#include <stdio.h>
#include <string.h>
#include "ui.h"

/* Updated help message to provide clearer instructions to the user. */

int main(int argc, char *argv[])
{
	if (argc > 1) {
		if (strcmp(argv[1], "-h") == 0) {
			print_help();
			return 0;
		}
		fprintf(stderr, "Unknown option '%s'. Try -h for help.\n", argv[1]);
		return 1;
	}

	run_ui();
	return 0;
}
