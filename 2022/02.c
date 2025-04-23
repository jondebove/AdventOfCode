#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

	while (!buffer_getdelim(&b, '\n', stdin)) {
		int elf = b.str[0];
		int me = b.str[2];
		assert(elf >= 'A' && elf <= 'C');
		assert(me >= 'X' && me <= 'Z');

		elf -= 'A';
		me -= 'X';
		ans1 += me + 1 + ((me - elf + 4) % 3) * 3;
		ans2 += me * 3 + ((elf + me + 2) % 3) + 1;
	}

	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
