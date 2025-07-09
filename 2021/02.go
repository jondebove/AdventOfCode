package main

import (
	"fmt"
	"io"
)

func main() {
	ans1 := 0
	ans2 := 0

	x := 0
	for {
		dir := ""
		cnt := 0
		_, err := fmt.Scan(&dir, &cnt)
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}

		switch dir {
		case "forward":
			x += cnt
			ans2 += cnt * ans1
		case "up":
			ans1 -= cnt
		case "down":
			ans1 += cnt
		default:
			panic(dir)
		}
	}
	ans1 *= x
	ans2 *= x

	fmt.Println(ans1, ans2)
}
