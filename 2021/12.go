package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func main() {
	ans1 := 0
	ans2 := 0

	graph := make(map[string]*vertex)

	scn := bufio.NewScanner(os.Stdin)
	for scn.Scan() {
		edge := scn.Text()
		idx := strings.IndexByte(edge, '-')
		v1 := getVertex(graph, edge[:idx])
		v2 := getVertex(graph, edge[idx+1:])
		v1.next = append(v1.next, v2)
		v2.next = append(v2.next, v1)
	}

	s := graph["start"]
	e := graph["end"]
	dfs(s, s, e, false, &ans1)
	dfs(s, s, e, true, &ans2)

	fmt.Println(ans1, ans2)
}

type vertex struct {
	next []*vertex
	seen bool
	big  bool
}

func getVertex(graph map[string]*vertex, name string) *vertex {
	if v, ok := graph[name]; ok {
		return v
	}
	v := &vertex{big: strings.ToUpper(name[:1]) == name[:1]}
	graph[name] = v
	return v
}

func dfs(v, s, e *vertex, twice bool, count *int) {
	for _, u := range v.next {
		if u == s {
		} else if u == e {
			*count++
		} else if !u.seen {
			if !u.big {
				u.seen = true
			}
			dfs(u, s, e, twice, count)
			u.seen = false
		} else if twice {
			dfs(u, s, e, false, count)
		}
	}
}
