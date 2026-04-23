.POSIX:
.SUFFIXES:

CFLAGS = -Wall -Wextra -Wpedantic #-g -pg
BINS = 01 02 03 04 05 06 07

all: $(BINS)

04: 04.c
06: 06.c
04 06: utils.o
	$(CC) $(CFLAGS) -o $@ $@.c utils.o -lm

.PHONY: clean
clean:
	-rm -f $(BINS) *.o

.SUFFIXES: .c .o
.c:
	$(CC) $(CFLAGS) -o $@ $< -lm

.c.o:
	$(CC) $(CFLAGS) -c $<
