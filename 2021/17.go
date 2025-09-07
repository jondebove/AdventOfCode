package main

import (
	"fmt"
)

type vec2 struct{ x, y int }

func main() {
	ans1 := 0
	ans2 := 0

	var tmin, tmax vec2 // Target
	_, err := fmt.Scanf("target area: x=%d..%d, y=%d..%d",
		&tmin.x, &tmax.x, &tmin.y, &tmax.y)
	if err != nil {
		panic(err)
	}

	// We choose to work in x >= 0 half space
	if (tmin.x > 0) != (tmax.x > 0) {
		panic("problem illformed: ans1=inf")
	}
	if tmin.x < 0 {
		tmin.x, tmax.x = -tmax.x, -tmin.x
	}

	// Inf bound for v.x to reach tmin.x
	vxmin := isqrt(tmin.x*2) - 1

	// part 1
	var v vec2
	for v.y = 200; v.y >= tmin.y; v.y-- {
		for v.x = vxmin; v.x <= tmax.x; v.x++ {
			if reach(v, tmin, tmax) {
				goto out
			}
		}
	}
out:
	ans1 = v.y * (v.y + 1) / 2

	// part 2
	for ; v.y >= tmin.y; v.y-- {
		for v.x = vxmin; v.x <= tmax.x; v.x++ {
			if reach(v, tmin, tmax) {
				ans2++
			}
		}
	}

	fmt.Println(ans1, ans2)
}

// Heron's method
func isqrt(x int) int {
	if x <= 1 {
		return x
	}
	y := x / 2
	for {
		y1 := (y + x/y) / 2
		if y1 >= y {
			return y
		}
		y = y1
	}
}

func reach(v, tmin, tmax vec2) bool {
	var r vec2
	for {
		if r.x >= tmin.x && r.x <= tmax.x &&
			r.y >= tmin.y && r.y <= tmax.y {
			return true
		}
		if r.x < tmin.x && v.x <= 0 {
			return false
		}
		if r.x > tmax.x && v.x >= 0 {
			return false
		}
		if r.y < tmin.y && v.y <= 0 {
			return false
		}

		r.x += v.x
		r.y += v.y
		if v.x > 0 {
			v.x -= 1
		} else if v.x < 0 {
			v.x += 1
		}
		v.y -= 1
	}
}
