package main

import (
	"fmt"
	"io"
)

func main() {
	ans1 := 0
	ans2 := 0

	var fishes []*node
	for {
		line := ""
		_, err := fmt.Scanln(&line)
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}

		n := newNode(nil)
		n.read(line)
		fishes = append(fishes, n)
	}

	sum := fishes[0]
	for i := 1; i < len(fishes); i++ {
		sum = add(sum, fishes[i])
	}
	ans1 = sum.magnitude()

	for i := 0; i < len(fishes); i++ {
		for j := i + 1; j < len(fishes); j++ {
			sum1 := add(fishes[i], fishes[j])
			sum2 := add(fishes[j], fishes[i])
			ans2 = max(ans2, sum1.magnitude(), sum2.magnitude())
		}
	}

	fmt.Println(ans1, ans2)
}

type node struct {
	value int
	left  *node
	right *node
	prev  *node
}

func newNode(prev *node) *node {
	return &node{value: -1, prev: prev}
}

func (n *node) clone(prev *node) *node {
	nn := newNode(prev)
	nn.value = n.value
	if n.left != nil {
		nn.left = n.left.clone(nn)
	}
	if n.right != nil {
		nn.right = n.right.clone(nn)
	}
	return nn
}

func (n *node) isLeaf() bool {
	return n.value != -1
}

func (n *node) level() int {
	ans := 0
	for n.prev != nil {
		ans++
		n = n.prev
	}
	return ans
}

func (n *node) root() *node {
	for n.prev != nil {
		n = n.prev
	}
	return n
}

func (n *node) backward() *node {
	p := n.prev
	for p != nil && p.left == n {
		n = p
		p = p.prev
	}
	if p == nil {
		return nil
	}
	for n = p.left; !n.isLeaf(); n = n.right {
	}
	return n
}

func (n *node) forward() *node {
	p := n.prev
	for p != nil && p.right == n {
		n, p = p, p.prev
	}
	if p == nil {
		return nil
	}
	for n = p.right; !n.isLeaf(); n = n.left {
	}
	return n
}

func (n *node) read(str string) int {
	i := 0
	if str[i] != '[' {
		n.value = int(str[i]) - '0'
		return 1
	}
	i++

	n.left = newNode(n)
	i += n.left.read(str[i:])

	if str[i] != ',' {
		panic(1)
	}
	i++

	n.right = newNode(n)
	i += n.right.read(str[i:])

	if str[i] != ']' {
		panic(1)
	}
	i++

	return i
}

func (n *node) String() string {
	if n.isLeaf() {
		return fmt.Sprintf("%d", n.value)
	}
	return fmt.Sprintf("[%s,%s]", n.left, n.right)
}

func add(l, r *node) *node {
	n := newNode(nil)
	n.left = l.clone(n)
	n.right = r.clone(n)
	n.reduce1()
	n.reduce2()
	return n
}

func (n *node) reduce1() {
	if n.explode() {
		return
	}
	if n.left != nil {
		n.left.reduce1()
	}
	if n.right != nil {
		n.right.reduce1()
	}
}

func (n *node) reduce2() {
	if n.split() && n.explode() {
		n = n.root()
	}
	if n.left != nil {
		n.left.reduce2()
	}
	if n.right != nil {
		n.right.reduce2()
	}
}

func (n *node) split() bool {
	if n.value > 9 {
		if n.left != nil {
			panic("left")
		}
		if n.right != nil {
			panic("right")
		}

		n.left = newNode(n)
		n.left.value = n.value / 2
		n.right = newNode(n)
		n.right.value = n.value - n.left.value
		n.value = -1
		return true
	}
	return false
}

func (n *node) explode() bool {
	if n.level() >= 4 && !n.isLeaf() {
		if !n.left.isLeaf() {
			panic("left")
		}
		if !n.right.isLeaf() {
			panic("right")
		}

		b := n.backward()
		if b != nil {
			b.value += n.left.value
		}
		f := n.forward()
		if f != nil {
			f.value += n.right.value
		}
		n.left = nil
		n.right = nil
		n.value = 0
		return true
	}
	return false
}

func (n *node) magnitude() int {
	if n.isLeaf() {
		return n.value
	}
	return 3*n.left.magnitude() + 2*n.right.magnitude()
}
