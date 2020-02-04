package main

import (
	"os"
	"fmt"
	"bufio"
)

func main() {
	var tc = 0
	reader := bufio.NewReader(os.Stdin)
	
	fmt.Scanf("%d", &tc)
	if tc < 0 || tc > 100 {
		fmt.Println("Invalid number of test cases.")
		return
	}
	
	for j := 1; j <= tc; j++ {
		output := ""

		khali, _ := reader.ReadString('\n')
		input, _ := reader.ReadString('\n')
		
		for _, c := range input {
			switch c {
			case 'S':
				output += "E"
			case 'E':
				output += "S"
			default:
				khali += ""
			}
		}
		
		result := fmt.Sprintf("Case #%d: %s", j, output)
		fmt.Println(result)
	}
}
