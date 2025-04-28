#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"
#include "utils.h"

struct vec2 {
	int x;
	int y;
	unsigned int flags;
	SHASH_ENTRY(vec2) vs;
};

void move(struct vec2 *pos, struct vec2 dir)
{
	pos->x += dir.x;
	pos->y += dir.y;
}

SHASH_TABLE(vec2_set, vec2);

int enter(struct vec2_set *set, struct vec2 pos)
{
	unsigned int h = pos.x + pos.y * 31;
	struct vec2 *v;
	SHASH_SEARCH_FOREACH(v, h, set, vs) {
		if (v->x == pos.x && v->y == pos.y) {
			break;
		}
	}
	if (!v) {
		v = xrealloc(NULL, sizeof(*v));
		*v = pos;
		SHASH_INSERT(set, v, h, vs);
		return 1;
	}
	if (v->flags & pos.flags) {
		return 0;
	}
	v->flags |= pos.flags;
	return 1;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

#define NKNOTS 10
	struct vec2 knots[NKNOTS] = { 0 };
	for (int i = 0; i < NKNOTS; i++) {
		knots[i].flags = (1U << i);
	}

	struct vec2_set *set = xrealloc(NULL, SHASH_TABLE_SIZE(vec2_set, 12));
	SHASH_INIT(set, 12);
	ans1 += enter(set, knots[1]);
	ans2 += enter(set, knots[9]);

	while (!buffer_getdelim(&b, '\n', stdin)) {
		char s[8];
		int n;
		sscanf(b.str, "%7s %d", s, &n);
		assert(n >= 0);

		struct vec2 dir = { 0 };
		switch (s[0]) {
			case 'L': dir.x = -1; break;
			case 'R': dir.x = +1; break;
			case 'D': dir.y = -1; break;
			case 'U': dir.y = +1; break;
			default: assert(0);
		}

		while (n--) {
			move(&knots[0], dir);

			for (int i = 1; i < NKNOTS; i++) {
				if (abs(knots[i-1].x - knots[i].x) <= 1 &&
				    abs(knots[i-1].y - knots[i].y) <= 1) {
					break;
				}
				struct vec2 d = {
					.x = (knots[i-1].x > knots[i].x) -
						(knots[i-1].x < knots[i].x),
					.y = (knots[i-1].y > knots[i].y) -
						(knots[i-1].y < knots[i].y),
				};
				move(&knots[i], d);
				if (i == 1) {
					ans1 += enter(set, knots[i]);
				}
				if (i == 9) {
					ans2 += enter(set, knots[i]);
				}
			}
		}
	}

	struct vec2 *v, *vn;
	SHASH_FOREACH_SAFE(v, set, vs, vn) {
		free(v);
	}
	free(set);
	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
