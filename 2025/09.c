#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct point {
	int x;
	int y;
};

bool point_inside(struct point const p[static 1], size_t np, struct point const ps[static np])
{
	bool inside = false;
	struct point const *pp;
	for (pp = &ps[np - 1]; np--; pp = ps++) {
		if ((pp->y >= p->y) == (ps->y >= p->y)) continue;

		int xf = (pp->x >= p->x);
		if (xf == (ps->x >= p->x)) {
			inside = xf ? !inside : inside;
		} else if ((ps->y - p->y) * (pp->x - ps->x) >= (ps->x - p->x) * (pp->y - ps->y)) {
			inside = !inside;
		}
	}
	return inside;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	/* Read. */
	struct point *ps = calloc(1024, sizeof(*ps));
	int np = 0;
	while (scanf(" %d,%d ", &ps[np].x, &ps[np].y) == 2) {
		np++;
		assert(np < 1024);
	}

	int i, j;
	for (i = 0; i < np; i++) {
		for (j = i + 1; j < np; j++) {
			long area = (labs(ps[i].x - ps[j].x) + 1L) *
				(labs(ps[i].y - ps[j].y) + 1L);
			ans1 = area > ans1 ? area : ans1;

			// TODO part 2
		}
	}

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
