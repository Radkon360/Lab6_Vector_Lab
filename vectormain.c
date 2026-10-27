/***********************************************
 * Filename: vectormain.c
 * Author: Konrad Swierczek
 * Compile: gcc vectormain.c ui.c vect.c vectstore.c csvio.c -o minimat
 * Run: ./minimat
 ***********************************************/

/*
 * main.c
 * Entry point: handle -h, otherwise hand control to the UI layer.
 * Also makes sure all vector memory is released on exit: clearvects()
 * is registered with atexit(), and Ctrl-C (SIGINT) is trapped so the
 * program leaves through the normal exit path.
 */
#define _POSIX_C_SOURCE 200809L /* for sigaction() under -std=c99 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "vectstore.h"

static void on_sigint(int sig)
{
    (void)sig;
    ui_request_stop(); /* only sets a flag; run_ui() leaves its loop */
}

int main(int argc, char *argv[])
{
    struct sigaction sa;

    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) {
            print_help();
            return 0;
        }
        fprintf(stderr, "Unknown option '%s'. Try -h for help.\n", argv[1]);
        return 1;
    }

    atexit(clearvects);

    /* no SA_RESTART, so a read blocked at the prompt returns on Ctrl-C */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    run_ui();
    return 0;
}

