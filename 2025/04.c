#include <stdio.h>

#include "utils.h"

int grid_get(struct grid const *g, long i, long j)
{
	return i >= 0 && i < g->nrow && j >= 0 && j < g->ncol ?
		g->data[j + (g->ncol + 1) * i] : -1;
}

static long replace(struct grid *g, int r)
{
	long ans = 0;
	for (long i = 0; i < g->nrow; i++) {
		for (long j = 0; j < g->ncol; j++) {
			char *c = grid_at(g, i, j);
			if (*c == '@' && 4 >
					(grid_get(g, i-1, j-1) == '@') +
					(grid_get(g, i-1, j+0) == '@') +
					(grid_get(g, i-1, j+1) == '@') +
					(grid_get(g, i+0, j-1) == '@') +
					(grid_get(g, i+0, j+1) == '@') +
					(grid_get(g, i+1, j-1) == '@') +
					(grid_get(g, i+1, j+0) == '@') +
					(grid_get(g, i+1, j+1) == '@')) {
				*c = r;
				ans++;
			}
		}
	}
	return ans;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	/* Read. */
	struct buffer b;
	buffer_create(&b);
	buffer_getdelim(&b, EOF, stdin);

	struct grid g;
	grid_init(&g, b.str, b.len);

	ans1 = replace(&g, '@');

	long n;
	while ((n = replace(&g, 'x'))) ans2 += n;

	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
