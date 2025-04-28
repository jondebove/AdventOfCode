#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static void render(long c, long x)
{
	c %= 40;
	putchar(labs(c - x) <= 1 ? '#' : '.');
	if (c == 39) {
		putchar('\n');
	}
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

	long x = 1;
	long c = 0;
	long nc = 20;
	while (!buffer_getdelim(&b, '\n', stdin)) {
		char s[8];
		long val;
		sscanf(b.str, "%s %ld", s, &val);
		if (strcmp(s, "noop") == 0) {
			val = 0;
			render(c++, x);
		} else {
			render(c++, x);
			render(c++, x);
		}
		if (c >= nc) {
			ans1 += nc * x;
			nc += 40;
		}
		x += val;
	}

	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
