PROGRAM = ice-tower
CC      = gcc
CFLAGS  = -g -ansi -Wall -I/usr/X11R6/include -I/usr/pkg/include -std=c99
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): main.o objekti.o image.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(PROGRAM)  main.o objekti.o image.o $(LDLIBS)

image.o: image.c image.h
	$(CC) $(LDFLAGS) $(CFLAGS) -c image.c -o image.o

objekti.o: objekti.c objekti.h
	$(CC) $(LDFLAGS) $(CFLAGS) -c objekti.c -o objekti.o

main.o: main.c objekti.h image.h
	$(CC) $(LDFLAGS) $(CFLAGS) -c main.c -o main.o

.PHONY: clean

clean:
	-rm *.o $(PROGRAM)
