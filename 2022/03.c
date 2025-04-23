#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

static void enter(unsigned int *base, char *s, long n, unsigned int mask)
{
	for (char *end = s + n; s != end; s++) {
		base[(unsigned char)(*s)] |= mask;
	}
}

static char *find(unsigned int *base, char *s, long n, unsigned int mask)
{
	for (char *end = s + n; s != end; s++) {
		if ((base[(unsigned char)(*s)] & mask) == mask) {
			return s;
		}
	}
	return NULL;
}

static void delete(unsigned int *base, unsigned int mask)
{
	for (int i = 'A'; i <= 'z'; i++) {
		base[i] &= ~mask;
	}
}

static int priority(int c) {
	return c - (islower(C(c)) ? 'a' - 1 : 'A' - 27);
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

	int mod = 0;
	unsigned int base[256];

	while (!buffer_getdelim(&b, '\n', stdin)) {
		/* part 1 */
		delete(base, 0x1);
		long len = (b.len - 1) / 2;
		enter(base, b.str, len, 0x1);
		ans1 += priority(*find(base, b.str + len, len, 0x1));

		/* part 2 */
		switch (mod) {
			case 0:
				delete(base, 0x2|0x4);
				enter(base, b.str, b.len-1, 0x2);
				mod = 1;
				break;
			case 1:
				enter(base, b.str, b.len-1, 0x4);
				mod = 2;
				break;
			case 2:
				ans2 += priority(*find(base, b.str, b.len-1, 0x2|0x4));
				mod = 0;
				break;
			default:
				assert(0);
		}
	}

	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
