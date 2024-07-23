package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

// Interpreter struct to hold the state of the interpreter
type Interpreter struct{}

// NewInterpreter creates a new instance of Interpreter
func NewInterpreter() *Interpreter {
	return &Interpreter{}
}

// ExecuteCommand parses and executes a given command
func (i *Interpreter) ExecuteCommand(command string) {
	parts := strings.Fields(command)
	if len(parts) == 0 {
		return
	}

	switch parts[0] {
	case "print":
		if len(parts) > 1 {
			fmt.Println(strings.Join(parts[1:], " "))
		} else {
			fmt.Println("Error: 'print' command requires an argument")
		}
	default:
		fmt.Println("Error: Unknown command")
	}
}

func main() {
	interpreter := NewInterpreter()
	scanner := bufio.NewScanner(os.Stdin)

	fmt.Println("Simple Scripting Language Interpreter")
	fmt.Println("Type 'print <message>' to print a message or 'exit' to quit")

	for {
		fmt.Print("> ")
		if !scanner.Scan() {
			break
		}
		line := scanner.Text()
		if line == "exit" {
			break
		}
		interpreter.ExecuteCommand(line)
	}

	if err := scanner.Err(); err != nil {
		fmt.Fprintln(os.Stderr, "Error reading input:", err)
	}
}
