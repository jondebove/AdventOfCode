#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "fnv1a.h"
#include "utils.h"

struct table;

struct dir {
	char *name;
	struct dir *parent;
	struct table *children;
	long size;
	SHASH_ENTRY(dir) dirs;
};

SHASH_TABLE(table, dir);

static
struct dir *dir_new(char const *name, struct dir *parent, long size)
{
	struct dir *d = xrealloc(NULL, sizeof(*d));
	d->name = strdup(name);
	d->parent = parent;
	d->size = size;
	if (size) {
		d->children = NULL;
	} else {
		d->children = xrealloc(NULL, SHASH_TABLE_SIZE(table, 4));
		SHASH_INIT(d->children, 4);
	}
	return d;
}

static
void dir_free(struct dir *d)
{
	if (d) {
		if (d->children) {
			struct dir *c, *cc;
			SHASH_FOREACH_SAFE(c, d->children, dirs, cc) {
				dir_free(c);
			}
			free(d->children);
		}
		free(d->name);
	}
	free(d);
}

static
struct dir *dir_searchchild(struct dir *d, char const *name, long size)
{
	if (!d->children) {
		return NULL;
	}

	unsigned int h = fnv1a_str(name, FNV1A_SEED);
	struct dir *c;
	SHASH_SEARCH_FOREACH(c, h, d->children, dirs) {
		if (strcmp(name, c->name) == 0) {
			break;
		}
	}
	if (!c) {
		c = dir_new(name, d, size);
		SHASH_INSERT(d->children, c, h, dirs);
	} else {
		c->size = size;
	}
	return c;
}

static
void dir_print(struct dir *d, int level)
{
	for (int i = level; i; i--) {
		putc(' ', stdout);
	}
	fprintf(stdout, "- %s (%s, %ld)\n",
			d->name,
			d->children ? "dir" : "file",
			d->size);
	if (d->children) {
		struct dir *c;
		SHASH_FOREACH(c, d->children, dirs) {
			dir_print(c, level + 2);
		}
	}
}

static
long dir_updatesize(struct dir *d)
{
	if (d->children) {
		d->size = 0;
		struct dir *c;
		SHASH_FOREACH(c, d->children, dirs) {
			d->size += dir_updatesize(c);
		}
	}
	return d->size;
}

static
long dir_ans1(struct dir const *d, long max)
{
	long ans = 0;
	if (d->children) {
		if (d->size <= max) {
			ans += d->size;
		}
		struct dir *c;
		SHASH_FOREACH(c, d->children, dirs) {
			ans += dir_ans1(c, max);
		}
	}
	return ans;
}

static
long dir_ans2(struct dir const *d, long min, long max)
{
	if (d->children) {
		if (d->size >= min && d->size < max) {
			max = d->size;
		}
		struct dir *c;
		SHASH_FOREACH(c, d->children, dirs) {
			max = dir_ans2(c, min, max);
		}
	}
	return max;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

	/* we suppose we start from root directory */
	buffer_getdelim(&b, '\n', stdin);
	assert(memcmp(b.str, "$ cd /\n", 7) == 0);
	struct dir *root = dir_new("/", NULL, 0);
	struct dir *curr = root;

	while (!buffer_getdelim(&b, '\n', stdin)) {
		char *s = strtok(b.str, " \n");
		if (strcmp(s, "$") == 0) {
			s = strtok(NULL, " \n");
			if (strcmp(s, "cd") == 0) {
				s = strtok(NULL, " \n");
				if (strcmp(s, "..") == 0) {
					curr = curr->parent;
					assert(curr);
				} else if (strcmp(s, "/") == 0) {
					curr = root;
				} else {
					curr = dir_searchchild(curr, s, 0);
				}
			}
		} else if (strcmp("dir", s) != 0) {
			long n = atol(s);
			s = strtok(NULL, " \n");
			dir_searchchild(curr, s, n);
		}
	}

	dir_updatesize(root);
	dir_print(root, 0);

	ans1 = dir_ans1(root, 100000);
	ans2 = dir_ans2(root, root->size + (30000000 - 70000000), root->size);

	dir_free(root);
	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
