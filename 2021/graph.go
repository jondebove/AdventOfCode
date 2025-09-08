package main

import (
	"container/heap"
	"math"
)

type Grapher[V any] interface {
	Vertices() []V
	Neighbors(v V) []V
	Weight(u, v V) float64
}

func Distance[V comparable](g Grapher[V], prev map[V]V, v V) float64 {
	ans := 0.0
	for u, ok := prev[v]; ok; u, ok = prev[v] {
		ans += g.Weight(u, v)
		v = u
	}
	return ans
}

func Bfs[V comparable](g Grapher[V], s V) map[V]V {
	q := []V{s}
	prev := map[V]V{s: s}
	for len(q) > 0 {
		u := q[0]
		q = q[1:]
		for _, v := range g.Neighbors(u) {
			if _, ok := prev[v]; !ok {
				q = append(q, v)
				prev[v] = u
			}
		}
	}
	delete(prev, s)
	return prev
}

func Dfs[V comparable](g Grapher[V], s V) map[V]V {
	q := []V{s}
	prev := map[V]V{s: s}
	for len(q) > 0 {
		u := q[len(q)-1]
		q = q[:len(q)-1]
		for _, v := range g.Neighbors(u) {
			if _, ok := prev[v]; !ok {
				prev[v] = u
				q = append(q, v)
			}
		}
	}
	delete(prev, s)
	return prev
}

func BellmanFord[V comparable](g Grapher[V], s V) map[V]V {
	vs := g.Vertices()
	dist := make(map[V]float64, len(vs))
	for _, v := range vs {
		dist[v] = math.Inf(1)
	}
	dist[s] = 0

	prev := make(map[V]V, len(vs))
	for n := len(vs) - 1; n > 0; n-- {
		for _, u := range vs {
			du := dist[u]
			for _, v := range g.Neighbors(u) {
				if d := du + g.Weight(u, v); d < dist[v] {
					dist[v] = d
					prev[v] = u
				}
			}
		}
	}
	return prev
}

func Dijkstra[V comparable](g Grapher[V], s V) map[V]V {
	vs := g.Vertices()
	items := make(map[V]*PriorityItem, len(vs))
	pq := make(PriorityQueue, len(vs))
	for i, v := range vs {
		d := math.Inf(1)
		if v == s {
			d = 0
		}
		item := PriorityItem{
			value:    v,
			priority: d,
			index:    i,
		}
		items[v] = &item
		pq[i] = &item
	}
	heap.Init(&pq)

	prev := make(map[V]V, len(vs))
	for pq.Len() > 0 {
		pu := heap.Pop(&pq).(*PriorityItem)
		u := pu.value.(V)
		for _, v := range g.Neighbors(u) {
			d := pu.priority + g.Weight(u, v)
			pv := items[v]
			if pv.priority > d {
				pv.priority = d
				heap.Fix(&pq, pv.index)
				prev[v] = u
			}
		}
	}
	return prev
}

// ----------------------------------------------------------------------------
// /usr/share/go-1.24/src/container/heap/example_pq_test.go

// Copyright 2012 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// A PriorityItem is something we manage in a priority queue.
type PriorityItem struct {
	value    any     // The value of the item; arbitrary.
	priority float64 // The priority of the item in the queue.
	// The index is needed by update and is maintained by the heap.Interface methods.
	index int // The index of the item in the heap.
}

// A PriorityQueue implements heap.Interface and holds PriorityItems.
type PriorityQueue []*PriorityItem

func (pq PriorityQueue) Len() int { return len(pq) }

func (pq PriorityQueue) Less(i, j int) bool {
	return pq[i].priority < pq[j].priority
}

func (pq PriorityQueue) Swap(i, j int) {
	pq[i], pq[j] = pq[j], pq[i]
	pq[i].index = i
	pq[j].index = j
}

func (pq *PriorityQueue) Push(x any) {
	n := len(*pq)
	item := x.(*PriorityItem)
	item.index = n
	*pq = append(*pq, item)
}

func (pq *PriorityQueue) Pop() any {
	old := *pq
	n := len(old)
	item := old[n-1]
	old[n-1] = nil  // don't stop the GC from reclaiming the item eventually
	item.index = -1 // for safety
	*pq = old[0 : n-1]
	return item
}
