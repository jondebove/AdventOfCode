package main

import (
	"fmt"
	"io"
)

type cube struct {
	xmin [3]int
	xmax [3]int
}

func union(a, b cube) []cube {
	var ans []cube
	if a.xmin > b.xmax || if b.xmin > a.xmax ||
		a.ymin > b.ymax || if b.ymin > a.ymax ||
		a.zmin > b.zmax || if b.zmin > a.zmax {
		return append(ans, a)
	}
}

func main() {
	ans1 := 0
	ans2 := 0

	// Read
	world := []cube{}
	for {
		op := ""
		xyz := [3][2]int{}
		_, err := fmt.Scanf("%s x=%d..%d,y=%d..%d,z=%d..%d",
		&op,
		&xyz[0][0], &xyz[0][1],
		&xyz[1][0], &xyz[1][1],
		&xyz[2][0], &xyz[2][1])
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}
		fmt.Println(op, xyz)

		opb := op == "on"

		for i := max(-50, xyz[0][0]); i <= min(50, xyz[0][1]); i++ {
			for j := max(-50, xyz[1][0]); j <= min(50, xyz[1][1]); j++ {
				for k := max(-50, xyz[2][0]); k <= min(50, xyz[2][1]); k++ {
					grid1[i+50][j+50][k+50] = opb
				}
			}
		}
	}

	for i := -50; i <= 50; i++ {
		for j := -50; j <= 50; j++ {
			for k := -50; k <= 50; k++ {
				if grid1[i+50][j+50][k+50] {
					ans1++
				}
			}
		}
	}

	fmt.Println(ans1, ans2)
}
