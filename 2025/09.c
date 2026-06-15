#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct point {
	int x;
	int y;
};

static bool intersect(struct point const *p0, struct point const *q0,
		struct point const *p1, struct point const *q1)
{
	if (p0->x == q0->x) {
		/* [p0,q0] vertical. */
		if (p1->x == q1->x) return false;
		/* [p1,q1] horizontal. */
		if ((p0->x <  p1->x) == (p0->x <  q1->x)) return false;
		if ((p1->y <= p0->y) == (p1->y <= q0->y)) return false;
	} else {
		/* [p0,q0] horizontal. */
		if (p1->y == q1->y) return false;
		/* [p1,q1] vertical. */
		if ((p0->y <  p1->y) == (p0->y <  q1->y)) return false;
		if ((p1->x <= p0->x) == (p1->x <= q0->x)) return false;
	}
	return true;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	/* Read. */
	struct point *ps = calloc(1024, sizeof(*ps));
	assert(ps);
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

			int k, l;
			for (k = np - 1, l = 0; l < np; k = l++) {
				/* We only check the borders of the rectangle. */
				struct point p, q;
				p.x = ps[i].x, p.y = ps[i].y;
				q.x = ps[i].x, q.y = ps[j].y;
				if (intersect(&p, &q, &ps[k], &ps[l])) goto next;
				q.x = ps[j].x, q.y = ps[i].y;
				if (intersect(&p, &q, &ps[k], &ps[l])) goto next;
				p.x = ps[j].x, p.y = ps[j].y;
				if (intersect(&p, &q, &ps[k], &ps[l])) goto next;
				q.x = ps[i].x, q.y = ps[j].y;
				if (intersect(&p, &q, &ps[k], &ps[l])) goto next;
			}
			ans2 = area > ans2 ? area : ans2;
next: ;
		}
	}

	free(ps);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
