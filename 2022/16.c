#include <assert.h>
#include <limits.h>
#include <search.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define NVERTICES_MAX 64

struct edge {
	struct vertex *next;
	long weight;
};

struct vertex {
	char name[4];
	bool open;
	long rate;
	unsigned long bits;

	struct edge edges[NVERTICES_MAX];
	long nedges;
};

static int vertex_cmp(void const *pa, void const *pb)
{
	struct vertex const *va = (struct vertex const *)pa; 
	struct vertex const *vb = (struct vertex const *)pb; 
	return strcmp(va->name, vb->name);
}

static struct vertex *vertex_search(char const *s, struct vertex *v, long *n)
{
	struct vertex key = { 0 };
	strncpy(key.name, s, sizeof(key.name) - 1);

	size_t nv = *n;
	v = lsearch(&key, v, &nv, sizeof(*v), vertex_cmp);
	assert(nv <= NVERTICES_MAX);
	*n = nv;

	return v;
}

static void floydwarshall(struct vertex *v, long n)
{
	for (long i = 0; i < n; i++) {
		v[i].nedges = n;
		for (long j = 0; j < n; j++) {
			struct edge *eij = &v[i].edges[j];
			if (!eij->next) {
				eij->next = &v[j];
				eij->weight = LONG_MAX;
			}
		}
	}
	for (long k = 0; k < n; k++) {
		for (long i = 0; i < n; i++) {
			long const wik = v[i].edges[k].weight;
			for (long j = 0; j < n; j++) {
				long const wkj = v[k].edges[j].weight;
				long const wij = wik == LONG_MAX || wkj ==
					LONG_MAX ? LONG_MAX : wik + wkj;
				if (wij < v[i].edges[j].weight) {
					v[i].edges[j].weight = wij;
				}
			}
		}
	}
}

static void dfs(struct vertex *v, long t, long *scores, unsigned long key, long val)
{
	v->open = true;

	val += t * v->rate;
	key |= v->bits;
	if (scores[key] < val) {
		scores[key] = val;
	}

	for (long i = 0; i < v->nedges; i++) {
		struct edge *e = &v->edges[i];
		struct vertex *u = e->next;
		long w = t - e->weight - 1;
		if (w > 0 && !u->open) {
			dfs(u, w, scores, key, val);
		}
	}

	v->open = false;
}

static long *walk(struct vertex *v, long t, long *scores, long size)
{
	scores = xrealloc(scores, size * sizeof(*scores));
	memset(scores, 0, size * sizeof(*scores));
	dfs(v, t, scores, 0, 0);
	return scores;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct vertex vertices[NVERTICES_MAX] = { 0 };
	long nvertices = 0;

	/* read input */
	struct buffer b;
	buffer_create(&b);

	while (!buffer_getdelim(&b, '\n', stdin)) {
		char name[4];
		long rate;
		char tunnels[8][4];
		int nscn = sscanf(b.str, "Valve %3[A-Z] has flow rate=%ld;"
				" %*s %*s to %*s"
				" %3[A-Z], %3[A-Z], %3[A-Z], %3[A-Z]"
				" %3[A-Z], %3[A-Z], %3[A-Z], %3[A-Z]",
				name, &rate,
				tunnels[0], tunnels[1],
				tunnels[2], tunnels[3],
				tunnels[4], tunnels[5],
				tunnels[6], tunnels[7]);
		assert(nscn >= 3 && nscn <= 9);

		struct vertex *v = vertex_search(name, vertices, &nvertices);
		v->rate = rate;
		v->nedges = nscn - 2;

		v->edges[v - vertices].next = v;
		v->edges[v - vertices].weight = 0;
		for (long i = 0; i < v->nedges; i++) {
			struct vertex *u = vertex_search(tunnels[i], vertices, &nvertices);
			v->edges[u - vertices].next = u;
			v->edges[u - vertices].weight = 1;
		}
	}

	buffer_destroy(&b);

	/* make dense connectivity matrix */
	floydwarshall(vertices, nvertices);

	/* remove broken valves */
	long idx = 0;
	for (long i = 0; i < nvertices; i++) {
		struct vertex *v = &vertices[i];
		if (v->rate != 0) {
			v->bits = 1UL << idx++;
		}
		for (long j = v->nedges - 1; j >= 0; j--) {
			if (v->edges[j].next->rate == 0 || i == j) {
				v->edges[j] = v->edges[--v->nedges];
			}
		}
	}

	struct vertex *aa = vertex_search("AA", vertices, &nvertices);
	unsigned long const n = 1UL << idx;
	long *scores = NULL;

	/* part 1 */
	scores = walk(aa, 30, scores, n);
	for (unsigned long i = n - 1; i; i--) {
		ans1 = MAX(ans1, scores[i]);
	}

	/* part 2 */
	scores = walk(aa, 26, scores, n);
	for (unsigned long i = n - 1; i; i--) {
		if (scores[i] > 0) {
			for (unsigned long j = 0; j < i; j++) {
				if (scores[j] > 0 && (i & j) == 0) {
					long const ans = scores[i] + scores[j];
					ans2 = MAX(ans2, ans);
				}
			}
		}
	}

	free(scores);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
