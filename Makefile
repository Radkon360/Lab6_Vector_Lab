CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

OBJS = vectormain.o ui.o vect.o vectstore.o csvio.o

minimat: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

vectormain.o: vectormain.c ui.h vectstore.h
ui.o: ui.c ui.h vect.h vectstore.h csvio.h
vect.o: vect.c vect.h
vectstore.o: vectstore.c vectstore.h vect.h
csvio.o: csvio.c csvio.h vect.h vectstore.h

clean:
	rm -f minimat $(OBJS)

