package main

import (
	"fmt"
	"io"
	"slices"
)

func main() {
	ans1 := 0
	ans2 := 0

	/* segments sorted by number of appearence */
	encoding := map[[7]byte]int{
		[7]byte{0, 4, 6, 7, 8, 8, 9}: 0,
		[7]byte{0, 0, 0, 0, 0, 8, 9}: 1,
		[7]byte{0, 0, 4, 7, 7, 8, 8}: 2,
		[7]byte{0, 0, 7, 7, 8, 8, 9}: 3,
		[7]byte{0, 0, 0, 6, 7, 8, 9}: 4,
		[7]byte{0, 0, 6, 7, 7, 8, 9}: 5,
		[7]byte{0, 4, 6, 7, 7, 8, 9}: 6,
		[7]byte{0, 0, 0, 0, 8, 8, 9}: 7,
		[7]byte{4, 6, 7, 7, 8, 8, 9}: 8,
		[7]byte{0, 6, 7, 7, 8, 8, 9}: 9,
	}
	for {
		sigs := [10]string{}
		outs := [4]string{}
		_, err := fmt.Scanf("%s %s %s %s %s %s %s %s %s %s | %s %s %s %s\n",
			&sigs[0], &sigs[1], &sigs[2], &sigs[3], &sigs[4],
			&sigs[5], &sigs[6], &sigs[7], &sigs[8], &sigs[9],
			&outs[0], &outs[1], &outs[2], &outs[3])
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}

		// part 1
		for _, s := range outs {
			switch len(s) {
			case 2:
				fallthrough
			case 3:
				fallthrough
			case 4:
				fallthrough
			case 7:
				ans1++
			}
		}

		// part 2
		hist := [7]byte{0}
		for _, s := range sigs {
			for _, b := range s {
				hist[b-'a']++
			}
		}
		num := 0
		for _, s := range outs {
			enc := [7]byte{}
			for i, b := range s {
				enc[i] = hist[b-'a']
			}
			slices.Sort(enc[:])
			num = num*10 + encoding[enc]
		}
		ans2 += num
	}

	fmt.Println(ans1, ans2)
}
