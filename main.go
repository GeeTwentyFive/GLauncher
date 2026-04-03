package main

import (
	"fmt"
	"os"
	"path/filepath"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/dialog"
)

var w fyne.Window

func askConfirm(message string) bool {
	ch := make(chan bool, 1) // to block execution until receive input

	dialog.ShowConfirm(
		"",
		message,
		func(reply bool) { ch <- reply },
		w,
	)

	return <-ch // block execution until receive input
}

func askString(message string) string {
	ch := make(chan string, 1) // to block execution until receive input

	dialog.ShowEntryDialog(
		"",
		message,
		func(entry string) { ch <- entry },
		w,
	)

	return <-ch // block execution until receive input
}

func askFile(message string) string {
	ch := make(chan string, 1) // to block execution until receive input

	fileDialog := dialog.NewFileOpen(
		func(reader fyne.URIReadCloser, err error) {
			if (err != nil) || (reader == nil) { // TODO: Handle err != nil separately
				ch <- ""
			} else {
				ch <- reader.URI().Path()
			}
		},
		w,
	)
	fileDialog.SetTitleText(message)
	fileDialog.Show()

	return <-ch // block execution until receive input
}

func askDirectory(message string) string {
	ch := make(chan string, 1) // to block execution until receive input

	dirDialog := dialog.NewFolderOpen(
		func(lu fyne.ListableURI, err error) {
			if (err != nil) || (lu == nil) { // TODO: Handle err != nil separately
				ch <- ""
			} else {
				ch <- lu.Path()
			}
		},
		w,
	)
	dirDialog.SetTitleText(message)
	dirDialog.Show()

	return <-ch // block execution until receive input
}

func executeTarget(target_path string, input_fstring string) {
	// TODO

	// TEMP; TEST:
	askConfirm(fmt.Sprintf("%s %s", target_path, input_fstring))
	askConfirm(askString("TEST TEXT INPUT"))
	askConfirm(askFile("TEST FILE INPUT"))
	askConfirm(askDirectory("TEST DIR INPUT"))
}

func main() {
	if len(os.Args) < 4 {
		fmt.Println("USAGE: <<PATH/TO/TARGET> <BUTTON_LABEL> <INPUT_FORMAT>> ...")
		os.Exit(1)
	}

	initial_dir, _ := os.Getwd()
	defer os.Chdir(initial_dir)

	if err := os.Chdir(filepath.Dir(os.Args[0])); err != nil {
		panic(err)
	}

	w = app.New().NewWindow("")

	// Buttonless mode:
	if os.Args[2] == " " {
		go executeTarget(os.Args[1], os.Args[3]) // (has to be a goroutine to not block UI loop)
		w.ShowAndRun()
		return
	}

	buttons := container.NewVBox()
	// TODO (`buttons.Add(widget.NewButton())`)

	w.SetContent(buttons)
	w.ShowAndRun()
}
