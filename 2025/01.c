#include <stdio.h>

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	int x = 50;
	char d;
	int n;
	while (scanf(" %[LR]%d ", &d, &n) == 2) {
		int dx = d == 'R' ? 1 : -1;
		while (n--) {
			x += dx;
			if (x < 0) x += 100;
			else if (x >= 100) x -= 100;
			if (x == 0) ans2++;
		}
		if (x == 0) ans1++;
	}

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
