#ifndef AOC_UTILS_H
#define AOC_UTILS_H

#include <stdio.h>

/* defs */
#define COUNTOF(a) (sizeof(a) / sizeof((a)[0]))
#define C(c) ((c) != EOF ? (unsigned char)(c) : EOF)
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* buffer */
struct buffer {
	char *str;
	long cap;
	long len;
};

void buffer_create(struct buffer *b);
void buffer_destroy(struct buffer *b);
int buffer_getdelim(struct buffer *b, int delim, FILE *stream);

#endif /* AOC_UTILS_H */
