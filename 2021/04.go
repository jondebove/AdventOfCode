package main

import (
	"fmt"
	"io"
)

func main() {
	ans1 := 0
	ans2 := 0

	// Read numbers
	nums := []int{}
	for {
		n := 0
		c := '\000'
		_, err := fmt.Scanf("%d%c", &n, &c)
		if err != nil {
			panic(err)
		}
		nums = append(nums, n)
		if c != ',' {
			break
		}
	}

	// Read boards
	boards := []*[5][5]int{}
	for {
		b := [5][5]int{}
		for i := 0; i < 5; i++ {
			for j := 0; j < 5; j++ {
				_, err := fmt.Scan(&b[i][j])
				if err == io.EOF {
					goto play
				} else if err != nil {
					panic(err)
				}
			}
		}
		boards = append(boards, &b)
	}

play:
	seen := [100]bool{}
	for _, n := range nums {
		seen[n] = true
		l := 0
		for _, b := range boards {
			if boardWin(b, &seen) {
				ans2 = 0
				for _, r := range b {
					for _, c := range r {
						if !seen[c] {
							ans2 += c
						}
					}
				}
				ans2 *= n
				if ans1 == 0 {
					ans1 = ans2
				}
			} else { // We only keep boards with no win
				boards[l] = b
				l++
			}
		}
		boards = boards[:l]
	}

	fmt.Println(ans1, ans2)
}

func boardWin(b *[5][5]int, seen *[100]bool) bool {
	for i := 0; i < 5; i++ {
		rowsum := 0
		colsum := 0
		for j := 0; j < 5; j++ {
			if seen[b[i][j]] {
				rowsum++
			}
			if seen[b[j][i]] {
				colsum++
			}
		}
		if rowsum == 5 || colsum == 5 {
			return true
		}
	}
	return false
}
