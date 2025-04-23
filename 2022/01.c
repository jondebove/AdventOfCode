#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

void isort(long *arr, long len)
{
	long i = len - 1;
	//for (i = 1; i < len; i++)
	{
		long j;
		long x = arr[i];
		for (j = i; j > 0 && arr[j - 1] < x; j--) {
			arr[j] = arr[j - 1];
		}
		arr[j] = x;
	}
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct buffer b;
	buffer_create(&b);

	long cal[4] = { 0 };
	while (!buffer_getdelim(&b, '\n', stdin)) {
		char *end;
		long n = strtol(b.str, &end, 10);
		if (b.str == end) {
			isort(cal, 4);
			cal[3] = 0;
		} else {
			cal[3] += n;
		}
	}
	ans1 = cal[0];
	ans2 = cal[0] + cal[1] + cal[2];

	buffer_destroy(&b);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
