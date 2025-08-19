package main

import (
	"fmt"
	"io"
	"os"
)

const flashed = '\000'

func main() {
	ans1 := 0
	ans2 := 0

	grid, err := io.ReadAll(os.Stdin)
	if err != nil {
		panic(err)
	}
	if len(grid) != 10*11 {
		panic("grid size != 10x10")
	}

	for ans2 = 1; ans2 <= 100; ans2++ {
		ans1 += step(grid)
	}
	for step(grid) != 100 {
		ans2++
	}

	fmt.Println(ans1, ans2)
}

func flash(grid []byte, pos int, count *int) {
	if pos < 0 || pos >= len(grid) || grid[pos] == '\n' ||
		grid[pos] == flashed {
		return
	}
	grid[pos]++
	if grid[pos] > '9' {
		grid[pos] = flashed
		(*count)++
		flash(grid, pos-12, count)
		flash(grid, pos-11, count)
		flash(grid, pos-10, count)
		flash(grid, pos-1, count)
		flash(grid, pos+1, count)
		flash(grid, pos+10, count)
		flash(grid, pos+11, count)
		flash(grid, pos+12, count)
	}
}

func step(grid []byte) int {
	count := 0
	for i := 0; i < len(grid); i++ {
		if grid[i] == flashed {
			grid[i] = '0'
		}
	}
	for i := 0; i < len(grid); i++ {
		flash(grid, i, &count)
	}
	return count
}
