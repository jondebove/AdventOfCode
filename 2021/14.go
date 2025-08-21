package main

import (
	"fmt"
	"io"
)

func main() {
	// Read polymer
	poly := ""
	_, err := fmt.Scanln(&poly)
	if err != nil {
		panic(err)
	}

	// Skip empty line
	_, err = fmt.Scanln()
	if err != nil {
		panic(err)
	}

	// Read rules
	rules := make(map[[2]byte]byte, 200)
	for {
		b := [2]byte{}
		c := byte(0)
		_, err = fmt.Scanf("%c%c -> %c\n", &b[0], &b[1], &c)
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}
		rules[b] = c
	}

	ans1 := f(poly, rules, 10)
	ans2 := f(poly, rules, 40)

	fmt.Println(ans1, ans2)
}

func f(poly string, rules map[[2]byte]byte, nsteps int) int {
	// Histogram of pairs
	hist := make(map[[2]byte]int, 200)
	for i := 0; i < len(poly)-1; i++ {
		hist[[2]byte{poly[i], poly[i+1]}]++
	}

	// Steps
	for ; nsteps > 0; nsteps-- {
		h := make(map[[2]byte]int, 200)
		for k, v := range hist {
			if b, ok := rules[k]; ok {
				h[[2]byte{k[0], b}] += v
				h[[2]byte{b, k[1]}] += v
			} else {
				panic("not implemented")
			}
		}
		hist = h
	}

	// Count letters
	h := make(map[byte]int, 200)
	h[poly[0]] = 1
	h[poly[len(poly)-1]] = 1
	for k, v := range hist {
		h[k[0]] += v
		h[k[1]] += v
	}

	// Quantity of interest
	qmin := int(^uint(0) >> 1)
	qmax := 0
	for _, v := range h {
		qmin = min(qmin, v)
		qmax = max(qmax, v)
	}
	return (qmax - qmin) / 2
}
