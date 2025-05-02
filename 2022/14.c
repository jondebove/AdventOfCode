#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

struct vec2 {
	long x;
	long y;
};

static struct vec2 parse_vec2(char const *s)
{
	struct vec2 v;
	int nscn = sscanf(s, "%ld,%ld", &v.x, &v.y);
	assert(nscn == 2);
	return v;
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

#define N 1024
	unsigned char data[N][N] = { 0 };

	struct buffer b;
	buffer_create(&b);

	/* make grid */
	long ymax = 0;
	while (!buffer_getdelim(&b, '\n', stdin)) {
		b.str[--b.len] = '\0'; /* chomp */
		char *s = strtok(b.str, " ->");
		struct vec2 v1 = parse_vec2(s);
		for (;;) {
			ymax = MAX(ymax, v1.y);
			if ((s = strtok(NULL, " ->")) == NULL) {
				data[v1.y][v1.x] = 0x03;
				break;
			}
			struct vec2 v2 = parse_vec2(s);
			struct vec2 dir = {
				.x = (v2.x > v1.x) - (v2.x < v1.x),
				.y = (v2.y > v1.y) - (v2.y < v1.y),
			};
			for (; v1.x != v2.x || v1.y != v2.y;
					v1.x += dir.x, v1.y += dir.y) {
				data[v1.y][v1.x] = 0x03;
			}
		}
	}
	struct vec2 const s = { 500, 0 };

	buffer_destroy(&b);

	/* part 1 */
	for (struct vec2 v = s; v.y < ymax; ) {
		if (!(data[v.y + 1][v.x] & 0x01)) {
			v.y++;
		} else if (!(data[v.y + 1][v.x - 1] & 0x01)) {
			v.y++;
			v.x--;
		} else if (!(data[v.y + 1][v.x + 1] & 0x01)) {
			v.y++;
			v.x++;
		} else {
			data[v.y][v.x] |= 0x01;
			ans1++;
			v = s;
		}
	}

	/* part 2 */
	for (struct vec2 v = s;;) {
		if (v.y > ymax) {
			data[v.y][v.x] |= 0x02;
			ans2++;
			v = s;
		} else if (!(data[v.y + 1][v.x] & 0x02)) {
			v.y++;
		} else if (!(data[v.y + 1][v.x - 1] & 0x02)) {
			v.y++;
			v.x--;
		} else if (!(data[v.y + 1][v.x + 1] & 0x02)) {
			v.y++;
			v.x++;
		} else {
			data[v.y][v.x] |= 0x02;
			ans2++;
			if (v.y == 0) {
				break;
			}
			v = s;
		}
	}

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
