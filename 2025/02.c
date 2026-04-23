#include <stdio.h>
#include <string.h>

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	long i, i2;
	while (scanf(" %ld-%ld%*c", &i, &i2) == 2) {
		for (; i <= i2; i++) {
			char s[22];
			int n = snprintf(s, sizeof(s), "%ld", i);
			if ((n & 1) == 0 && memcmp(s, s + n / 2, n / 2) == 0) {
				ans1 += i;
				continue;
			}
			for (int j = n / 3; j > 0; j--) {
				if (n % j) continue;
				int k;
				for (k = j; k < n; k += j) {
					if (memcmp(s, s + k, j) != 0) break;
				}
				if (k == n) {
					ans2 += i;
					break;
				}
			}
		}
	}
	ans2 += ans1;

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
