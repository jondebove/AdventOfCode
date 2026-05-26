#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct box {
	struct box *prev;
	int count;

	long x;
	long y;
	long z;
};

struct con {
	struct box *a;
	struct box *b;
	long d;
};

static void con_init(struct con *c, struct box *a, struct box *b)
{
	c->a = a;
	c->b = b;

	long d;
	d = a->x - b->x; c->d  = d * d;
	d = a->y - b->y; c->d += d * d;
	d = a->z - b->z; c->d += d * d;
}

static int con_cmp(void const *a, void const *b)
{
	struct con const *xa = a;
	struct con const *xb = b;
	return (xa->d < xb->d) ? -1 : (xa->d > xb->d);
}

static int int_cmp_rev(void const *a, void const *b)
{
	int const *xa = a;
	int const *xb = b;
	return (*xa > *xb) ? -1 : (*xa < *xb);
}

static void isortappend(void const *elem, void *base, size_t n, size_t size,
		int (*cmp)(void const *a, void const *b))
{
	char *p = (char *)base + n * size;
	while (p != base && cmp(p - size, elem) > 0) {
		memcpy(p, p - size, size);
		p -= size;
	}
	memcpy(p, elem, size);
}

static int connect(struct con const *c)
{
	/* Find with path compression. */
	struct box *a, *b, *p;
	for (a = c->a; a->prev != a; a = p) {
		p = a->prev;
		a->prev = p->prev;
	}
	for (b = c->b; b->prev != b; b = p) {
		p = b->prev;
		b->prev = p->prev;
	}

	/* Union. */
	if (a == b) return 0;

	if (a->count < b->count) {
		a->prev = b;
		b->count += a->count;
	} else {
		b->prev = a;
		a->count += b->count;
	}
	return 1;
}

#define BS_MAX 1024
#define CS_MAX (1024 * 512)

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	/* Boxes. */
	struct box *bs = calloc(BS_MAX, sizeof(*bs));
	struct box *b = bs;
	while (scanf(" %ld,%ld,%ld ", &b->x, &b->y, &b->z) == 3) {
		b->prev = b;
		b->count = 1;
		b++;
		assert(b - bs < BS_MAX);
	}
	int nbs = b - bs;
	int ncircuits = nbs;

	/* Connections. */
	struct con *cs = calloc(CS_MAX, sizeof(*cs));
	int i, j;
	int ncs = 0;
	for (i = 0; i < nbs; i++) {
		for (j = i + 1; j < nbs; j++) {
			con_init(&cs[ncs++], &bs[i], &bs[j]);
		}
	}
	qsort(cs, ncs, sizeof(*cs), con_cmp);

	/*
	 * Part 1.
	 */
	int ncon = nbs > 100 ? 1000 : 10;
	struct con *c;
	for (c = cs; ncon--; c++) {
		assert(c - cs < ncs);
		ncircuits -= connect(c);
	}

	/* Get the size of the 3 largest circuits. */
	int n[4] = {0};
	for (i = 0; i < nbs; i++) {
		if (bs[i].prev != &bs[i]) continue;
		isortappend(&bs[i].count, n, 3, sizeof(*n), int_cmp_rev);
	}
	ans1 = n[0] * n[1] * n[2];

	/*
	 * Part 2.
	 */
	for (;; c++) {
		assert(c - cs < ncs);
		ncircuits -= connect(c);
		if (ncircuits == 1) break;
	}
	assert(c->a->prev->count == nbs);
	ans2 = c->a->x * c->b->x;

	free(bs);
	free(cs);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
