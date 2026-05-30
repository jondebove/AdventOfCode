#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define NSTACKS_MAX 16
#define NCRATES_MAX 127

struct stack {
	int c[NCRATES_MAX];
	int n;
};

int main(void)
{
	char ans1[NSTACKS_MAX + 1];
	char ans2[NSTACKS_MAX + 1];

	struct buffer b;
	buffer_create(&b);

	/* read 1st line */
	buffer_getdelim(&b, '\n', stdin);

	/* initialize */
	struct stack stacks1[NSTACKS_MAX] = { 0 };
	int nstacks = b.len / 4;
	assert(nstacks <= NSTACKS_MAX);

	int i;
	/* parse initial state */
	do {
		for (i = 0; i < nstacks; i++) {
			int id = b.str[4*i+1];
			if (id != ' ') {
				stacks1[i].c[stacks1[i].n++] = id;
			}
		}
	} while (!buffer_getdelim(&b, '\n', stdin) && b.str[1] != '1');

	/* reverse */
	for (i = 0; i < nstacks; i++) {
		int j, k;
		for (j = 0, k = stacks1[i].n - 1; j < k; j++, k--) {
			int tmp = stacks1[i].c[j];
			stacks1[i].c[j] = stacks1[i].c[k];
			stacks1[i].c[k] = tmp;
		}
	}

	/* duplicate for part 2 */
	struct stack stacks2[NSTACKS_MAX];
	memcpy(stacks2, stacks1, sizeof(stacks1));

	/* skip empty line */
	buffer_getdelim(&b, '\n', stdin);

	/* move crates */
	while (!buffer_getdelim(&b, '\n', stdin)) {
		int n, from, to;
		sscanf(b.str, "move %d from %d to %d", &n, &from, &to);
		if (n == 0 || from == to) continue;

		/* part 1 */
		struct stack *src = &stacks1[from - 1];
		struct stack *dst = &stacks1[to - 1];
		for (i = n < src->n ? n : src->n; i; i--) {
			dst->c[dst->n++] = src->c[--src->n];
		}

		/* part 2 */
		src = &stacks2[from - 1];
		dst = &stacks2[to - 1];
		n = n < src->n ? n : src->n;
		for (i = n; i; i--) {
			dst->c[dst->n++] = src->c[src->n - i];
		}
		src->n -= n;
	}

	for (i = 0; i < nstacks; i++) {
		ans1[i] = stacks1[i].c[stacks1[i].n - 1];
		ans2[i] = stacks2[i].c[stacks2[i].n - 1];
	}
	ans1[nstacks] = '\0';
	ans2[nstacks] = '\0';

	buffer_destroy(&b);

	printf("%s %s\n", ans1, ans2);
	return 0;
}
