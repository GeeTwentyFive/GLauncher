CLI-configured program which creates a GUI launcher which can gather and pass user input


# Usage

`<PATH/TO/TARGET> <BUTTON_LABEL> [[-file <PROMPT> | -dir <PROMPT> | -text <PROMPT>] ...] ...`

^ example: `bin/program1.exe "First program" -file "Select target file" bin/program2.exe "Second program" -dir "" -text "Enter key"`


# Build

`pyinstaller -F GLauncher.py`