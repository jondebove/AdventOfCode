#define _XOPEN_SOURCE 700
#include <assert.h>
#include <search.h>
#include <stdio.h>

#include "utils.h"

#define NMAX 5000

struct num {
	struct qnode list;
	long val;
};

static void init(struct num *nums, long len, struct qnode *q)
{
	q->next = q->prev = NULL;
	while (len--) {
		insque(&nums[len], q);
	}
}

static void mix(struct num *nums, long len, struct qnode *q)
{
	for (long i = 0; i < len; i++) {
		struct num *n = &nums[i];

		long val = n->val % (len - 1);
		if (val == 0) {
			continue;
		}
		if (val < 0) {
			val += len - 1;
		}

		struct num *p = n;
		while (val--) {
			p = p->list.next;
			if (!p) {
				p = q->next;
			}
		}
		remque(n);
		insque(n, p);
	}
}

static long index(struct qnode const *q, long val)
{
	long idx = 0;
	struct num *n;
	for (n = q->next; n; n = n->list.next) {
		if (n->val == val) {
			return idx;
		}
		idx++;
	}
	return -1;
}

static long value(struct qnode const *q, long idx)
{
	struct num *n;
	for (n = q->next; n; n = n->list.next) {
		if (idx-- == 0) {
			return n->val;
		}
	}
	return 0;
}

static long calc(struct qnode const *q, long len)
{
	long offset = index(q, 0);
	return  value(q, (1000 + offset) % len) +
		value(q, (2000 + offset) % len) +
		value(q, (3000 + offset) % len);
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	/* read input */
	struct num nums[NMAX];
	long len = 0;
	{
		struct num *n = nums;
		for (long val; scanf("%ld", &val) == 1;) {
			assert(n - nums < NMAX);
			n->val = val;
			n++;
		}
		len = n - nums;
	}

	/* part 1 */
	struct qnode q;
	init(nums, len, &q);
	mix(nums, len, &q);
	ans1 = calc(&q, len);

	/* part 2 */
	for (long i = 0; i < len; i++) {
		nums[i].val *= 811589153;
	}
	init(nums, len, &q);
	for (long i = 0; i < 10; i++) {
		mix(nums, len, &q);
	}
	ans2 = calc(&q, len);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
