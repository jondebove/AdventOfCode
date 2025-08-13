package main

import (
	"bytes"
	"fmt"
	"io"
	"os"
)

func main() {
	ans1 := 0
	ans2 := 0

	// Read grid
	grid, err := io.ReadAll(os.Stdin)
	if err != nil {
		panic(err)
	}
	ncol := bytes.IndexByte(grid, '\n') + 1
	//nrow := len(grid) / ncol

	// part 1
	for i, b := range grid {
		if b != '\n' &&
			greater(grid, i-1, b) && greater(grid, i+1, b) &&
			greater(grid, i-ncol, b) && greater(grid, i+ncol, b) {
			ans1 += (int(b) - '0') + 1
		}
	}

	// part 2
	p2 := [4]int{0}
	for i := range grid {
		p2[3] = flood(grid, ncol, i)
		for j := 3; j > 0; j-- {
			if p2[j] <= p2[j-1] {
				break
			} else {
				p2[j], p2[j-1] = p2[j-1], p2[j]
			}
		}
	}
	ans2 = p2[0] * p2[1] * p2[2]

	fmt.Println(ans1, ans2)
}

func valid(grid []byte, pos int) bool {
	return pos >= 0 && pos < len(grid) && grid[pos] != '\n'
}

func greater(grid []byte, pos int, val byte) bool {
	return !valid(grid, pos) || grid[pos] > val
}

func flood(grid []byte, ncol int, pos int) int {
	if !valid(grid, pos) || grid[pos] == '9' {
		return 0
	}
	grid[pos] = '9'
	return flood(grid, ncol, pos-1) +
		flood(grid, ncol, pos+1) +
		flood(grid, ncol, pos-ncol) +
		flood(grid, ncol, pos+ncol) + 1
}
