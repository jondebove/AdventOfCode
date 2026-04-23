#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	char line[256] = { 0 };
	char *s = fgets(line, sizeof(line), stdin);
	assert(s && !line[sizeof(line) - 1]);
	s = strchr(line, 'S');
	assert(s);

	long row[sizeof(line) - 1] = { 0 };
	row[s - line] = 1;

	int i;
	while (fgets(line, sizeof(line), stdin)) {
		for (i = 0; line[i]; i++) {
			if (line[i] == '^' && row[i]) {
				row[i - 1] += row[i];
				row[i + 1] += row[i];
				row[i] = 0;
				ans1++;
			}
		}
	}
	for (i = 0; line[i]; i++) ans2 += row[i];

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
