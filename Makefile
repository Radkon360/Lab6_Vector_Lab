CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

OBJS = vectormain.o ui.o vect.o vectstore.o

minimat: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

main.o:      vectormain.c ui.h
ui.o:        ui.c ui.h vect.h vectstore.h
vect.o:      vect.c vect.h
vectstore.o: vectstore.c vectstore.h vect.h

clean:
	rm -f minimat $(OBJS)

