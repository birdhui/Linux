# Makefile


CC = gcc
CFLAGS =
OBJS = main.o addnum.o
LIBS = 
all : add.out

add.out : $(OBJS)
	$(CC) $(CFAGS) -o add.out $(OBJS) $(LIBS)

main.o : main.c
	$(CC) $(CFALGS) -c main.c
addnum.o : addnum.c
	$(CC) $(CFAGS) -c addnum.c

clean:
	rm -f $(OBJS) add.out *.o core
