#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static struct shape {
	struct {
		long x;
		long y;
	} const data[5];
	long const length;
	long const height;
} const shapes[5] = {
	{
		/* - */
		{
			{ 0, 0 },
			{ 1, 0 },
			{ 2, 0 },
			{ 3, 0 },
		}, 4, 1,
	}, {
		/* + */
		{
			{ 1, 0 },
			{ 0, 1 },
			{ 1, 1 },
			{ 2, 1 },
			{ 1, 2 },
		}, 5, 3,
	}, {
		/* J */
		{
			{ 0, 0 },
			{ 1, 0 },
			{ 2, 0 },
			{ 2, 1 },
			{ 2, 2 },
		}, 5, 3,
	}, {
		/* I */
		{
			{ 0, 0 },
			{ 0, 1 },
			{ 0, 2 },
			{ 0, 3 },
		}, 4, 4,
	}, {
		/* O */
		{
			{ 0, 0 },
			{ 1, 0 },
			{ 0, 1 },
			{ 1, 1 },
		}, 4, 2,
	},
};

struct jet {
	char const *const data;
	long offset;
};

struct rock {
	struct shape const *shape;
	long x;
	long y;
};

static bool rock_move1(struct rock *r, struct grid const *g, long dx, long dy)
{
	r->x += dx;
	r->y += dy;
	struct shape const *s = r->shape;
	for (long i = 0; i < s->length; i++) {
		char *c = grid_at(g, r->y + s->data[i].y, r->x + s->data[i].x);
		if (!c || *c == '#') {
			r->x -= dx;
			r->y -= dy;
			return false;
		}
	}
	return true;
}

static void rock_move(struct grid *g, struct jet *j, long *ymax)
{
	static int idx_shape = 0;

	struct rock r;
	r.shape = &shapes[(idx_shape++) % COUNTOF(shapes)];
	r.x = 2;
	r.y = *ymax + 3;

	for (;;) {
		char const *c = j->data + j->offset;
		j->offset = *(c + 1) ? j->offset + 1 : 0;
		switch (*c) {
			case '<': rock_move1(&r, g, -1, 0); break;
			case '>': rock_move1(&r, g, +1, 0); break;
			default: assert(0);
		}

		if (!rock_move1(&r, g, 0, -1)) {
			struct shape const *s = r.shape;
			for (long i = 0; i < s->length; i++) {
				*grid_at(g, r.y + s->data[i].y, r.x + s->data[i].x) = '#';
			}
			*ymax = MAX(*ymax, r.y + r.shape->height);
			assert(*ymax + 7 < g->nrow);
			return;
		}
	}
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	/* read input */
	struct buffer b;
	buffer_create(&b);

	buffer_getdelim(&b, EOF, stdin);
	b.str[--b.len] = '\0'; // chomp
	struct jet j = { b.str, 0 };

	struct grid g;
	grid_create(&g, 1L << 16, 7);

	/* part 1 */
	long ymax = 0;
	long nr;
	for (nr = 0; nr < 2022; nr++) {
		rock_move(&g, &j, &ymax);
	}
	ans1 = ymax;

	/* part 2: find cycle */
	long const nrow = 15;
	long const size = (g.ncol + 1) * nrow;
	char *ref = xrealloc(NULL, size);
	memcpy(ref, grid_at(&g, ymax - nrow, 0), size);
	do {
		rock_move(&g, &j, &ymax);
		nr++;
	} while (memcmp(ref, grid_at(&g, ymax - nrow, 0), size));
	long cycle_row = ymax - ans1;
	long cycle_rock = nr - 2022;

	/* part 2: compute remainder */
	ldiv_t res = ldiv(1000000000000 - nr, cycle_rock);
	while (res.rem--) {
		rock_move(&g, &j, &ymax);
	}
	ans2 = ymax + res.quot * cycle_row;

	free(ref);
	grid_destroy(&g);
	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
