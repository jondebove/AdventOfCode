#include <stdio.h>

#include "utils.h"

struct range {
	int from;
	int to;
};

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

	while (!buffer_getdelim(&b, '\n', stdin)) {
		struct range r1, r2;
		sscanf(b.str, "%d-%d,%d-%d", &r1.from, &r1.to, &r2.from, &r2.to);
		if ((r1.from >= r2.from && r1.to <= r2.to) ||
		    (r1.from <= r2.from && r1.to >= r2.to)) {
			ans1++;
		}
		if (r1.to >= r2.from && r2.to >= r1.from) {
			ans2++;
		}
	}

	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
