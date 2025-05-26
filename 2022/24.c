#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

#include "hashtable.h"
#include "utils.h"

enum dir { N, S, W, E, Z };

struct vec2 {
	int row;
	int col;
};

static struct vec2 const dirs[] = {
	[N]  = { -1,  0 },
	[S]  = {  1,  0 },
	[W]  = {  0, -1 },
	[E]  = {  0,  1 },
	[Z]  = {  0,  0 },
};

struct vertex {
	int row;
	int col;
	int t;

	SHASH_ENTRY(vertex) hash;
	STAILQ_ENTRY(vertex) queue;
	struct vertex *next[5 + 1];
	struct vertex *prev;
};

struct graph {
	unsigned int nvertices;
	struct vertex *vertices;
	SHASH_TABLE(hash, vertex) *htab;
};

void graph_create(struct graph *g, int shift)
{
	g->htab = xrealloc(NULL, SHASH_TABLE_SIZE(hash, shift));
	SHASH_INIT(g->htab, shift);

	g->vertices = xrealloc(NULL, sizeof(*g->vertices) * SHASH_SIZE(g->htab));
	memset(g->vertices, 0, sizeof(*g->vertices) * SHASH_SIZE(g->htab));

	g->nvertices = 0;
}

void graph_destroy(struct graph *g)
{
	free(g->vertices);
	free(g->htab);
}

struct vertex *graph_enter(struct graph *g, int row, int col, int t)
{
	unsigned int h = row + 50 * (col + 150 * t);
	struct vertex *vp;
	SHASH_SEARCH_FOREACH(vp, h, g->htab, hash) {
		if (vp->row == row && vp->col == col && vp->t == t) {
			break;
		}
	}
	if (!vp) {
		assert(g->nvertices < SHASH_SIZE(g->htab));
		vp = &g->vertices[g->nvertices++];
		vp->row = row;
		vp->col = col;
		vp->t = t;
		SHASH_INSERT(g->htab, vp, h, hash);
	}
	return vp;
}

static struct vertex *bfs(struct graph *g, struct vertex *s,
		bool (*done)(struct vertex *v, void *ctx), void *ctx)
{
	for (unsigned int i = g->nvertices; i;) {
		g->vertices[--i].prev = NULL;
	}

	STAILQ_HEAD(, vertex) q;
	STAILQ_INIT(&q);
	STAILQ_INSERT_TAIL(&q, s, queue);
	s->prev = s;

	while (!STAILQ_EMPTY(&q)) {
		struct vertex *v = STAILQ_FIRST(&q); 
		STAILQ_REMOVE_HEAD(&q, queue);
		if (done(v, ctx)) {
			return v;
		}
		for (struct vertex **u = v->next; *u; u++) {
			if (!(*u)->prev) {
				STAILQ_INSERT_TAIL(&q, *u, queue);
				(*u)->prev = v;
			}
		}
	}
	return NULL;
}

static bool done(struct vertex *v, void *ctx)
{
	return v->row == *(int *)ctx;
}

static void move_blizzards(struct grid *dst, struct grid const *src)
{
	memset(dst->data, 0, dst->nrow * (dst->ncol + 1));
	for (int i = 0; i < src->nrow; i++) {
		for (int j = 0; j < src->ncol; j++) {
			int c = *grid_at(src, i, j);
			if (c == 0) {
			} else if (c & (1U << Z)) {
				*grid_at(dst, i, j) = c;
			} else {
				if (c & (1U << N)) {
					*grid_at(dst, i != 1 ? i - 1 : src->nrow - 2, j) |= (1U << N);
				}
				if (c & (1U << S)) {
					*grid_at(dst, i != src->nrow - 2 ? i + 1 : 1, j) |= (1U << S);
				}
				if (c & (1U << W)) {
					*grid_at(dst, i, j != 1 ? j - 1 : src->ncol - 2) |= (1U << W);
				}
				if (c & (1U << E)) {
					*grid_at(dst, i, j != src->ncol - 2 ? j + 1 : 1) |= (1U << E);
				}
			}
		}
	}
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

	buffer_getdelim(&b, EOF, stdin);

	/* init grid */
	struct grid g, gn;
	grid_init(&g, b.str, b.len);
	grid_create(&gn, g.nrow, g.ncol, 0);

	/* use bitmap to handle blizzards */
	for (int i = 0; i < g.nrow; i++) {
		for (int j = 0; j < g.ncol; j++) {
			char *c = grid_at(&g, i, j);
			switch (*c) {
				case '^': *c = 1U << N; break;
				case 'v': *c = 1U << S; break;
				case '<': *c = 1U << W; break;
				case '>': *c = 1U << E; break;
				case '#': *c = 1U << Z; break;
				case '.': *c = 0; break;
				default: assert(0);
			}
		}
	}

	struct graph gr;
	graph_create(&gr, 20);

	/* search starting point */
	struct vertex *s = NULL;
	for (int j = 0; j < g.ncol; j++) {
		if (!*grid_at(&g, 0, j)) {
			s = graph_enter(&gr, 0, j, 0);
			break;
		}
	}

	/* build connectivities */
	int ntime = lcm(g.nrow - 2, g.ncol - 2);
	for (int t = 0; t < ntime; t++) {
		move_blizzards(&gn, &g);
		int tn = (t + 1) % ntime;
		for (int i = 0; i < g.nrow; i++) {
			for (int j = 0; j < g.ncol; j++) {
				if (*grid_at(&g, i, j)) {
					continue;
				}
				struct vertex *v = graph_enter(&gr, i, j, t);
				int k = 0;
				for (int d = N; d <= Z; d++) {
					int in = i + dirs[d].row;
					int jn = j + dirs[d].col;
					char *c = grid_at(&gn, in, jn);
					if (c && !*c) {
						v->next[k++] = graph_enter(&gr, in, jn, tn);
					}
				}
				assert(k < COUNTOF(v->next));
			}
		}
		struct grid tmp = g;
		g = gn;
		gn = tmp;
	}

	/* part 1 */
	int row = g.nrow - 1;
	s = bfs(&gr, s, done, &row);
	for (struct vertex *v = s; v && v != v->prev; v = v->prev) {
		ans1++;
	}

	/* part 2 */
	ans2 = ans1;
	row = 0;
	s = bfs(&gr, s, done, &row);
	for (struct vertex *v = s; v && v != v->prev; v = v->prev) {
		ans2++;
	}
	row = g.nrow - 1;
	s = bfs(&gr, s, done, &row);
	for (struct vertex *v = s; v && v != v->prev; v = v->prev) {
		ans2++;
	}

	/* cleanup */
	graph_destroy(&gr);
	grid_destroy(&gn);
	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
