/***********************************************
 * Filename: csvio.h
 * Author: Konrad Swierczek
 * Compile: gcc vectormain.c ui.c vect.c vectstore.c csvio.c -o minimat
 * Run: ./minimat
 ***********************************************/

/*
 * csvio.h
 * File layer: load and save vectors as CSV, one per line: name,x,y,z
 * No console output here; results are reported back to the UI.
 */
#ifndef CSVIO_H
#define CSVIO_H

typedef struct {
    int loaded;                    /* vectors stored from the file */
    int skipped;                   /* mis-formatted lines ignored */
    int first_bad_line;            /* line number of first bad line, 0 if none */
    const char *first_bad_reason;  /* why it was bad, NULL if none */
} csvresult;

/* Merge the vectors in fname into storage (same name = replaced).
 * Bad lines are skipped and counted in *res.
 * Returns 0 on success, -1 if the file could not be opened/read
 * or memory ran out (errno is set). */
int loadcsv(const char *fname, csvresult *res);

/* Write every stored vector to fname, replacing any existing file.
 * *written receives the number of vectors written.
 * Returns 0 on success, -1 on failure (errno is set). */
int savecsv(const char *fname, int *written);

#endif
