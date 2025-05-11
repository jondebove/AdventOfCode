.POSIX:
.SUFFIXES:

CFLAGS = -Wall -Wextra -Wpedantic
BINS = 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17

all: $(BINS)

$(BINS): utils.o utils.h

07 09: hashtable.h

.PHONY: clean
clean:
	-rm -f $(BINS) *.o

.SUFFIXES: .c .o
.c:
	$(CC) $(CFLAGS) -o $@ $< utils.o -lm

.c.o:
	$(CC) $(CFLAGS) -c $<
