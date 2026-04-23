#include <assert.h>
#include <stdio.h>
#include <string.h>

static long f(char const *s, long n)
{
	assert(n > 0 && n <= 12);

	char b[12] = { 0 };
	int i;
	for (; s[n - 1]; s++) {
		for (i = 0; i < n; i++) {
			if (s[i] > b[i]) {
				memcpy(b + i, s + i, n - i);
				break;
			}
		}
	}

	long ans = 0;
	for (i = 0; i < n; i++) ans = ans * 10 + (b[i] - '0');
	return ans;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	char buf[2048];
	while (scanf("%2047s", buf) == 1) {
		ans1 += f(buf, 2);
		ans2 += f(buf, 12);
	}

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
