import os
import sys
import subprocess
import tkinter as tk
import tkinter.simpledialog as tk_dialog
import tkinter.filedialog as tk_filedialog


# For Pyinstaller
if getattr(sys, "frozen", False):
        os.chdir(os.path.dirname(sys.executable))
else:
        os.chdir(os.path.abspath(os.path.dirname(__file__)))


def create_process(args: list[str]):
        if os.name == "nt":
                subprocess.Popen(args, creationflags=subprocess.DETACHED_PROCESS)
        else:
                subprocess.Popen(args, start_new_session=True)


if len(sys.argv) < 3:
        print("Usage: <PATH/TO/TARGET> <BUTTON_LABEL> [[-file <PROMPT> | -dir <PROMPT> | -text <PROMPT>] ...] ...")
        sys.exit()

buttons = [] # Format: [(target_path, button_label, inputs[(type, prompt)]), ...]
i = 1
while i < len(sys.argv):
        target = sys.argv[i]
        i += 1
        if i == len(sys.argv):
                print(f"ERROR: Target '{target}' is missing a label")
                sys.exit(1)
        label = sys.argv[i]
        i += 1
        inputs = []
        while (i < len(sys.argv)) and (sys.argv[i][0] == '-'):
                if sys.argv[i] not in ("-file", "-dir", "-text"):
                        print(f"ERROR: Invalid input type '{sys.argv[i]}' for target '{target}'")
                        sys.exit(1)
                if i+1 == len(sys.argv):
                        print(f"ERROR: Input '{sys.argv[i]}' in target '{target}' is missing a prompt")
                        sys.exit(1)
                inputs.append((sys.argv[i], sys.argv[i+1]))
                i += 2
        buttons.append((target, label, inputs))

root = tk.Tk()
root.title("")
root.eval("tk::PlaceWindow . center")

def on_button_pressed(target_path: str, inputs: list[(str, str)]):
        args = []
        for input in inputs:
                if input[0] == "": continue
                match input[0]:
                        case "-file":
                                file_path = tk_filedialog.askopenfilename(title=input[1])
                                if file_path == (): sys.exit(1)
                                args.append(file_path)
                        case "-dir":
                                dir_path = tk_filedialog.askdirectory(title=input[1])
                                if dir_path == (): sys.exit(1)
                                args.append(dir_path)
                        case "-text":
                                text = tk_dialog.askstring("", input[1])
                                if (text == None) or (text == ""): sys.exit(1)
                                args.append(text)
        create_process([target_path] + args)
        sys.exit()

for button in buttons:
        tk.Button(
                root,
                text=button[1],
                command=(lambda t=button[0], i=button[2]: on_button_pressed(t, i))
        ).pack()

root.mainloop()
