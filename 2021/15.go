package main

import (
	"bytes"
	"container/heap"
	"fmt"
	"io"
	"math"
	"os"
)

func main() {
	ans1 := 0
	ans2 := 0

	// Initialize grid
	grid, err := io.ReadAll(os.Stdin)
	if err != nil {
		panic(err)
	}
	ncol := bytes.IndexByte(grid, '\n')
	nrow := len(grid) / (ncol + 1)

	// Make graph
	g := make(graph, nrow*ncol)
	s := g.getVertex(0)
	e := g.getVertex(ktype(len(grid) - 2))
	for k, b := range grid {
		if b == '\n' {
			continue
		}
		for _, d := range [4]int{-ncol - 1, -1, 1, ncol + 1} {
			l := k + d
			if l < 0 || l >= len(grid) || grid[l] == '\n' {
				continue
			}
			g.addEdge(ktype(k), ktype(l), float64(grid[l]-'0'))
		}
	}

	if v := g.dijkstra(s, e); v != nil {
		ans1 = int(v.dist)
	}

	fmt.Println(ans1, ans2)
}

type ktype int

type vertex struct {
	key   ktype
	dist  float64
	prev  *vertex
	edges []edge
	index int
}

type edge struct {
	next   *vertex
	weight float64
}

type graph map[ktype]*vertex

func (g graph) getVertex(k ktype) *vertex {
	v, ok := g[k]
	if !ok {
		v = &vertex{key: k, edges: make([]edge, 0, 4)}
		g[k] = v
	}
	return v
}

func (g graph) addEdge(src, dst ktype, weight float64) {
	u := g.getVertex(src)
	v := g.getVertex(dst)
	u.edges = append(u.edges, edge{v, weight})
}

func (g graph) dijkstra(s *vertex, e *vertex) *vertex {
	pq := make(PriorityQueue, len(g))
	i := 0
	for _, v := range g {
		v.dist = math.Inf(1)
		v.prev = nil
		if v == s {
			v.dist = 0
			v.prev = s
		}
		v.index = i
		pq[i] = v
		i++
	}
	heap.Init(&pq)

	for pq.Len() > 0 {
		u := heap.Pop(&pq).(*vertex)
		if u == e {
			return u
		}
		for _, e := range u.edges {
			v := e.next
			d := u.dist + e.weight
			if d < v.dist {
				v.prev = u
				v.dist = d
				heap.Fix(&pq, v.index)
			}
		}
	}
	return nil
}

// Copyright 2012 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
type PriorityQueue []*vertex

func (pq PriorityQueue) Len() int { return len(pq) }

func (pq PriorityQueue) Less(i, j int) bool {
	return pq[i].dist < pq[j].dist
}

func (pq PriorityQueue) Swap(i, j int) {
	pq[i], pq[j] = pq[j], pq[i]
	pq[i].index = i
	pq[j].index = j
}

func (pq *PriorityQueue) Push(x any) {
	n := len(*pq)
	v := x.(*vertex)
	v.index = n
	*pq = append(*pq, v)
}

func (pq *PriorityQueue) Pop() any {
	old := *pq
	n := len(old)
	v := old[n-1]
	old[n-1] = nil // don't stop the GC from reclaiming the v eventually
	v.index = -1   // for safety
	*pq = old[0 : n-1]
	return v
}
