package main

import (
	"fmt"
	"io"
)

func main() {
	ans1 := 0
	ans2 := 0

	// Read
	instr := ""
	fmt.Scanln(&instr)

	line := ""
	fmt.Scanln(&line)

	g := newGrid(false)
	for i := 0; ; i++ {
		_, err := fmt.Scanln(&line)
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}

		for j := 0; j < len(line); j++ {
			g.set(i, j, line[j])
		}
	}

	// Part 1
	i := 0
	for ; i < 2; i++ {
		g = g.enhance(instr)
	}
	ans1 = len(g.data)
	//fmt.Println(g)

	// Part 2
	for ; i < 50; i++ {
		g = g.enhance(instr)
	}
	ans2 = len(g.data)

	fmt.Println(ans1, ans2)
}

type pos struct {
	i int
	j int
}

type grid struct {
	data map[pos]bool
	inf  pos
	sup  pos
	inv  bool
}

func newGrid(inv bool) grid {
	return grid{
		data: make(map[pos]bool),
		inv:  inv,
	}
}

func (g *grid) set(i, j int, v byte) {
	if (v == '#') != g.inv {
		g.data[pos{i, j}] = true
		g.inf.i = min(g.inf.i, i)
		g.sup.i = max(g.sup.i, i)
		g.inf.j = min(g.inf.j, j)
		g.sup.j = max(g.sup.j, j)
	}
}

func (g grid) get(i, j int) byte {
	if g.data[pos{i, j}] != g.inv {
		return '#'
	}
	return '.'
}

func (g grid) String() string {
	nr := g.sup.i - g.inf.i + 1
	nc := g.sup.j - g.inf.j + 1
	b := make([]byte, nr*(nc+1))
	k := 0
	for i := g.inf.i; i <= g.sup.i; i++ {
		for j := g.inf.j; j <= g.sup.j; j++ {
			b[k] = g.get(i, j)
			k++
		}
		b[k] = '\n'
		k++
	}
	return string(b)
}

func (g grid) enhance(instr string) grid {
	gn := newGrid((instr[0] == '#') != g.inv)
	for i := g.inf.i - 1; i <= g.sup.i+1; i++ {
		for j := g.inf.j - 1; j <= g.sup.j+1; j++ {
			n := 0
			for k, p := range []pos{
				pos{+1, 1}, pos{+1, 0}, pos{+1, -1},
				pos{+0, 1}, pos{+0, 0}, pos{+0, -1},
				pos{-1, 1}, pos{-1, 0}, pos{-1, -1}} {
				if g.get(i+p.i, j+p.j) == '#' {
					n |= 1 << k
				}
			}
			gn.set(i, j, instr[n])
		}
	}
	return gn
}
