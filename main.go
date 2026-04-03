package main

import (
	"fmt"
	"os"

	"fyne.io/fyne/v2/app"
)

func main() {
	args := os.Args[1:]

	if len(args) == 0 {
		fmt.Println("USAGE: <<PATH/TO/TARGET> <BUTTON_LABEL> <INPUT_FORMAT>> ...")
		os.Exit(1)
	}

	// TODO

	a := app.New()
	w := a.NewWindow("")

	// TODO

	w.ShowAndRun()
}
