#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static long vatol(struct grid const *g, long j)
{
	int c = *grid_at(g, 0, j);
	long ans = c == ' ' ? 0 : c - '0';

	long i;
	for (i = 1; i < g->nrow - 1; i++) {
		c = *grid_at(g, i, j);
		if (c != ' ') ans = ans * 10 + c - '0';
	}
	return ans;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);
	buffer_getdelim(&b, EOF, stdin);

	struct grid g;
	grid_init(&g, b.str, b.len);

	char *ops = grid_at(&g, g.nrow - 1, 0);

	/* Part 1. */
	char *o;
	for (o = ops; o; o = strpbrk(o + 1, "+*")) {
		long res = atol(grid_at(&g, 0, o - ops));
		long i;
		for (i = 1; i < g.nrow - 1; i++) {
			long v = atol(grid_at(&g, i, o - ops));
			if (*o == '+') {
				res += v;
			} else {
				res *= v;
			}
		}
		ans1 += res;
	}

	/* Part 2. */
	for (o = ops; o; o = strpbrk(o + 1, "+*")) {
		long res = vatol(&g, o - ops);
		long j;
		for (j = (o - ops) + 1; j < g.ncol; j++) {
			long v = vatol(&g, j);
			if (v == 0) break;
			if (*o == '+') {
				res += v;
			} else {
				res *= v;
			}
		}
		ans2 += res;
	}

	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
