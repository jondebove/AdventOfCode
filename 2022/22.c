#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define ROW_MAX 200
#define COL_MAX 150

struct vec2 {
	long row;
	long col;
};

enum dir { E, S, W, N };
static struct vec2 const dirs[N + 1] = {
	[E] = {  0,  1 },
	[S] = {  1,  0 },
	[W] = {  0, -1 },
	[N] = { -1,  0 },
};

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	/* read input */
	struct buffer b;
	buffer_create(&b);

	/* read map */
	struct grid g;
	grid_create(&g, ROW_MAX, COL_MAX, ' ');
	long n = 0;
	while (!buffer_getdelim(&b, '\n', stdin) && b.len > 1) {
		assert(n < ROW_MAX);
		assert(b.len - 1 <= COL_MAX);
		memcpy(grid_at(&g, n++, 0), b.str, b.len - 1);
	}
	g.nrow = n;

	/* read path */
	buffer_getdelim(&b, '\n', stdin);
	b.str[--b.len] = '\0';  /* chomp */

	/* move */
	struct vec2 p = { 0, 0 };
	while (p.col < g.ncol && *grid_at(&g, p.row, p.col) != '.') {
		p.col++;
	}
	enum dir d = E;
	for (char const *s = b.str; *s;) {
		if (*s == 'R') {
			d = (d + 1) % (N + 1);
			s++;
		} else if (*s == 'L') {
			d = (d + N) % (N + 1);
			s++;
		} else {
			/* move */
			char *end;
			n = strtol(s, &end, 10);
			assert(s != end);
			s = end;

			struct vec2 pn = p;
			while (n) {
				pn.col += dirs[d].col;
				pn.row += dirs[d].row;
				char const *c = grid_at(&g, pn.row, pn.col);
				if (!c) {
					switch (d) {
						case E: pn.col = 0; break;
						case S: pn.row = 0; break;
						case W: pn.col = g.ncol - 1; break;
						case N: pn.row = g.nrow - 1; break;
						default: assert(0);
					}
					c = grid_at(&g, pn.row, pn.col);
				}

				if (*c == '#') {
					break;
				} else if (*c == '.') {
					p = pn;
					n--;
				}
			}
		}
	}
	ans1 = 1000 * (p.row + 1) + 4 * (p.col + 1) + d;

	grid_destroy(&g);
	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
