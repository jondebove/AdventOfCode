#include <assert.h>
#include <stdio.h>

#define N 22

enum type {
	AIR,
	LAVA,
	FILL,
};

static long surface(enum type world[N][N][N], enum type out)
{
	long ans = 0;
	for (int i = 1; i < N-1; i++) {
		for (int j = 1; j < N-1; j++) {
			for (int k = 1; k < N-1; k++) {
				if (world[i][j][k] != LAVA) {
					continue;
				}
				ans +=  (world[i-1][j][k] == out) +
					(world[i+1][j][k] == out) +
					(world[i][j-1][k] == out) +
					(world[i][j+1][k] == out) +
					(world[i][j][k-1] == out) +
					(world[i][j][k+1] == out);
			}
		}
	}
	return ans;
}

static void floodfill(enum type world[N][N][N], int i, int j, int k)
{
	if (world[i][j][k] != AIR) {
		return;
	}
	world[i][j][k] = FILL;
	if (i > 0)   floodfill(world, i-1, j, k);
	if (i < N-1) floodfill(world, i+1, j, k);
	if (j > 0)   floodfill(world, i, j-1, k);
	if (j < N-1) floodfill(world, i, j+1, k);
	if (k > 0)   floodfill(world, i, j, k-1);
	if (k < N-1) floodfill(world, i, j, k+1);
}

int main(void)
{
	long ans1 = 0;
	long ans2 = 0;

	/* read input */
	enum type world[N][N][N] = { 0 };  /* AIR */
	{
		int x, y, z;
		while (scanf(" %d,%d,%d ", &x, &y, &z) == 3) {
			/* ensure there is no lava on boundaries */
			assert(x >= 0 && x < N-2);
			assert(y >= 0 && y < N-2);
			assert(z >= 0 && z < N-2);
			world[x+1][y+1][z+1] = LAVA;
		}
	}

	/* part 1 */
	ans1 = surface(world, AIR);

	/* part 2 */
	floodfill(world, 0, 0, 0);
	ans2 = surface(world, FILL);

	printf("%ld %ld\n", ans1, ans2);

	return 0;
}
