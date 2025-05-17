#include <assert.h>
#include <stdio.h>
#include <sys/queue.h>

#include "utils.h"

#define NMAX 5000

struct num {
	long val;
	LIST_ENTRY(num) q;
};
LIST_HEAD(qnum, num);

static void init(struct num *nums, long len, struct qnum *q)
{
	LIST_INIT(q);
	while (len--) {
		LIST_INSERT_HEAD(q, &nums[len], q);
	}
}

static void mix(struct num *nums, long len, struct qnum *q)
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
			p = LIST_NEXT(p, q);
			if (!p) {
				p = LIST_FIRST(q);
			}
		}
		LIST_REMOVE(n, q);
		LIST_INSERT_AFTER(p, n, q);
	}
}

static long index(struct qnum const *q, long val)
{
	long idx = 0;
	struct num *n;
	LIST_FOREACH(n, q, q) {
		if (n->val == val) {
			return idx;
		}
		idx++;
	}
	return -1;
}

static long value(struct qnum const *q, long idx)
{
	struct num *n;
	LIST_FOREACH(n, q, q) {
		if (idx-- == 0) {
			return n->val;
		}
	}
	return 0;
}

static long calc(struct qnum const *q, long len)
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
	struct qnum q;
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
