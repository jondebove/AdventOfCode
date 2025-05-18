#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define NELVES_MAX 3000
#define MAP_PAD 50
#define MAP_SIZ 200

enum dir {
	N, S, W, E,
	NW, NE, SW, SE,
};

struct pos {
	int row;
	int col;
};

static struct pos const dirs[8] = {
	[N]  = { -1,  0 },
	[S]  = {  1,  0 },
	[W]  = {  0, -1 },
	[E]  = {  0,  1 },
	[NW] = { -1, -1 },
	[NE] = { -1,  1 },
	[SW] = {  1, -1 },
	[SE] = {  1,  1 },
};

static unsigned int const mask[4] = {
	[N] = 1U << N | 1U << NW | 1U << NE,
	[S] = 1U << S | 1U << SW | 1U << SE,
	[W] = 1U << W | 1U << NW | 1U << SW,
	[E] = 1U << E | 1U << NE | 1U << SE,
};

struct elf {
	struct pos pos;
	unsigned int neighbors;
};

static int next(struct elf *elves, long nelves, struct elf *map[MAP_SIZ][MAP_SIZ])
{
	static int turn = 0;
	int n = 0;

	/* Scan the neighborhood of each elf before trying to move them */
	for (long i = nelves - 1; i >= 0; i--) {
		struct elf *e = &elves[i];
		e->neighbors = 0;
		for (int d = 0; d < 8; d++) {
			if (map[e->pos.row + dirs[d].row][e->pos.col + dirs[d].col]) {
				e->neighbors |= 1U << d;
			}
		}
	}

	/*
	 * If the tile is free, we move him and handle the collision later.
	 * We may do this because we know the tile was free before starting
	 * to move the elves AND the elf occupying the tile had to come from
	 * the opposite direction.
	 */
	for (long i = nelves - 1; i >= 0; i--) {
		struct elf *e = &elves[i];
		if (e->neighbors == 0) {
			continue;
		}
		for (int j = 0; j < 4; j++) {
			int d = (turn + j) % 4;
			if ((e->neighbors & mask[d]) == 0) {
				struct elf **ep = &map[e->pos.row + dirs[d].row][e->pos.col + dirs[d].col];
				if (!*ep) {
					/* greedy move */
					map[e->pos.row][e->pos.col] = NULL;
					e->pos.row += dirs[d].row;
					e->pos.col += dirs[d].col;
					*ep = e;
					n++;
				} else {
					/* backtrack */
					(*ep)->pos.row += dirs[d].row;
					(*ep)->pos.col += dirs[d].col;
					map[(*ep)->pos.row][(*ep)->pos.col] = *ep;
					*ep = NULL;
					n--;
				}
				break;
			}
		}
		assert(e->pos.row > 0 && e->pos.row < MAP_SIZ - 1);
		assert(e->pos.col > 0 && e->pos.col < MAP_SIZ - 1);
	}
	turn++;
	return n;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct elf elves[NELVES_MAX];
	long nelves = 0;
	struct elf *map[MAP_SIZ][MAP_SIZ] = { 0 };

	/* parse */
	{
		struct buffer b;
		buffer_create(&b);

		int r = MAP_PAD;
		while (!buffer_getdelim(&b, '\n', stdin)) {
			for (char const *s = b.str; (s = strchr(s, '#')); s++) {
				assert(nelves < NELVES_MAX);
				struct elf *e = &elves[nelves++];
				e->pos.row = r;
				e->pos.col = s - b.str + MAP_PAD;

				assert(e->pos.row < MAP_SIZ && e->pos.col < MAP_SIZ);
				map[e->pos.row][e->pos.col] = e;
			}
			r++;
		}

		buffer_destroy(&b);
	}

	/* part 1 */
	for (ans2 = 0; ans2 < 10; ans2++) {
		next(elves, nelves, map);
	}
	{
		int rmin = MAP_SIZ - 1;
		int rmax = 0;
		int cmin = MAP_SIZ - 1;
		int cmax = 0;
		for (long i = nelves - 1; i >= 0; i--) {
			struct elf *e = &elves[i];
			rmin = MIN(rmin, e->pos.row);
			rmax = MAX(rmax, e->pos.row);
			cmin = MIN(cmin, e->pos.col);
			cmax = MAX(cmax, e->pos.col);
		}
		ans1 = (rmax - rmin + 1) * (cmax - cmin + 1) - nelves;
	}

	/* part 2 */
	for (ans2++; next(elves, nelves, map); ans2++);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
