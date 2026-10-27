/***********************************************
 * Filename: ui.h
 * Author: Konrad Swierczek
 * Compile: gcc vectormain.c ui.c vect.c vectstore.c csvio.c -o minimat
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

/* Run the interactive prompt until quit, end of input or Ctrl-C. */
void run_ui(void);

/* Ask run_ui() to stop. Only sets a flag, so it is safe to call
 * from a signal handler. */
void ui_request_stop(void);

#endif
