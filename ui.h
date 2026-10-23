/***********************************************
 * Filename: ui.h
 * Author: Konrad Swierczek
 * Compile: gcc main.c ui.c vect.c vectstore.c -o minimat
 * Run: ./minimat
***********************************************/

/*
 * ui.h
 * User interface layer: parsing, console input and output.
 */
#ifndef UI_H
#define UI_H

/* Print usage/help text. */
void print_help(void);

/* Run the interactive prompt until quit or end of input. */
void run_ui(void);

#endif
