#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

#include "utils.h"

struct vertex {
	int data;

	STAILQ_ENTRY(vertex) queue;
	struct vertex *next[4 + 1];
	struct vertex *prev;
	bool seen;
};
STAILQ_HEAD(queue, vertex);

static
struct vertex *bfs(struct vertex *vertices, long nvertices,
		struct vertex *s,
		bool (*done)(struct vertex *v, void *ctx), void *ctx)
{
	assert(nvertices > 0);

	while (nvertices--) {
		vertices->seen = 0;
		vertices++;
	}

	struct queue q;
	STAILQ_INIT(&q);
	STAILQ_INSERT_TAIL(&q, s, queue);
	s->seen = true;

	while (!STAILQ_EMPTY(&q)) {
		struct vertex *v = STAILQ_FIRST(&q); 
		STAILQ_REMOVE_HEAD(&q, queue);
		if (done(v, ctx)) {
			return v;
		}
		for (struct vertex **u = v->next; *u; u++) {
			if (!(*u)->seen) {
				STAILQ_INSERT_TAIL(&q, *u, queue);
				(*u)->seen = true;
				(*u)->prev = v;
			}
		}
	}
	return NULL;
}

static
bool done1(struct vertex *v, void *ctx)
{
	return v == (struct vertex *)ctx;
}

static
bool done2(struct vertex *v, void *ctx)
{
	return v->data == *(int *)ctx;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

	buffer_getdelim(&b, EOF, stdin);

	/* init grid */
	struct grid g;
	grid_init(&g, b.str, b.len);

	/* make graph */
	long nvertices = g.nrow * g.ncol;
	struct vertex *vertices = xrealloc(NULL, sizeof(*vertices) * nvertices);
	memset(vertices, 0, sizeof(*vertices) * nvertices);

	/* find endpoints */
	struct vertex *s = NULL;
	struct vertex *e = NULL;
	struct vertex *v = vertices;
	for (long i = 0; i < g.nrow; i++) {
		for (long j = 0; j < g.ncol; j++) {
			char *c = grid_at(&g, i, j);
			if (*c == 'S') {
				e = v;
				*c = 'a';
			} else if (*c == 'E') {
				s = v;
				*c = 'z';
			}
			v++;
		}
	}
	assert(s);
	assert(e);

	/* build connectivities */
	v = vertices;
	for (long i = 0; i < g.nrow; i++) {
		for (long j = 0; j < g.ncol; j++) {
			v->data = *grid_at(&g, i, j);
			int c = v->data - 1;
			char *p;
			int k = 0;
			if ((p = grid_at(&g, i, j + 1)) && *p >= c) {
				v->next[k++] = &vertices[(j+1)+i*g.ncol];
			}
			if ((p = grid_at(&g, i, j - 1)) && *p >= c) {
				v->next[k++] = &vertices[(j-1)+i*g.ncol];
			}
			if ((p = grid_at(&g, i + 1, j)) && *p >= c) {
				v->next[k++] = &vertices[j+(i+1)*g.ncol];
			}
			if ((p = grid_at(&g, i - 1, j)) && *p >= c) {
				v->next[k++] = &vertices[j+(i-1)*g.ncol];
			}
			v++;
		}
	}

	for (v = bfs(vertices, nvertices, s, done1, e);
			v && v != s; v = v->prev) {
		ans1++;
	}
	for (v = bfs(vertices, nvertices, s, done2, &e->data);
			v && v != s; v = v->prev) {
		ans2++;
	}

	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
