#ifndef AOC_UTILS_H
#define AOC_UTILS_H

#include <stddef.h>
#include <stdio.h>

/* defs */
#define COUNTOF(a) ((ptrdiff_t)(sizeof(a) / sizeof((a)[0])))
#define C(c) ((c) != EOF ? (unsigned char)(c) : EOF)
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define SAMESIGN(x, y) (((x) < 0) == ((y) < 0))

/* xalloc */
extern size_t nalloc;
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

/* slice */
struct slice {
	char *str;
	long len;
};

/* grid */
struct grid {
	char *data;
	long nrow;
	long ncol;
};
void grid_create(struct grid *g, long nrow, long ncol, int fill);
void grid_destroy(struct grid *g);
void grid_init(struct grid *g, char *data, long len);
char *grid_at(struct grid const *g, long i, long j);

/* arith */
long gcd(long x, long y);
long vgcd(long x, ...);
#define GCD(...) vgcd(__VA_ARGS__, 0L)

long lcm(long x, long y);
long vlcm(long x, ...);
#define LCM(...) vlcm(__VA_ARGS__, 0L)

long bisectl(long (*f)(long x, void *ctx), long lo, long hi, void *ctx);

#endif /* AOC_UTILS_H */
