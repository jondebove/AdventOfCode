#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define GRIDSIZE (1UL<<15)

long dfs(char const *grid, long nrow, long ncol, long pos, long dist, bool *seen)
{
	if (pos / ncol == nrow - 1) {
		return dist;
	}

	seen[pos] = true;

	long ans = 0;
	long const dir[4] = { 1, -1, ncol, -ncol };
	for (long i = 0; i < 4; i++) {
		long p = pos + dir[i];
		if (p >= 0 && p < ncol * nrow && !seen[p] &&
				grid[p] != '#' && grid[p] != '\n') {
			long ans1 = dfs(grid, nrow, ncol, p, dist + 1, seen);
			ans = ans > ans1 ? ans : ans1;
		}
	}

	seen[pos] = false;
	return ans;
}

int main(void)
{
	char grid[GRIDSIZE] = { 0 };
	bool seen[GRIDSIZE] = { 0 };
	size_t n = fread(grid, sizeof(grid[0]), GRIDSIZE, stdin);
	long ncol = strchr(grid, '\n') + 1 - (char *)grid;
	long nrow = n / ncol;

	printf("%ld\n", dfs(grid, nrow, ncol, 1, 0, seen));

	return 0;
}
