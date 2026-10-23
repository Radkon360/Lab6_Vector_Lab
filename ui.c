/***********************************************
 * Filename: ui.c
 * Author: Konrad Swierczek
 * Compile: gcc main.c ui.c vect.c vectstore.c -o minimat
 * Run: ./minimat
***********************************************/

/*
 * ui.c
 * User interface layer. All console I/O lives here.
 *
 * Expressions expect spaces around =, +, - and * (allowed by the spec).
 * Commas are treated as spaces, so "a = 1,2,3" and "a = 1 2 3" both work.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"
#include "vect.h"
#include "vectstore.h"

#define LINE_LEN   256
#define MAX_TOKENS 8
#define PROMPT     "minimat>"

/* ---------- helpers --------- */

static int is_op(const char *t)
{
	return strlen(t) == 1 && strchr("+-*", t[0]) != NULL;
}

/* Parse the whole token as a double. Returns 1 on success. */
static int parse_double(const char *s, double *out)
{
	char *end;

	if (*s == '\0' || isalpha((unsigned char)*s)) {
		return 0;           /* blocks "inf", "nan", names */
	}
	*out = strtod(s, &end);
	return *end == '\0';
}

static int is_reserved(const char *s)
{
	return strcmp(s, "quit") == 0 || strcmp(s, "clear") == 0 ||
	       strcmp(s, "list") == 0;
}

static int valid_name(const char *s)
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
	return !is_reserved(s);
}

static void print_vect(const vect *v)
{
	printf("%s = %g %g %g\n", v->name, v->x, v->y, v->z);
}

/* look up an operand as a vector or a number. Returns 0 on success. */
static int resolve(const char *t, vect *v, double *d, int *is_vec)
{
	if (findvect(t, v) == 0) {
		*is_vec = 1;
		return 0;
	}
	if (parse_double(t, d)) {
		*is_vec = 0;
		return 0;
	}
	printf("Error: '%s' is not defined vector or a number.\n", t);
	return -1;
}

/* Evaluate "lhs op rhs". Result name is left empty. Returns 0 on success. */
static int eval_expr(const char *lhs, const char *op, const char *rhs, vect *result)
{
	vect a = {"", 0, 0, 0}, b = {"", 0, 0, 0};
	double da = 0, db = 0;
	int a_vec, b_vec;

	if (resolve(lhs, &a, &da, &a_vec) != 0 || resolve(rhs, &b, &db, &b_vec) != 0) {
		return -1;
	}

	switch (op[0]) {
	case '+':
	case '-':
		if (a_vec && !b_vec) {
			printf("Error: '%s' needs two vectors.\n", op);
			return -1;
		}
		*result = (op[0] == '+') ? add(a,b) : sub(a, b);
		return 0;
	case '*':
		if (a_vec && !b_vec) {
			*result = scale(a, db);
		} else if (!a_vec && b_vec) {
			*result = scale(b, da);
		} else {
			printf("Error: '*' needs one vector and one number.\n");
			return -1;
		}
		return 0;
	}
	return 0;
}

/* ------------- command handlers ------------ */

static void do_list(void)
{
	vect v;
	int count = 0;

	for (int i = 0; i < MAX_VECTS; i++) {
		if (getvect(i, &v) == 0) {
			print_vect(&v);
			count++;
		}
	}
	if (count == 0) {
		printf("No vectors stored.\n");
	}
}

static void do_display(const char *name)
{
	vect v;

	if (findvect(name, &v) == 0) {
		print_vect(&v);
	} else {
		printf("Error: vector '%s' does not exist.\n", name);
	}
}

/* Store result under name, report if full, and display it either way. */
static void store_and_show(const char *name, vect *result)
{
	strncpy(result->name, name, NAME_LEN - 1);
	result->name[NAME_LEN - 1] = '\0';

	if (addvect(*result) != 0) {
		printf("Memory full: cannot store '%s' (max %d vectors)."
		       "Use 'clear' to free space.\n", name, MAX_VECTS);

	}
	print_vect(result);
}

/* tok[0] = name, tok[1] = "=", rest is a value list or an expression. */
static void do_assignment(char *tok[], int n)
{
	vect result = {"", 0, 0, 0};

	if (!valid_name(tok[0])) {
		printf("Error: invalid vector name '%s'.\n", tok[0]);
		return;
	}

	if (n == 5 && is_op(tok[3])) {
		if (eval_expr(tok[2], tok[3], tok[4], &result) != 0) {
			return;
		}
	} else {
		double val[3] = {0.0, 0.0, 0.0};

		if (n != 5) {
			printf("Error: assignment needs 3 values, "
			       "e.g. a = 1 2 3\n");
			return;
		}
		for (int i = 0; i < 3; i++) {
			if (!parse_double(tok[2 + i], &val[i])) {
				printf("Error: '%s' is not a valid number.\n", tok[2 + i]);
				return;
			}
		}
		result.x = val[0];
		result.y = val[1];
		result.z = val[2];
	}
	store_and_show(tok[0], &result);
}

static void process_line(char *line)
{
	char *tok[MAX_TOKENS];
	int n = 0;
	char *t;
	vect result = {"", 0, 0, 0};

	for (char *p = line; *p; p++) {     /* commas act as spaces */
		if (*p == ',') {
			*p = ' ';
		}
	}

	for (t = strtok(line, " \t\r\n"); t != NULL; t = strtok(NULL, " \t\r\n")) {
		if (n == MAX_TOKENS) {
			printf("Error: expression too long.\n");
			return;
		}
		tok[n++] = t;
	}

	if (n == 0) {
		return;
	}

	if (n == 1) {
		if (strcmp(tok[0], "clear") == 0) {
			clearvects();
			printf("All vectors cleared.\n");
		} else if (strcmp(tok[0], "list") == 0) {
			do_list();
		} else {
			do_display(tok[0]);
		}
	} else if (n >= 3 && strcmp(tok[1], "=") == 0) {
		do_assignment(tok, n);
	} else if (n == 3 && is_op(tok[1])) {
		if (eval_expr(tok[0], tok[1], tok[2], &result) == 0) {
			strcpy(result.name, "ans");
			print_vect(&result);
		}
	} else {
		printf("Error: could not parse expression. "
		       "Spaces are required around = + - *\n");
	}
}

/* ------------------ public ---------------- */

void print_help(void)
{
	printf("minimat - 3D vector calculator\n"
	       "\n"
	       "Usage: minimat [-h]\n"
	       "\n"
	       "Commands (space required around = + - *):\n"
	       "  name = x y z       create/replace a vector (commas ok)\n"
	       "  name               display a vector\n"
	       "  a + b              add two vectors\n"
               "  a - b              subtract two vectors\n"
	       "  a * n  or  n * a   scalar multiplication\n"
	       "  c = a + b          operation with assignment (also - and *)\n"
	       "  list               list all stored vectors\n"
	       "  clear              remove all stored vectors\n"
	       "  quit               exit the program\n"
	       "\n"
	       "Up to %d vectors can be stored.\n", MAX_VECTS);
}

void run_ui(void)
{
	char line[LINE_LEN];

	for (;;) {
		printf(PROMPT);
		fflush(stdout);

		if (fgets(line, sizeof(line), stdin) == NULL) {
			printf("\n");
			break;                   /* end of input */
		}
		if (strchr(line, '\n') == NULL && !feof(stdin)) {
			int c;                   /* discard overlong line remainder */
			while ((c = getchar()) != '\n' && c != EOF) {
			}
			printf("Error: line too long.\n");
			continue;
		}


		/* quit handled here so process_line stays free of exit logic */
		char copy[LINE_LEN];
		strcpy(copy, line);
		char *first = strtok(copy, " \t\r\n");
		if (first != NULL && strtok(NULL, " \t\r\n") == NULL && strcmp(first, "quit") == 0) {
			break;
		}

		process_line(line);
	}
}

