#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define NITEMS_MAX 32
struct monkey {
	long items[NITEMS_MAX];
	int nitems;
	/* operation */
	int op;
	long val;
	/* test */
	long div;
	struct monkey *next[2];
	long count;
};

#define NMONKEYS_MAX 16
static struct monkey monkeys[NMONKEYS_MAX];

static struct monkey *read(void)
{
	char c;
	long val;
	char s[8];

	int nscn = scanf(" Monkey %ld:", &val);
	if (nscn != 1) {
		return NULL;
	}

	assert(val < NMONKEYS_MAX);
	struct monkey *m = &monkeys[val];

	nscn = scanf(" Starting items%c", &c);
	assert(nscn == 1);
	do {
		nscn = scanf(" %ld%c", &val, &c);
		assert(nscn == 2);
		assert(m->nitems < NITEMS_MAX);
		m->items[m->nitems++] = val;
	} while (c == ',');

	nscn = scanf(" Operation: new = old %c %7s", &c, s);
	assert(nscn == 2);
	m->op = c;
	m->val = strcmp(s, "old") ? atol(s) : LONG_MIN;

	nscn = scanf(" Test: divisible by %ld", &val);
	assert(nscn == 1);
	m->div = val;
	nscn = scanf(" If true: throw to monkey %ld", &val);
	assert(nscn == 1);
	assert(val < NMONKEYS_MAX);
	m->next[1] = &monkeys[val];
	nscn = scanf(" If false: throw to monkey %ld", &val);
	assert(nscn == 1);
	assert(val < NMONKEYS_MAX);
	m->next[0] = &monkeys[val];

	m->count = 0;

	return m;
}

static void inspect(struct monkey *m, long mod)
{
	for (int i = 0; i < m->nitems; i++) {
		/* operate */
		long val = m->val == LONG_MIN ? m->items[i] : m->val;
		switch (m->op) {
			case '+': m->items[i] += val; break;
			case '-': m->items[i] -= val; break;
			case '*': m->items[i] *= val; break;
			case '/': m->items[i] /= val; break;
			default: assert(0);
		}

		if (mod) {
			m->items[i] %= mod;
		} else {
			m->items[i] /= 3L;
		}

		/* test */
		struct monkey *n = m->next[m->items[i] % m->div == 0];
		assert(n->nitems < NITEMS_MAX);
		n->items[n->nitems++] = m->items[i];

		m->count++;
	}
	m->nitems = 0;
}

static void die(char const *progname, char const *errmesg)
{
	fprintf(stderr, "%s: %s\nusage: %s part <input\n",
			progname, errmesg, progname);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	long ans = 0;

	if (argc != 2) {
		die(argv[0], "missing argument");
		return 1;
	}

	long mod;
	int nrounds;
	char *part = argv[1];
	if (strcmp(part, "1") == 0) {
		mod = 0;
		nrounds = 20;
	} else if (strcmp(part, "2") == 0) {
		mod = 1;
		nrounds = 10000;
	} else {
		die(argv[0], "invalid argument");
	}

	int nm = 0;
	struct monkey *m;
	while ((m = read())) {
		nm++;
		mod = lcm(m->div, mod);
	}

	for (int i = nrounds; i; i--) {
		for (int j = 0; j < nm; j++) {
			inspect(&monkeys[j], mod);
		}
	}

	long max[2] = { 0 };
	for (int j = 0; j < nm; j++) {
		long n = monkeys[j].count;
		if (n > max[0]) {
			max[1] = max[0];
			max[0] = n;
		} else if (n > max[1]) {
			max[1] = n;
		}
	}
	ans = max[0] * max[1];

	printf("%ld\n", ans);
	return 0;
}
