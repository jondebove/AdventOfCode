package main

import (
	"fmt"
	"io"
)

func main() {
	ans1 := 0
	ans2 := 0

	const big = int(^uint(0) >> 3)

	vals := [4]int{big, big, big, big}
	for i := 0; ; i = (i + 1) % 4 {
		_, err := fmt.Scan(&vals[i])
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}

		// part 1
		if vals[i] > vals[(i+3)%4] {
			ans1++
		}

		// part 2
		if vals[i] > vals[(i+1)%4] {
			ans2++
		}
	}

	fmt.Println(ans1, ans2)
}
