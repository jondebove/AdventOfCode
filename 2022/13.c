#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define NNODES_MAX	(1L << 16)
#define NPACKETS_MAX	(1 << 10)

enum node_type {
	NODE_NUM,
	NODE_LIST,
};

struct node {
	enum node_type type;
	union {
		long num;
		struct node *next;
	} val;
	struct node *next;
};

static struct node nodes[NNODES_MAX];
static struct node *node_new = nodes;

static struct node *parse_list(char const *s, char **endptr)
{
	assert(*s == '[');

	struct node *const list = node_new++;
	assert(node_new - nodes <= NNODES_MAX);

	struct node *n = list;
	n->type = NODE_LIST;
	n->val.next = NULL;
	n->next = NULL;
	struct node **np = &n->val.next;

	char *end;
	for (s++;;) {
		switch (*s) {
		case '[':
			/* list */
			*np = parse_list(s, &end);
			n = *np;
			s = end;
			break;
		case '\n':
			assert(0);
			break;
		case ']':
			if (endptr) {
				*endptr = (char *)s + 1;
			}
			return list;
		case ',':
			np = &n->next;
			s++;
			break;
		default:
			/* number */
			*np = node_new++;
			assert(node_new - nodes <= NNODES_MAX);

			n = *np;
			n->type = NODE_NUM;
			n->val.num = strtol(s, &end, 10);
			n->next = NULL;
			assert(s != end);
			s = end;
			break;
		}
	}
}

static int compare_list(struct node const *n1, struct node const *n2)
{
	for (;;) {
		int ans;
		struct node n;
		if (!n1 && !n2) {
			return 0;
		}
		if (!n1 && n2) {
			return -1;
		}
		if (n1 && !n2) {
			return 1;
		}
		if (n1->type == NODE_NUM && n2->type == NODE_NUM) {
			if (n1->val.num < n2->val.num) {
				return -1;
			}
			if (n1->val.num > n2->val.num) {
				return 1;
			}
			goto next;
		}
		if (n1->type == NODE_LIST && n2->type == NODE_LIST) {
			if ((ans = compare_list(n1->val.next, n2->val.next))) {
				return ans;
			}
			goto next;
		}
		if (n1->type == NODE_NUM && n2->type == NODE_LIST) {
			n.type = NODE_NUM;
			n.val.num = n1->val.num;
			n.next = NULL;
			if ((ans = compare_list(&n, n2->val.next))) {
				return ans;
			}
			goto next;
		}
		if (n1->type == NODE_LIST && n2->type == NODE_NUM) {
			n.type = NODE_NUM;
			n.val.num = n2->val.num;
			n.next = NULL;

			if ((ans = compare_list(n1->val.next, &n))) {
				return ans;
			}
			goto next;
		}
next:
		n1 = n1->next;
		n2 = n2->next;
	}
	return 0;
}

static int compare_listptr(void const *p1, void const *p2)
{
	struct node *const *n1 = (struct node *const *)p1;
	struct node *const *n2 = (struct node *const *)p2;
	return compare_list(*n1, *n2);
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 1;

	struct buffer b;
	buffer_create(&b);

	struct node *packets[NPACKETS_MAX];
	struct node *p1, *p2;
	int npackets = 0;

	for (int i = 0;; i++) {
		buffer_getdelim(&b, '\n', stdin);
		p1 = parse_list(b.str, NULL);

		buffer_getdelim(&b, '\n', stdin);
		p2 = parse_list(b.str, NULL);

		if (compare_list(p1, p2) < 0) {
			ans1 += i + 1;
		}

		packets[npackets++] = p1;
		assert(npackets <= NPACKETS_MAX);
		packets[npackets++] = p2;
		assert(npackets <= NPACKETS_MAX);

		if (buffer_getdelim(&b, '\n', stdin)) {
			break;
		}
	}

	p1 = parse_list("[[2]]", NULL);
	p2 = parse_list("[[6]]", NULL);
	packets[npackets++] = p1;
	packets[npackets++] = p2;
	qsort(packets, npackets, sizeof(*packets), compare_listptr);
	for (int i = 0; i < npackets; i++) {
		if (packets[i] == p1 || packets[i] == p2) {
			ans2 *= i + 1;
		}
	}

	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
