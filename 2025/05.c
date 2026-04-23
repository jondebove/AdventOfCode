#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct range {
	long min;
	long max;
};

int long_cmp(void const *a, void const *b)
{
	long const *xa = a;
	long const *xb = b;
	return (*xa < *xb) ? -1 : (*xa > *xb);
}

int range_cmp(void const *a, void const *b)
{
	struct range const *ra = a;
	struct range const *rb = b;
	int cmp = long_cmp(&ra->min, &rb->min);
	return cmp ? cmp : long_cmp(&ra->max, &rb->max);
}

int range_has(void const *a, void const *b)
{
	long const *x = a;
	struct range const *r = b;
	if (*x < r->min) return -1;
	if (*x > r->max) return 1;
	return 0;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	char line[1024] = { 0 };

	/* Read ranges. */
	struct range rs[256] = { 0 };
	long nr = 0;
	while (fgets(line, 1024, stdin)) {
		int n = sscanf(line, " %ld-%ld ", &rs[nr].min, &rs[nr].max);
		if (n != 2) break;
		assert(nr < 256);
		nr++;
	}

	/* Merge. */
	qsort(rs, nr, sizeof(*rs), range_cmp);
	long r, i;
	for (i = 0, r = 1; r < nr; r++) {
		if (rs[r].min <= rs[i].max + 1) {
			if (rs[i].max < rs[r].max) {
				rs[i].max = rs[r].max;
			}
		} else {
			rs[++i] = rs[r];
		}
	}
	nr = i + 1;

	/* Part 1. */
	for (;;) {
		int n = scanf(" %ld ", &i);
		if (n != 1) break;
		if (bsearch(&i, rs, nr, sizeof(*rs), range_has)) {
			ans1++;
		}
	}

	/* Part 2. */
	for (r = 0; r < nr; r++) {
		ans2 += rs[r].max - rs[r].min + 1;
	}

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
