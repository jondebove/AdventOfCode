#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEVS_CAP 1024

struct dev {
	char name[4];
	struct dev *outs[32];
	int nout;
	int seen;
	long ans;
};

static struct dev *dev_search(struct dev *devs, int *n, char const *s)
{
	assert(strlen(s) < sizeof(devs->name));
	int i;
	for (i = *n; i--; ) {
		if (strcmp(devs[i].name, s) == 0) {
			return &devs[i];
		}
	}

	assert(*n < DEVS_CAP);
	strcpy(devs[*n].name, s);
	*n += 1;
	return &devs[*n - 1];
}

static long dfs(struct dev *src, struct dev *dst)
{
	if (src == dst) return 1;
	if (src->ans < 0) {
		src->ans = 0;
		src->seen = 1;
		int i;
		for (i = src->nout; i--; ) {
			if (!src->outs[i]->seen) {
				src->ans += dfs(src->outs[i], dst);
			}
		}
		src->seen = 0;
	}
	return src->ans;
}

static long f(struct dev *devs, int n, char const *src, char const *dst)
{
	int i;
	for (i = n; i--; ) devs[i].ans = -1;
	return dfs(dev_search(devs, &n, src), dev_search(devs, &n, dst));
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	struct dev *devs = calloc(DEVS_CAP, sizeof(*devs));
	int ndevs = 0;

	/* Parse. */
	char line[1024];
	while (fgets(line, sizeof(line), stdin)) {
		char *s = strtok(line, ": ");
		struct dev *d = dev_search(devs, &ndevs, s);
		for (; s; s = strtok(NULL, " \n")) {
			d->outs[d->nout++] = dev_search(devs, &ndevs, s);
		}
	}

	/* Part 1. */
	ans1 = f(devs, ndevs, "you", "out");

	/* Part 2. */
	ans2 =  f(devs, ndevs, "svr", "fft") *
		f(devs, ndevs, "fft", "dac") *
		f(devs, ndevs, "dac", "out") +
		f(devs, ndevs, "svr", "dac") *
		f(devs, ndevs, "dac", "fft") *
		f(devs, ndevs, "fft", "out");

	free(devs);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
