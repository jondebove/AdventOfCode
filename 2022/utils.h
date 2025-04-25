#ifndef AOC_UTILS_H
#define AOC_UTILS_H

#include <stdio.h>

/* defs */
#define COUNTOF(a) (sizeof(a) / sizeof((a)[0]))
#define C(c) ((c) != EOF ? (unsigned char)(c) : EOF)
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* xalloc */
void *xrealloc(void *ptr, size_t size);

/* buffer */
struct buffer {
	char *str;
	long len;
	long cap;
};

void buffer_create(struct buffer *b);
void buffer_destroy(struct buffer *b);
int buffer_getdelim(struct buffer *b, int delim, FILE *stream);

struct slice {
	char *str;
	long len;
};

#endif /* AOC_UTILS_H */
