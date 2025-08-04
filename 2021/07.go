package main

import (
	"fmt"
	"io"
)

const huge = int(^uint(0) >> 1)

func main() {
	ans1 := huge
	ans2 := huge

	crabs := []int{}
	cmin := huge
	cmax := -huge
	for {
		pos := 0
		sep := 0
		_, err := fmt.Scanf("%d%c", &pos, &sep)
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}
		crabs = append(crabs, pos)
		if pos < cmin {
			cmin = pos
		}
		if pos > cmax {
			cmax = pos
		}
	}

	for target := cmin; target <= cmax; target++ {
		a1 := 0
		a2 := 0
		for _, c := range crabs {
			dist := c - target
			if dist < 0 {
				dist = -dist
			}
			a1 += dist                  // part 1
			a2 += dist * (dist + 1) / 2 // part 2
		}
		if a1 < ans1 {
			ans1 = a1
		}
		if a2 < ans2 {
			ans2 = a2
		}
	}

	fmt.Println(ans1, ans2)
}
