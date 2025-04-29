#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

size_t nalloc = 0;
void *xrealloc(void *ptr, size_t size)
{
	void *x = realloc(ptr, size);
	if (!x) {
		fprintf(stderr, "%s\n", strerror(ENOMEM));
		exit(1);
	}
	nalloc++;
	return x;
}

void buffer_create(struct buffer *b)
{
	b->str = NULL;
	b->cap = 0;
	b->len = 0;
}

void buffer_destroy(struct buffer *b)
{
	free(b->str);
}

int buffer_getdelim(struct buffer *b, int delim, FILE *stream)
{
	assert((unsigned long)b->cap <= SIZE_MAX);

	size_t cap = b->cap;
	ssize_t len = getdelim(&b->str, &cap, delim, stream);
	if (len == -1) {
		return -1;
	}

	assert(cap <= LONG_MAX);
	assert(len <= LONG_MAX);
	b->cap = cap;
	b->len = len;
	return 0;
}

void grid_init(struct grid *g, char *data, long len)
{
	g->data = data;
	g->ncol = strchr(data, '\n') - data;
	g->nrow = len / (g->ncol + 1);
}

char *grid_at(struct grid *g, long i, long j)
{
	return i >= 0 && i < g->nrow && j >= 0 && j < g->ncol ?
		&g->data[j + (g->ncol + 1) * i] : NULL;
}

/* arith */
long gcd(long x, long y)
{
	while (y != 0) {
		long z = x % y;
		x = y;
		y = z;
	}
	return labs(x);
}

long lcm(long x, long y)
{
	return (x / gcd(x, y)) * y;
}

#include <stdarg.h>

long vgcd(long x, ...)
{
	va_list ap;
	va_start(ap, x);

	long ans = x;
	while ((x = va_arg(ap, long))) {
		ans = gcd(ans, x);
	}
	va_end(ap);
	return ans;
}

long vlcm(long x, ...)
{
	va_list ap;
	va_start(ap, x);

	long ans = x;
	while ((x = va_arg(ap, long))) {
		ans = (ans / gcd(ans, x)) * x;
	}
	va_end(ap);
	return ans;
}
