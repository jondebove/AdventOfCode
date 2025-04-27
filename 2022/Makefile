.POSIX:
.SUFFIXES:

CFLAGS = -Wall -Wextra -Wpedantic
BINS = 01 02 03 04 05 06 07 08

all: $(BINS)

$(BINS): utils.o utils.h

07: hashtable.h fnv1a.h

.PHONY: clean
clean:
	-rm -f $(BINS) *.o

.SUFFIXES: .c .o
.c:
	$(CC) $(CFLAGS) -o $@ $< utils.o -lm

.c.o:
	$(CC) $(CFLAGS) -c $<
