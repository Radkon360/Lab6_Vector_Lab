/***********************************************
 * Filename: csvio.c
 * Author: Konrad Swierczek
 * Compile: gcc vectormain.c ui.c vect.c vectstore.c csvio.c -o minimat
 * Run: ./minimat
 ***********************************************/

/*
 * csvio.c
 * CSV load/save. Accepts "\n" or "\r\n" line endings, blank lines,
 * spaces around fields, and the UTF-8 marker Excel may put at the top.
 */
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csvio.h"
#include "vect.h"
#include "vectstore.h"

#define LINE_LEN 256

/* Remove leading/trailing whitespace; returns pointer into s. */
static char *trim(char *s)
{
    char *end;

    while (isspace((unsigned char)*s)) {
        s++;
    }
    end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) {
        *--end = '\0';
    }
    return s;
}

/* Same naming rules as the UI: letter or _ first, then letters/digits/_ */
static int name_ok(const char *s)
{
    if (strlen(s) == 0 || strlen(s) >= NAME_LEN) {
        return 0;
    }
    if (!isalpha((unsigned char)s[0]) && s[0] != '_') {
        return 0;
    }
    for (const char *p = s; *p; p++) {
        if (!isalnum((unsigned char)*p) && *p != '_') {
            return 0;
        }
    }
    return 1;
}

/* Whole string must be a number ("inf", "nan" and empty are rejected). */
static int parse_value(const char *s, double *out)
{
    char *end;

    if (*s == '\0' || isalpha((unsigned char)*s)) {
        return 0;
    }
    errno = 0;
    *out = strtod(s, &end);
    return *end == '\0' && errno != ERANGE;
}

/* Returns 0 = vector parsed, 1 = blank line, -1 = bad line (*why set). */
static int parse_line(char *line, vect *out, const char **why)
{
    char *field[4];
    char *p;
    int n = 1;
    double val[3];

    line = trim(line);
    if (*line == '\0') {
        return 1;
    }

    field[0] = line;
    p = line;
    while ((p = strchr(p, ',')) != NULL) {
        *p++ = '\0';
        if (n == 4) {
            *why = "too many fields (expected name,x,y,z)";
            return -1;
        }
        field[n++] = p;
    }
    if (n != 4) {
        *why = "too few fields (expected name,x,y,z)";
        return -1;
    }

    for (int i = 0; i < 4; i++) {
        field[i] = trim(field[i]);
    }
    if (!name_ok(field[0])) {
        *why = "invalid vector name";
        return -1;
    }
    for (int i = 0; i < 3; i++) {
        if (!parse_value(field[i + 1], &val[i])) {
            *why = "value is not a valid number";
            return -1;
        }
    }

    memset(out, 0, sizeof(*out));
    strcpy(out->name, field[0]);
    out->x = val[0];
    out->y = val[1];
    out->z = val[2];
    return 0;
}

int loadcsv(const char *fname, csvresult *res)
{
    FILE *fp;
    char line[LINE_LEN];
    int lineno = 0;
    int saved_errno;

    res->loaded = 0;
    res->skipped = 0;
    res->first_bad_line = 0;
    res->first_bad_reason = NULL;

    fp = fopen(fname, "r");
    if (fp == NULL) {
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        char *text = line;
        const char *why = "";
        vect v;
        int rc;

        lineno++;

        if (strchr(line, '\n') == NULL && !feof(fp)) {
            int c; /* overlong line: discard the rest of it */
            while ((c = fgetc(fp)) != '\n' && c != EOF) {
            }
            rc = -1;
            why = "line too long";
        } else {
            if (lineno == 1 && (unsigned char)text[0] == 0xEF &&
                (unsigned char)text[1] == 0xBB &&
                (unsigned char)text[2] == 0xBF) {
                text += 3; /* skip UTF-8 BOM */
            }
            rc = parse_line(text, &v, &why);
        }

        if (rc == 1) {
            continue; /* blank line */
        }
        if (rc < 0) {
            if (res->skipped == 0) {
                res->first_bad_line = lineno;
                res->first_bad_reason = why;
            }
            res->skipped++;
            continue;
        }
        if (addvect(v) != 0) {
            fclose(fp);
            errno = ENOMEM;
            return -1;
        }
        res->loaded++;
    }

    saved_errno = ferror(fp) ? errno : 0;
    fclose(fp);
    if (saved_errno != 0) {
        errno = saved_errno;
        return -1;
    }
    return 0;
}

int savecsv(const char *fname, int *written)
{
    FILE *fp;
    vect v;

    *written = 0;
    fp = fopen(fname, "w");
    if (fp == NULL) {
        return -1;
    }

    for (int i = 0; i < vectcount(); i++) {
        if (getvect(i, &v) != 0) {
            continue;
        }
        if (fprintf(fp, "%s,%.15g,%.15g,%.15g\n",
                    v.name, v.x, v.y, v.z) < 0) {
            int e = errno;
            fclose(fp);
            errno = e;
            return -1;
        }
        (*written)++;
    }

    if (fclose(fp) != 0) {
        return -1;
    }
    return 0;
}
