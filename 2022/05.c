#include <assert.h>
#include <stdio.h>
#include <sys/queue.h>

#include "utils.h"

struct crate {
	int id;
	STAILQ_ENTRY(crate) crates1;
	STAILQ_ENTRY(crate) crates2;
};

STAILQ_HEAD(stack, crate);

#define NSTACKS_MAX 16
#define NCRATES_MAX 256

int main(void)
{
	char ans1[NSTACKS_MAX + 1];
	char ans2[NSTACKS_MAX + 1];

	struct buffer b;
	buffer_create(&b);

	/* read 1st line */
	buffer_getdelim(&b, '\n', stdin);

	/* initialize */
	struct crate crates[NCRATES_MAX];
	struct crate *crate_end = crates;
	struct stack stacks1[NSTACKS_MAX];
	struct stack stacks2[NSTACKS_MAX];

	long nstacks = b.len / 4;
	assert(nstacks <= NSTACKS_MAX);
	for (long i = 0; i < nstacks; i++) {
		STAILQ_INIT(&stacks1[i]);
		STAILQ_INIT(&stacks2[i]);
	}

	/* parse initial state */
	do {
		for (long i = 0; i < nstacks; i++) {
			int id = b.str[4*i+1];
			if (id != ' ') {
				struct crate *c = crate_end++;
				assert(crate_end < crates + NCRATES_MAX);
				c->id = id;
				STAILQ_INSERT_TAIL(&stacks1[i], c, crates1);
				STAILQ_INSERT_TAIL(&stacks2[i], c, crates2);
			}
		}
	} while (!buffer_getdelim(&b, '\n', stdin) && b.str[1] != '1');

	/* skip empty line */
	buffer_getdelim(&b, '\n', stdin);

	/* move crates */
	while (!buffer_getdelim(&b, '\n', stdin)) {
		long n, from, to;
		sscanf(b.str, "move %ld from %ld to %ld", &n, &from, &to);
		if (n == 0 || from == to) {
			continue;
		}
		from--; to--;
		assert(n > 0);
		assert(from >= 0 && from < nstacks);
		assert(to >= 0 && to < nstacks);

		for (long i = n; i && !STAILQ_EMPTY(&stacks1[from]); i--) {
			struct crate *c = STAILQ_FIRST(&stacks1[from]);
			STAILQ_REMOVE_HEAD(&stacks1[from], crates1);
			STAILQ_INSERT_HEAD(&stacks1[to], c, crates1);
		}

		struct crate *p = STAILQ_FIRST(&stacks2[from]);
		STAILQ_REMOVE_HEAD(&stacks2[from], crates2);
		STAILQ_INSERT_HEAD(&stacks2[to], p, crates2);
		for (long i = n - 1; i && !STAILQ_EMPTY(&stacks2[from]); i--) {
			struct crate *c = STAILQ_FIRST(&stacks2[from]);
			STAILQ_REMOVE_HEAD(&stacks2[from], crates2);
			STAILQ_INSERT_AFTER(&stacks2[to], p, c, crates2);
			p = c;
		}
	}

	for (long i = 0; i < nstacks; i++) {
		struct crate *c;
		c = STAILQ_FIRST(&stacks1[i]);
		ans1[i] = c ? c->id : '.';
		c = STAILQ_FIRST(&stacks2[i]);
		ans2[i] = c ? c->id : '.';
	}
	ans1[nstacks] = '\0';
	ans2[nstacks] = '\0';

	buffer_destroy(&b);

	printf("%s %s\n", ans1, ans2);

	return 0;
}
