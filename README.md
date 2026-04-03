CLI-configured launcher which makes a GUI for CLI programs and gathers and passes various user input

TODO: GIF


# Usage

`<<PATH/TO/TARGET> <BUTTON_LABEL> <INPUT_FORMAT>> ...`

Input formats:
- `%f{OPTIONAL_PROMPT}` - file picker
- `%d{OPTIONAL_PROMPT}` - directory picker
- `%s{OPTIONAL_PROMPT}` - text input

Example: `GLauncher.exe example.exe "Click to launch first app" "%f{Select file} -o %d{Select output directory}" inner/example2.exe "Click to launch 2nd app" "-i %s{} --verbose"`

**Buttonless mode**: If you set the label of the first app's button to `" "` then no initial buttons GUI will be built and just that first app will be launched

^ example: `GLauncher.exe example.exe " " "-i f{} -o %d{} --name %s{} --verbose"`
