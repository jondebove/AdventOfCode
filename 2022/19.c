#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"
#include "utils.h"

enum resource {
	NONE = -1,  /* force int */
	ORE,
	CLAY,
	OBSIDIAN,
	GEODE,
	END,
};

struct blueprint {
	int id;
	int costs[END][END];
	int costs_max[END];
};

static int blueprint_read(struct blueprint *b)
{
	*b = (struct blueprint){ 0 };

	if (7 != scanf(" Blueprint %d: "
			"Each ore robot costs %d ore. "
			"Each clay robot costs %d ore. "
			"Each obsidian robot costs %d ore and %d clay. "
			"Each geode robot costs %d ore and %d obsidian.",
			&b->id, &b->costs[ORE][ORE], &b->costs[CLAY][ORE],
			&b->costs[OBSIDIAN][ORE], &b->costs[OBSIDIAN][CLAY],
			&b->costs[GEODE][ORE], &b->costs[GEODE][OBSIDIAN])) {
		return -1;
	}

	for (enum resource r = OBSIDIAN; r >= ORE; r--) {
		for (enum resource rb = GEODE; rb >= ORE; rb--) {
			b->costs_max[r] = MAX(b->costs_max[r], b->costs[rb][r]);
		}
	}
	return 0;
}

struct state {
	int t;
	int robots[END];
	int resources[END];
	long ans;
	SHASH_ENTRY(state) ht;
};

static unsigned int state_hash(struct state const *s)
{
	unsigned int h = 0;
#define X(x) (h = h * 31U + (s->x))
	X(t);
	for (enum resource r = GEODE; r >= ORE; r--) {
		X(robots[r]);
		X(resources[r]);
	}
#undef X
	return h;
}

static int state_equal(struct state const *a, struct state const *b)
{
#define X(x) if (a->x != b->x) return 0
	X(t);
	for (enum resource r = GEODE; r >= ORE; r--) {
		X(robots[r]);
		X(resources[r]);
	}
	return 1;
#undef X
}

/* memoization */
SHASH_TABLE(ht_state, state);

struct memo_state {
	struct ht_state *cache;
	struct state *pool;
	long cap;
	long len;
};

static void dfs(struct blueprint const *b, struct state *s, struct memo_state *m)
{
	if (s->t == 0) {
		s->ans = s->resources[GEODE];
		return;
	}

	/* remove states which cannot win in remaining time */
	if (s->ans >= s->resources[GEODE] + s->robots[GEODE] * s->t +
			s->t * (s->t - 1) / 2) {
		return;
	}

	/* remove states with too many robots */
	for (enum resource r = OBSIDIAN; r >= ORE; r--) {
		if (s->robots[r] > b->costs_max[r]) {
			return;
		}
	}

	/* memo_find */
	unsigned int h = state_hash(s);
	struct state *sp;
	SHASH_SEARCH_FOREACH(sp, h, m->cache, ht) {
		if (state_equal(s, sp)) {
			s->ans = sp->ans;
			return;
		}
	}

	/* loop on robots */
	for (enum resource r = GEODE; r >= ORE;) {
		struct state ss = *s;
		/* loop on resources */
		for (enum resource rr = r; rr >= ORE; rr--) {
			if ((ss.resources[rr] -= b->costs[r][rr]) < 0) {
				goto next_robot;
			}
		}
		/* loop on resources */
		for (enum resource rr = GEODE; rr >= ORE; rr--) {
			ss.resources[rr] += ss.robots[rr];
		}
		ss.robots[r]++;
		ss.t--;
		dfs(b, &ss, m);
		s->ans = MAX(s->ans, ss.ans);
next_robot:
		r--;
	}

	/* no robot */
	{
		struct state ss = *s;
		for (enum resource rr = GEODE; rr >= ORE; rr--) {
			ss.resources[rr] += ss.robots[rr];
		}
		ss.t--;
		dfs(b, &ss, m);
		s->ans = MAX(s->ans, ss.ans);
	}

	/* memo_enter */
	assert(m->len <= m->cap);
	sp = &m->pool[m->len++];
	*sp = *s;
	SHASH_INSERT(m->cache, sp, h, ht);
}

static long ngeodes_max(struct blueprint const *b, int t, int shift)
{
	/* memo_create */
	struct memo_state m;
	m.cache = xrealloc(NULL, SHASH_TABLE_SIZE(ht_state, shift));
	SHASH_INIT(m.cache, shift);
	m.len = 0;
	m.cap = SHASH_SIZE(m.cache);
	m.pool = xrealloc(NULL, m.cap * sizeof(*m.pool));

	struct state s = { .t = t, .robots = { [ORE] = 1 } };
	dfs(b, &s, &m);

	/* memo_destroy */
	free(m.cache);
	free(m.pool);

	return s.ans;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 1;

	/* read input */
	struct blueprint b;
	while (!blueprint_read(&b)) {
		//printf("%d\n", b.id);
		ans1 += b.id * ngeodes_max(&b, 24, 20);

		if (b.id <= 3) {
			ans2 *= ngeodes_max(&b, 32, 22);
		}
	}

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
