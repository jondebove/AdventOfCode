#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "utils.h"

static char *find(char *s, long len, long uniq)
{
	char *base[256] = { 0 };
	char *smax = s + len - uniq;
	char *t;
	for (t = s; t < s + uniq; t++) {
		char **p = &base[(unsigned char)(*t)];
		if (*p) {
			for (; s != *p; s++) {
				base[(unsigned char)(*s)] = NULL;
			}
			if (++s >= smax) {
				return NULL;
			}
		}
		*p = t;
	}
	return t;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

	buffer_getdelim(&b, EOF, stdin);

	ans1 = find(b.str, b.len,  4) - b.str;
	ans2 = find(b.str, b.len, 14) - b.str;

	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
