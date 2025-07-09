package main

import (
	"fmt"
	"io"
	"strconv"
)

func main() {
	ans1 := int64(0)
	ans2 := int64(0)

	nums := [][]byte{}
	for i := 0; ; i++ {
		num := []byte{}
		_, err := fmt.Scan(&num)
		if err == io.EOF {
			break
		} else if err != nil {
			panic(err)
		}
		nums = append(nums, num)
	}

	gam := make([]byte, len(nums[0]))
	for i := range gam {
		gam[i] = mostcommon(nums, i)
	}
	ans1, _ = strconv.ParseInt(string(gam), 2, 64)
	ans1 *= (^ans1 & (1<<len(gam) - 1))

	ans2 = filter(nums, false)
	ans2 *= filter(nums, true)

	fmt.Println(ans1, ans2)
}

func mostcommon(nums [][]byte, bit int) byte {
	tot := 0
	for _, n := range nums {
		tot += int(n[bit]) - '0'
	}
	if tot*2 >= len(nums) {
		return '1'
	} else {
		return '0'
	}
}

func filter(nums [][]byte, invert bool) int64 {
	for i := range nums[0] {
		b := mostcommon(nums, i)
		k := 0
		for j := range nums {
			if (nums[j][i] == b) != invert {
				nums[k], nums[j] = nums[j], nums[k]
				k++
			}
		}
		nums = nums[:k]
		if k == 1 {
			break
		}
	}
	rate, _ := strconv.ParseInt(string(nums[0]), 2, 64)
	return rate
}
