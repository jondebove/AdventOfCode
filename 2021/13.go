package main

import (
	"fmt"
)

type pos struct {
	x int
	y int
}

func main() {
	ans1 := 0
	ans2 := 0

	grid := make(map[pos]struct{}, 1000)
	xmax := 0
	ymax := 0

	for {
		var p pos
		n, _ := fmt.Scanf("%d,%d\n", &p.x, &p.y)
		if n != 2 {
			break
		}
		if p.x > xmax {
			xmax = p.x
		}
		if p.y > ymax {
			ymax = p.y
		}
		grid[p] = struct{}{}
	}

	for i := 0; ; i++ {
		d := '\000'
		v := 0
		n, _ := fmt.Scanf("fold along %c=%d\n", &d, &v)
		if n != 2 {
			break
		}
		if d == 'x' {
			for p := range grid {
				if p.x >= v {
					delete(grid, p)
					p.x = 2*v - p.x
					grid[p] = struct{}{}
				}
			}
			xmax = v - 1
		} else if d == 'y' {
			for p := range grid {
				if p.y >= v {
					delete(grid, p)
					p.y = 2*v - p.y
					grid[p] = struct{}{}
				}
			}
			ymax = v - 1
		}

		// part 1
		if i == 0 {
			ans1 = len(grid)
		}
	}

	// part 2
	buf := make([]byte, (xmax+2)*(ymax+1))
	var p pos
	for p.y = 0; p.y <= ymax; p.y++ {
		yoff := p.y * (xmax + 2)
		for p.x = 0; p.x <= xmax; p.x++ {
			if _, ok := grid[p]; ok {
				buf[p.x+yoff] = '#'
			} else {
				buf[p.x+yoff] = '.'
			}
		}
		buf[p.x+yoff] = '\n'
	}
	fmt.Print(string(buf))

	fmt.Println(ans1, ans2)
}
