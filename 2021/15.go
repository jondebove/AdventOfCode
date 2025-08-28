package main

import (
	"bytes"
	"fmt"
	"io"
	"os"
)

func main() {
	ans1 := 0
	ans2 := 0

	// Initialize grid
	data, err := io.ReadAll(os.Stdin)
	if err != nil {
		panic(err)
	}

	// part 1
	g := newGraph(data)
	ans1 = int(Distance(g, Dijkstra(g, 0), len(g.data)-2))

	// part 2
	data = make([]byte, g.nrow*5*(g.ncol*5+1))
	k := 0
	for i := 0; i < g.nrow*5; i++ {
		for j := 0; j < g.ncol*5; j++ {
			data[k] = byte(int(g.data[(i%g.nrow)*(g.ncol+1)+j%g.ncol])-'1'+i/g.nrow+j/g.ncol)%9 + '1'
			k++
		}
		data[k] = '\n'
		k++
	}
	g = newGraph(data)
	ans2 = int(Distance(g, Dijkstra(g, 0), len(g.data)-2))

	fmt.Println(ans1, ans2)
}

type graph struct {
	data []byte
	nrow int
	ncol int
}

func newGraph(data []byte) *graph {
	ncol := bytes.IndexByte(data, '\n')
	nrow := len(data) / (ncol + 1)
	return &graph{
		data: data,
		nrow: nrow,
		ncol: ncol,
	}
}

func (g *graph) Vertices() []int {
	vs := make([]int, g.nrow*g.ncol)
	j := 0
	for i, b := range g.data {
		if b != '\n' {
			vs[j] = i
			j++
		}
	}
	return vs
}

func (g *graph) Neighbors(v int) []int {
	ns := make([]int, 0, 4)
	for _, d := range [4]int{-g.ncol - 1, -1, 1, g.ncol + 1} {
		u := v + d
		if u >= 0 && u < len(g.data) && g.data[u] != '\n' {
			ns = append(ns, u)
		}
	}
	return ns
}

func (g *graph) Weight(u, v int) float64 {
	return float64(g.data[v] - '0')
}
