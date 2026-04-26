#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct box {
	struct box *prev;
	struct box *next;
	int seen;

	int x;
	int y;
	int z;
};

struct con {
	struct box *a;
	struct box *b;
	long d;
};

void con_init(struct con *c, struct box *a, struct box *b)
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
	int const *xa = b;
	int const *xb = a;
	return (*xa < *xb) ? -1 : (*xa > *xb);
}

void isortappend(void *elem, void *base, size_t n, size_t size,
		int (*cmp)(void const *a, void const *b))
{
	char *p0 = (char *)base + n * size;
	memcpy(p0, elem, size);

	while (p0 != base) {
		char *p1 = p0 - size;
		if (cmp(p0, p1) >= 0) break;

		memcpy(elem, p0, size);
		memcpy(p0, p1, size);
		memcpy(p1, elem, size);

		p0 = p1;
	}
}

static int connect(struct con const *c)
{
	/* Search if already connected. */
	struct box *b;
	for (b = c->a->next; b != c->a; b = b->next) {
		if (b == c->b) {
			b = NULL;
			break;
		}
	}

	/* Concatenate. */
	if (b) {
		c->b->prev->next = c->a->next;
		c->a->next->prev = c->b->prev;
		c->a->next = c->b;
		c->b->prev = c->a;
		//con_print(c);
		return 1;
	}
	return 0;
}

#define BS_MAX 1024
#define CS_MAX (1024 * 512)
#define NC_PART1 1000

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	/* Boxes. */
	struct box *bs = calloc(BS_MAX, sizeof(*bs));
	struct box *b = bs;
	while (scanf(" %d,%d,%d ", &b->x, &b->y, &b->z) == 3) {
		b->prev = b;
		b->next = b;
		b->seen = 0;
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
	int ncon = NC_PART1;
	struct con *c;
	for (c = cs; ncon--; c++) {
		assert(c - cs < ncs);
		ncircuits -= connect(c);
	}

	/* Get the size of the 3 largest circuits. */
	int n3[4] = {0};
	for (i = 0; i < nbs; i++) {
		if (bs[i].seen) continue;

		/* Count. */
		bs[i].seen = 1;
		int n = 1;
		for (b = bs[i].next; b != &bs[i]; b = b->next) {
			b->seen = 1;
			n++;
		}

		isortappend(&n, n3, 3, sizeof(n), int_cmp_rev);
	}
	ans1 = n3[0] * n3[1] * n3[2];

	/*
	 * Part 2.
	 */
	while ((ncircuits -= connect(c)) > 1) {
		c++;
		assert(c - cs < ncs);
	}
	ans2 = c->a->x * c->b->x;

	free(bs);
	free(cs);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
