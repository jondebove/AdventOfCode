package main

import (
	"fmt"
)

func main() {
	ans1 := 0
	ans2 := uint64(0)

	// Read
	pos0 := 0
	pos1 := 0
	fmt.Scanf("Player 1 starting position: %d", &pos0)
	fmt.Scanf("Player 2 starting position: %d", &pos1)

	// Part 1
	p := [2]player{
		player{pos: pos0},
		player{pos: pos1},
	}
	dice := 1
	for i := 0; ; i = (i + 1) & 1 {
		for j := 3; j != 0; j-- {
			p[i].pos = (p[i].pos-1+dice)%10 + 1
			dice = (dice % 100) + 1
		}
		ans1 += 3
		p[i].score += p[i].pos
		if p[i].score >= 1000 {
			ans1 *= p[(i+1)&1].score
			break
		}
	}

	// Part 2
	p[0].pos = pos0
	p[0].score = 0
	p[1].pos = pos1
	p[1].score = 0
	r := [2]uint64{}

	dfs(p, 0, 1, &r)
	ans2 = max(r[0], r[1])

	fmt.Println(ans1, ans2)
}

type player struct {
	pos    int
	score  int
}

func dfs(p [2]player, i int, w uint64, ans *[2]uint64) {
	// 3^3=27 universes            sum count
	// -------------------------------------
	// 111                           3     1
	// 112 121 211                   4     3
	// 113 122 131 212 221 311       5     6
	// 123 132 213 231 312 321 222   6     7
	// 133 223 232 313 322 331       7     6
	// 332 323 233                   8     3
	// 333                           9     1
	// -------------------------------------
	// So 27 universes turn into 7 universes with weights

	ii := (i + 1) & 1
	sums := [7]int{3, 4, 5, 6, 7, 8, 9}
	counts := [7]uint64{1, 3, 6, 7, 6, 3, 1}
	for j := 0; j < 7; j++ {
		p := p
		p[i].pos += sums[j]
		if p[i].pos > 10 {
			p[i].pos -= 10
		}
		p[i].score += p[i].pos
		if w := w * counts[j]; p[i].score >= 21 {
			ans[i] += w
		} else {
			dfs(p, ii, w, ans)
		}
	}
}
