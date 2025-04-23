.POSIX:
.SUFFIXES:

CFLAGS = -Wall -Wextra -Wpedantic
BINS = 01

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
