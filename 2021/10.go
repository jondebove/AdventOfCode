package main

import (
	"fmt"
	"io"
	"slices"
)

func main() {
	ans1 := 0
	ans2 := 0

	scores := make([]int, 0, 100)

	for {
	nextLine:
		line := ""
		_, err := fmt.Scanln(&line)
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}

		stack := make([]byte, 0, len(line))
		for i := 0; i < len(line); i++ {
			switch line[i] {
			case ')':
				if stack[len(stack)-1] != '(' {
					ans1 += 3
					goto nextLine
				}
			case ']':
				if stack[len(stack)-1] != '[' {
					ans1 += 57
					goto nextLine
				}
			case '}':
				if stack[len(stack)-1] != '{' {
					ans1 += 1197
					goto nextLine
				}
			case '>':
				if stack[len(stack)-1] != '<' {
					ans1 += 25137
					goto nextLine
				}
			default:
				stack = append(stack, line[i])
				continue
			}
			stack = stack[:len(stack)-1]
		}

		score := 0
		for i := len(stack) - 1; i >= 0; i-- {
			switch stack[i] {
			case '(':
				score = score*5 + 1
			case '[':
				score = score*5 + 2
			case '{':
				score = score*5 + 3
			case '<':
				score = score*5 + 4
			default:
				panic("unreachable")
			}
		}
		scores = append(scores, score)
	}
	slices.Sort(scores)
	ans2 = scores[len(scores)/2]

	fmt.Println(ans1, ans2)
}
