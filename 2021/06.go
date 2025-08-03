package main

import (
	"fmt"
	"io"
)

func main() {
	ans1 := 0
	ans2 := 0

	hist := [9]int{}
	for {
		bin := 0
		sep := 0
		_, err := fmt.Scanf("%d%c", &bin, &sep)
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}
		hist[bin]++
	}

	ans1 = f(&hist, 80)
	ans2 = f(&hist, 256-80)

	fmt.Println(ans1, ans2)
}

func f(hist *[9]int, step int) int {
	for ; step != 0; step-- {
		newt := hist[0]
		for i := 0; i < 8; i++ {
			hist[i] = hist[i+1]
		}
		hist[8] = newt
		hist[6] += newt
	}
	ans := 0
	for i := 0; i < 9; i++ {
		ans += hist[i]
	}
	return ans
}
