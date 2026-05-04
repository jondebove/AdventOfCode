#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRY_MAX 100
#define GEN_MAX 1000
#define POP_LEN 100
#define ELIT_LEN 2
#define REC_FREQ 1000
#define MUT_FREQ 4

#define CHR_CAP 16
#define SCO_CAP 16
#define ANS_MAX 100000

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

struct item {
	int scores[2][SCO_CAP];
	int buttons[CHR_CAP][SCO_CAP];
	int nbuttons;
	int nscores;
};

struct ind {
	int chr[CHR_CAP];
	float fit;
	int ans;
};

static void ind_init(struct ind *ind, int chrlen, int chrmax)
{
	while (chrlen--) ind->chr[chrlen] = rand() % (chrmax + 1);
	ind->fit = 0;
	ind->ans = 0;
}

static int ind_eval1(struct ind *ind, int chrlen, struct item const *item)
{
	assert(chrlen == item->nbuttons);
	if (ind->fit) return ind->fit;

	int i, j;
	int fit1 = 0;
	int ok = 1;
	for (i = item->nscores; i--; ) {
		int t = item->scores[0][i];
		for (j = item->nbuttons; j--; ) {
			t = item->buttons[j][i] && ind->chr[j] ? !t : t;
		}
		if (t == 0) fit1++;
		else ok = 0;
	}

	int fit2 = 0;
	for (j = chrlen; j--; ) {
		fit2 += ind->chr[j];
	}
	ind->fit = fit1 + 1.0 / (1 + fit2);
	ind->ans = ok ? fit2 : ANS_MAX;
	return ind->fit;
}

static int ind_eval2(struct ind *ind, int chrlen, struct item const *item)
{
	assert(chrlen == item->nbuttons);
	if (ind->fit) return ind->fit;

	int i, j;
	int fit1 = 0;
	for (i = item->nscores; i--; ) {
		int t = item->scores[1][i];
		for (j = item->nbuttons; j--; ) {
			t -= item->buttons[j][i] * ind->chr[j];
		}
		fit1 += abs(t);
	}

	int fit2 = 0;
	for (j = chrlen; j--; ) {
		fit2 += ind->chr[j];
	}
	ind->ans = fit1 == 0 ? fit2 : ANS_MAX;
	ind->fit = 1.0f / (10 + fit1) + 1.0f / ind->ans;
	return ind->fit;
}

static int ind_recombine(struct ind *a, struct ind *b, int chrlen)
{
	int p = rand();
	if (p < RAND_MAX / REC_FREQ) return 0;

	p = p % (chrlen - 1) + 1; /* pivot */
	memcpy(a->chr, b->chr, sizeof(*a->chr) * p);
	memcpy(b->chr + p, a->chr + p, sizeof(*a->chr) * (chrlen - p));
	a->fit = 0;
	b->fit = 0;
	return 1;
}

static int ind_mutate(struct ind *ind, int chrlen, int chrmax)
{
	int ans = 0;
	while (chrlen--) {
		int p = rand() % (2 * MUT_FREQ);
		if (p == 0) {
			ind->chr[chrlen] = ind->chr[chrlen] < chrmax ?
				ind->chr[chrlen] + 1 : ind->chr[chrlen];
			ans++;
		} else if (p == 1) {
			ind->chr[chrlen] = ind->chr[chrlen] > 0 ?
				ind->chr[chrlen] - 1 : ind->chr[chrlen];
			ans++;
		}
	}
	if (ans) ind->fit = 0;
	return ans;
}

static int ind_cmp(void const *a, void const *b)
{
	float xa = ((struct ind const *)a)->fit;
	float xb = ((struct ind const *)b)->fit;
	return xa > xb ? -1 : xa < xb;
}

struct pop {
	struct ind *inds;
	int chrlen;
	int chrmax;
	float fit;
};

static void pop_create(struct pop *p)
{
	memset(p, 0, sizeof(*p));
	p->inds = calloc(POP_LEN * 2, sizeof(*p->inds));
	assert(p->inds);
}

static void pop_destroy(struct pop *p)
{
	free(p->inds);
	memset(p, 0, sizeof(*p));
}

static void pop_init(struct pop *p, int chrlen, int chrmax)
{
	p->chrlen = chrlen;
	p->chrmax = chrmax;
	int i;
	for (i = 0; i < POP_LEN; i++) {
		ind_init(&p->inds[i], chrlen, chrmax);
	}
}

static int pop_eval(struct pop *p,
		int (*eval)(struct ind *ind, int chrlen, struct item const *item),
		struct item const *item)
{
	int i;
	p->fit = 0;
	for (i = 0; i < POP_LEN; i++) {
		p->fit += eval(&p->inds[i], p->chrlen, item);
	}
	return p->fit;
}

static void pop_select(struct pop *p)
{
	int i, j;

	/* Stochastic Universal Sampling. */
	float width = p->fit / (POP_LEN - ELIT_LEN);
	float start = ((rand() % (POP_LEN - ELIT_LEN)) * width) / (POP_LEN - ELIT_LEN);
	float cumfit = p->inds[j = 0].fit;
	for (i = POP_LEN + ELIT_LEN; i < POP_LEN * 2; i++) {
		while (cumfit < start) cumfit += p->inds[++j].fit;
		memcpy(&p->inds[i], &p->inds[j], sizeof(*p->inds));
		start += width;
	}

	/* Elitism. */
	if (ELIT_LEN > 0) {
		qsort(p->inds, POP_LEN, sizeof(*p->inds), ind_cmp);
		/* Unique. */
		for (i = 1, j = 1; i < ELIT_LEN; j++) {
			assert(j < POP_LEN);
			if (memcmp(p->inds[j].chr, p->inds[i - 1].chr, sizeof(*p->inds->chr) * p->chrlen)) {
				memcpy(&p->inds[i++], &p->inds[j], sizeof(*p->inds));
			}
		}
		memcpy(p->inds + POP_LEN, p->inds, sizeof(*p->inds) * ELIT_LEN);
	}
}

static void pop_recombine(struct pop *p)
{
	int i;
	for (i = ELIT_LEN; i < POP_LEN; i += 2) {
		int j = rand() % POP_LEN + POP_LEN;
		int k = rand() % POP_LEN + POP_LEN;

		memcpy(&p->inds[i    ], &p->inds[j], sizeof(*p->inds));
		memcpy(&p->inds[i + 1], &p->inds[k], sizeof(*p->inds));
		ind_recombine(&p->inds[i], &p->inds[i + 1], p->chrlen);
	}
}

static void pop_mutate(struct pop *p)
{
	int i;
	for (i = ELIT_LEN; i < POP_LEN; i++) {
		ind_mutate(&p->inds[i], p->chrlen, p->chrmax);
	}
}

int main(int argc, char *argv[])
{
	long ans1 = 0;
	long ans2 = 0;

	struct pop p;
	pop_create(&p);

	int l = 0;
	char line[1024];
	while (fgets(line, sizeof(line), stdin)) {
		/* Parse. */
		int i;
		char *s, *end;
		struct item c = { 0 };
		for (s = strtok(line, " "); s; s = strtok(NULL, " ")) {
			if (*s == '[') {
				for (s++, i = 0; *s != ']'; s++, i++) {
					if (*s == '#') c.scores[0][i] = 1;
				}
				c.nscores = i;
			} else if (*s == '(') {
				for (end = s; *end != ')'; s = end) {
					c.buttons[c.nbuttons][
						strtol(s + 1, &end, 10)] = 1;
				}
				c.nbuttons++;
			} else if (*s == '{') {
				for (end = s, i = 0; *end != '}'; s = end, i++) {
					c.scores[1][i] = strtol(s + 1, &end, 10);
				}
				assert(i == c.nscores);
			}
		}
		l++;

		int ans, g;
		srand(argc < 2 ? 0 : atoi(argv[1]));

		/* Part 1. */
		ans = ANS_MAX;
		pop_init(&p, c.nbuttons, 1);
		for (g = 0; ; g++) {
			pop_eval(&p, ind_eval1, &c);
			for (i = 0; i < POP_LEN; i++) {
				ans = MIN(p.inds[i].ans, ans);
			}
			if (g == GEN_MAX) break;
			pop_select(&p);
			pop_recombine(&p);
			pop_mutate(&p);
		}
		printf("%d 1 %d\n", l, ans);
		ans1 += ans;

		/* Part 2. */
		int scoremax = 0;
		for (i = c.nscores; i--; ) {
			scoremax = MAX(c.scores[1][i], scoremax);
		}
		ans = ANS_MAX;
		int j;
		for (j = TRY_MAX; j--; ) {
			int ansj;
			for (ansj = ANS_MAX; ansj == ANS_MAX; ) {
				pop_init(&p, c.nbuttons, scoremax);
				for (g = 0; ; g++) {
					pop_eval(&p, ind_eval2, &c);
					for (i = 0; i < POP_LEN; i++) {
						ansj = MIN(p.inds[i].ans, ansj);
					}
					if (g == GEN_MAX) break;
					pop_select(&p);
					pop_recombine(&p);
					pop_mutate(&p);
				}
			}
			ans = MIN(ansj, ans);
		}
		printf("%d 2 %d\n", l, ans);
		fflush(stdout);
		ans2 += ans;
	}

	pop_destroy(&p);

	printf("%ld %ld\n", ans1, ans2);
	return 0;
}
