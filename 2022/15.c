#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

#include "utils.h"

struct sensor {
	/* position of sensor */
	long x;
	long y;
	long d;
};

struct range {
	long min;
	long max;
	LIST_ENTRY(range) list;
};
LIST_HEAD(rlist, range);

static struct range *range_new(long min, long max)
{
	struct range *r = xrealloc(NULL, sizeof(*r));
	r->min = min;
	r->max = max;
	return r;
}

struct rlist scan_row(struct sensor *s, long n, long row)
{
	struct rlist ranges;
	LIST_INIT(&ranges);

	for (; n--; s++) {
		long const dr = s->d - labs(s->y - row);
		if (dr < 0) {
			continue;
		}

		struct range *r0 = range_new(s->x - dr, s->x + dr);
		struct range *r, *rn;
		for (r = LIST_FIRST(&ranges); r; r = rn) {
			rn = LIST_NEXT(r, list);
			if (r->min > r0->max || r->max < r0->min) {
				/* disjoint */
			} else {
				r0->min = MIN(r0->min, r->min);
				r0->max = MAX(r0->max, r->max);
				LIST_REMOVE(r, list);
				free(r);
			}
		}
		LIST_INSERT_HEAD(&ranges, r0, list);
	}
	return ranges;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

#define NSENSORS_MAX 256
	struct sensor sensors[NSENSORS_MAX];
	int nsensors = 0;

	while (!buffer_getdelim(&b, '\n', stdin)) {
		struct sensor *s = &sensors[nsensors++];
		long bx, by;
		int nscn = sscanf(b.str, "Sensor at x=%ld, y=%ld: "
				"closest beacon is at x=%ld, y=%ld",
				&s->x, &s->y, &bx, &by);
		assert(nscn == 4);
		s->d = labs(s->x - bx) + labs(s->y - by);
	}

	buffer_destroy(&b);

	/* part 1 */
	struct rlist ranges = scan_row(sensors, nsensors, 2000000);
	struct range *r, *rn;
	for (r = LIST_FIRST(&ranges); r; r = rn) {
		rn = LIST_NEXT(r, list);
		ans1 += r->max - r->min; // no +1 because a beacon is already there
		free(r);
	}

	for (long i = 0; i < 4000000; i++) {
		struct rlist ranges = scan_row(sensors, nsensors, i);
		struct range r0 = { .min = 0, .max = 4000000 };
		struct range *r, *rn;
		for (r = LIST_FIRST(&ranges); r; r = rn) {
			rn = LIST_NEXT(r, list);
			if (r->min > r0.max || r->max < r0.min) {
				/* disjoint */
			} else {
				if (r->min <= r0.min) {
					r0.min = r->max + 1;
				}
				if (r->max >= r0.max) {
					r0.max = r->min - 1;
				}
			}
			free(r);
		}
		if (r0.min == r0.max) {
			ans2 = r0.min * 4000000 + i;
			break;
		}
	}

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
