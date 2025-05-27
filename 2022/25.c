#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "utils.h"

long parse(char const *s)
{
	long ans = 0;
	for (;;) {
		switch (*s++) {
			case '=': ans = ans * 5 - 2; break;
			case '-': ans = ans * 5 - 1; break;
			case '0': ans = ans * 5 + 0; break;
			case '1': ans = ans * 5 + 1; break;
			case '2': ans = ans * 5 + 2; break;
			default: return ans;
		}
	}
	return ans;
}

char const *format(long n)
{
	static char const digits[] = "012=-";
	static char s[32];
	char *ans = &s[sizeof(s) - 1];
	*ans = '\0';
	while (n != 0) {
		assert(ans != s);
		long r = n % 5;
		n = n / 5 + (r > 2);
		*--ans = digits[r];
	}
	return ans;
}

int main(void)
{
	char const *ans1 = NULL;
	char const *ans2 = "";  /* unused */

	struct buffer b;
	buffer_create(&b);

	long n = 0;
	while (!buffer_getdelim(&b, '\n', stdin)) {
		n += parse(b.str);
	}
	ans1 = format(n);

	buffer_destroy(&b);

	printf("%s %s\n", ans1, ans2);

	return 0;
}
