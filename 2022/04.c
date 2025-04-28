#include <stdio.h>

struct range {
	int from;
	int to;
};

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct range r1, r2;
	while (scanf("%d-%d,%d-%d", &r1.from, &r1.to, &r2.from, &r2.to) == 4) {
		if ((r1.from >= r2.from && r1.to <= r2.to) ||
		    (r1.from <= r2.from && r1.to >= r2.to)) {
			ans1++;
		}
		if (r1.to >= r2.from && r2.to >= r1.from) {
			ans2++;
		}
	}

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
