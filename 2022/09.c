#include <assert.h>
#include <search.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

struct vec2 {
	int x;
	int y;
	unsigned int flags;
};

static int vec2_cmp(void const *a, void const *b)
{
	struct vec2 const *va = a;
	struct vec2 const *vb = b;
	int cmp = va->x - vb->x;
	if (cmp == 0) cmp = va->y - vb->y;
	return cmp;
}

static void move(struct vec2 *pos, struct vec2 dir)
{
	pos->x += dir.x;
	pos->y += dir.y;
}

static int search(tnode *set, struct vec2 pos)
{
	struct vec2 *v = xrealloc(NULL, sizeof(*v));
	v->x = pos.x;
	v->y = pos.y;
	v->flags = 0;
	struct vec2 **vp = tsearch(v, set, vec2_cmp);
	if ((*vp)->flags) { /* found */
		free(v);
		v = *vp;
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

#define NKNOTS 10
	struct vec2 knots[NKNOTS] = { 0 };
	for (int i = 0; i < NKNOTS; i++) {
		knots[i].flags = (1U << i);
	}

	tnode *set = NULL;
	ans1 += search(&set, knots[1]);
	ans2 += search(&set, knots[9]);

	char s[8];
	int n;
	while (scanf("%7s %d", s, &n) == 2) {
		struct vec2 dir = { 0 };
		switch (s[0]) {
			case 'L': dir.x = -1; break;
			case 'R': dir.x = +1; break;
			case 'D': dir.y = -1; break;
			case 'U': dir.y = +1; break;
			default: assert(0);
		}
		assert(n >= 0);

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
					ans1 += search(&set, knots[i]);
				}
				if (i == 9) {
					ans2 += search(&set, knots[i]);
				}
			}
		}
	}

	tdestroy(&set, free);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
