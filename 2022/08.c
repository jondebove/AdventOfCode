#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

	buffer_getdelim(&b, EOF, stdin);

	/* init grid */
	struct grid g;
	grid_init(&g, b.str, b.len);

	/* part 1 */
	bool *visibility = xrealloc(NULL, b.len * sizeof(*visibility));
	memset(visibility, 0, b.len * sizeof(*visibility));

	for (long i = 0; i < g.nrow; i++) {
		/* L -> R */
		int height = '0' - 1;
		for (long j = 0; j < g.ncol; j++) {
			char *c = grid_at(&g, i, j);
			if (*c > height) {
				visibility[c - g.data] = true;
				height = *c;
			}
		}
		/* R -> L */
		height = '0' - 1;
		for (long j = g.ncol - 1; j >= 0; j--) {
			char *c = grid_at(&g, i, j);
			if (*c > height) {
				visibility[c - g.data] = true;
				height = *c;
			}
		}
	}

	for (long j = 0; j < g.ncol; j++) {
		/* U -> D */
		int height = '0' - 1;
		for (long i = 0; i < g.nrow; i++) {
			char *c = grid_at(&g, i, j);
			if (*c > height) {
				visibility[c - g.data] = true;
				height = *c;
			}
		}
		/* D -> U */
		height = '0' - 1;
		for (long i = g.nrow - 1; i >= 0; i--) {
			char *c = grid_at(&g, i, j);
			if (*c > height) {
				visibility[c - g.data] = true;
				height = *c;
			}
		}
	}

	for (long i = 0; i < b.len; i++) {
		ans1 += visibility[i];
	}

	free(visibility);

	/* part 2 */
	for (long i = 1; i < g.nrow - 1; i++) {
		for (long j = 1; j < g.ncol - 1; j++) {
			char height = *grid_at(&g, i, j);
			long score = 1;
			int k;
			/* R */
			for (k = j + 1; k < g.ncol - 1; k++) {
				if (*grid_at(&g, i, k) >= height) {
					break;
				}
			}
			score *= k - j;
			/* L */
			for (k = j - 1; k > 0; k--) {
				if (*grid_at(&g, i, k) >= height) {
					break;
				}
			}
			score *= j - k;
			/* D */
			for (k = i + 1; k < g.nrow - 1; k++) {
				if (*grid_at(&g, k, j) >= height) {
					break;
				}
			}
			score *= k - i;
			/* U */
			for (k = i - 1; k > 0; k--) {
				if (*grid_at(&g, k, j) >= height) {
					break;
				}
			}
			score *= i - k;
			ans2 = MAX(ans2, score);
		}
	}

	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
