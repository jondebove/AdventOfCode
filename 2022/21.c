#include <assert.h>
#include <search.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define NMONKEYS_MAX (1L << 12)
#define VAL_MAX      (1L << 48)

struct monkey {
	char name[8];
	bool hasval;
	int op;
	struct monkey *args[2];
	long val;
};

static int monkey_cmp(void const *pa, void const *pb)
{
	struct monkey const *a = (struct monkey const *)pa; 
	struct monkey const *b = (struct monkey const *)pb; 
	return strcmp(a->name, b->name);
}

struct monkey_slice {
	struct monkey data[NMONKEYS_MAX];
	long len;
};

static struct monkey *monkey_search(struct monkey_slice *ms, char const *name)
{
	struct monkey key = { 0 };
	strncpy(key.name, name, sizeof(key.name) - 1);

	size_t n = ms->len;
	assert(n < NMONKEYS_MAX);

	struct monkey *m = lsearch(&key, ms->data, &n, sizeof(*m), monkey_cmp);
	ms->len = n;

	return m;
}

static void dfs(struct monkey *m)
{
	if (m->hasval) {
		return;
	}

	dfs(m->args[0]);
	dfs(m->args[1]);

	switch (m->op) {
		case '+': m->val = m->args[0]->val + m->args[1]->val; break;
		case '-': m->val = m->args[0]->val - m->args[1]->val; break;
		case '*': m->val = m->args[0]->val * m->args[1]->val; break;
		case '/': m->val = m->args[0]->val / m->args[1]->val; break;
		default: assert(0);
	}
	m->hasval = true;
}

static long getval(long x, void *ctx)
{
	struct monkey_slice *ms = (struct monkey_slice *)ctx;
	struct monkey *root = NULL, *humn = NULL;
	for (struct monkey *m = ms->data; m != ms->data + ms->len; m++) {
		m->hasval = m->op == '\0';
		if (!root && strcmp(m->name, "root") == 0) {
			root = m;
		} else if (!humn && strcmp(m->name, "humn") == 0) {
			humn = m;
		}
	}
	assert(root && humn);
	humn->val = x;
	dfs(root);
	return root->val;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	/* read input */
	struct buffer b;
	buffer_create(&b);

	struct monkey_slice monkeys = { 0 };

	while (!buffer_getdelim(&b, '\n', stdin)) {
		char name[8], arg0[8], arg1[8], op[2];
		long val;
		if (sscanf(b.str, "%7[a-z]: %ld", name, &val) == 2) {
			struct monkey *m = monkey_search(&monkeys, name);
			m->hasval = true;
			m->val = val;
		} else if (sscanf(b.str, "%7[a-z]: %7[a-z] %1[+-*/] %7[a-z]",
					name, arg0, op, arg1) == 4) {
			struct monkey *m = monkey_search(&monkeys, name);
			m->op = op[0];
			m->args[0] = monkey_search(&monkeys, arg0);
			m->args[1] = monkey_search(&monkeys, arg1);
		} else {
			assert(0);
		}
	}

	buffer_destroy(&b);

	/* part 1 */
	ans1 = getval(monkey_search(&monkeys, "humn")->val, &monkeys);

	/* part 2 */
	monkey_search(&monkeys, "root")->op = '-';
	ans2 = bisectl(getval, 0, VAL_MAX, &monkeys);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
