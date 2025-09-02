package main

import (
	"fmt"
	"io"
	"os"
)

type reader struct {
	buf []byte
	pos int
	decoding [256]byte
}

func newReader(buf []byte) *reader {
	r := reader{ buf: buf }
	encoding := []byte("0123456789ABCDEF")
	for i, b := range encoding {
		r.decoding[b] = byte(i)
	}
	return &r
}

func (r *reader) read(n int) uint64 {
	if n > 60 {
		panic("n > 60")
	}

	ans := uint64(0)
	j := (r.pos + n) / 4
	for i := r.pos / 4; i <= j; i++ {
		ans <<= 4
		ans |= uint64(r.decoding[r.buf[i]]) & 0xF
	}
	r.pos += n
	return (ans >> (4 - r.pos%4)) & ((uint64(1) << n) - 1)
}

func dfs(r *reader) (nread, ans1, ans2 uint64) {
	nread = uint64(6)
	ans1 = r.read(3)
	ptype := r.read(3)

	values := make([]uint64, 0, 4)
	if ptype != 4 {
		switch r.read(1) {
		case 0:
			nread += 16
			for n := r.read(15); n != 0; {
				nr, a1, a2 := dfs(r)
				nread += nr
				ans1 += a1
				values = append(values, a2)
				if nr > n { panic("nr > n") }
				n -= nr
			}
		case 1:
			nread += 12
			for n := r.read(11); n != 0; n-- {
				nr, a1, a2 := dfs(r)
				nread += nr
				ans1 += a1
				values = append(values, a2)
			}
		}
	}
	switch ptype {
	case 0:
		ans2 = 0
		for _, v := range values {
			ans2 += v
		}
	case 1:
		ans2 = 1
		for _, v := range values {
			ans2 *= v
		}
	case 2:
		ans2 = ^uint64(0)
		for _, v := range values {
			if ans2 > v {
				ans2 = v
			}
		}
	case 3:
		ans2 = 0
		for _, v := range values {
			if ans2 < v {
				ans2 = v
			}
		}
	case 4 :
		cont := r.read(1)
		ans2 = r.read(4)
		nread += 5
		for cont == 1 {
			ans2 <<= 4
			cont = r.read(1)
			ans2 |= r.read(4)
			nread += 5
		}
	case 5:
		if values[0] > values[1] {
			ans2 = 1
		} else {
			ans2 = 0
		}
	case 6:
		if values[0] < values[1] {
			ans2 = 1
		} else {
			ans2 = 0
		}
	case 7:
		if values[0] == values[1] {
			ans2 = 1
		} else {
			ans2 = 0
		}
	}
	return
}

func main() {
	data, err := io.ReadAll(os.Stdin)
	if err != nil {
		panic(err)
	}

	_, ans1, ans2 := dfs(newReader(data))

	fmt.Println(ans1, ans2)
}
