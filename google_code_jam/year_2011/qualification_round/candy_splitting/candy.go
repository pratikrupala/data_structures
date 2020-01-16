package main

import "fmt"

func main() {
	var tc = 0

	fmt.Scanf("%d", &tc)
	if tc < 0 || tc > 100 {
		fmt.Println("Invalid number of test cases.")
		return
	}

	for j := 1; j <= tc; j++ {
		var inum = 0
		var xresult = 1
		var result = 0

		fmt.Scanf("%d", &inum)
		if inum < 2 || inum > 1000 {
			fmt.Println("Invalid number of input numbers.")
			return
		}

		var input = make([]int, inum)

		for i := range input {
			fmt.Scanf("%d", &input[i])
		}

		xresult = input[0]
		for i := range input {
			if i != 0 {
				xresult ^= input[i]
			}
		}

//		fmt.Println("Result: ", xresult)

		if xresult != 0 {
			fmt.Printf("Case #%d: NO\n", j)
		} else {
			result = get_count(input, inum)
			fmt.Printf("Case #%d: %d\n", j, result)
		}
	}
}

func get_count(inp []int, n int) int {
	var min = inp[0]
	var sum = 0

	for i := 0; i < n; i++ {
		if inp[i] < min {
			min = inp[i]
		}
		sum += inp[i]
	}

	sum -= min

	return sum
}
