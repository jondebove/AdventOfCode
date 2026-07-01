.POSIX:
.SUFFIXES:

CFLAGS = -O2 -Wall -Wextra -pedantic #-g -pg
BINS = 01 02 03 04 05 06 07 08 09 10 11

all: $(BINS)

$(BINS): utils.o utils.h

.PHONY: clean
clean:
	-rm -f $(BINS) *.o

.SUFFIXES: .c .o
.c:
	$(CC) $(CFLAGS) -o $@ $< utils.o -lm

.c.o:
	$(CC) $(CFLAGS) -c $<
