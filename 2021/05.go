package main

import (
	"fmt"
	"io"
)

func main() {
	ans1 := 0
	ans2 := 0

	grid := [1000][1000][2]int{}
	for {
		var x, y, dx, dy, n int
		_, err := fmt.Scanf("%d,%d -> %d,%d", &x, &y, &dx, &dy)
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}
		dx -= x
		dy -= y
		if dx > 0 {
			n = 1 + dx
		} else if dx < 0 {
			n = 1 - dx
		} else if dy > 0 {
			n = 1 + dy
		} else if dy < 0 {
			n = 1 - dy
		} else {
			n = 1
		}
		if dx > 0 {
			dx = 1
		} else if dx < 0 {
			dx = -1
		} else {
			dx = 0
		}
		if dy > 0 {
			dy = 1
		} else if dy < 0 {
			dy = -1
		} else {
			dy = 0
		}

		for ; n > 0; n-- {
			// part 1
			if dx == 0 || dy == 0 {
				grid[y][x][0]++
				if grid[y][x][0] == 2 {
					ans1++
				}
			}
			// part 2
			grid[y][x][1]++
			if grid[y][x][1] == 2 {
				ans2++
			}
			x += dx
			y += dy
		}
	}

	fmt.Println(ans1, ans2)
}
