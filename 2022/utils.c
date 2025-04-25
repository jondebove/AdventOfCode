#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void *xrealloc(void *ptr, size_t size)
{
	void *x = realloc(ptr, size);
	if (!x) {
		fprintf(stderr, "%s\n", strerror(ENOMEM));
		exit(1);
	}
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
