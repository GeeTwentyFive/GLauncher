#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include <linux/limits.h>
#include <unistd.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>


#define ERROR(fmt, ...) \
        do { Fl::fatal( \
                "[ERROR] %s@%d (%s): " fmt "\n", \
                        __FILE__, __LINE__, __func__, \
                        ##__VA_ARGS__ \
        ); } while (0)


char cmd[ARG_MAX];
int cmd_len = 0;

char prompt[ARG_MAX];
int prompt_len = 0;

void ExecuteTarget(const char* target_path, const char* input_fstring) {
        cmd_len = strlen(target_path);
        memcpy(cmd, target_path, cmd_len);
        cmd[cmd_len++] = ' ';

        int _input_fstring_len = strlen(input_fstring);
        for (int i = 0; i < _input_fstring_len; i++) {
                if (
                        i+2 >= _input_fstring_len ||  // at end

                        input_fstring[i] != '%' ||
                        (
                                input_fstring[i+1] != 'f' &&
                                input_fstring[i+1] != 'd' &&
                                input_fstring[i+1] != 's'
                        ) ||
                        input_fstring[i+2] != '{'
                ) {
                        cmd[cmd_len++] = input_fstring[i];
                        continue;
                }

                char dialog_type = input_fstring[i+1];

                i += 3; // skip "%x{"

                while(
                        (input_fstring[i] != '}') &&
                        (input_fstring[i] != '\0')
                ) prompt[prompt_len++] = input_fstring[i++];

                if (input_fstring[i] == '\0') {
                        Fl::fatal(
                                "ERROR: Input format '%s' for target '%s' is missing closing '}'",
                                input_fstring, target_path
                        );
                }

                char* input;

                switch (dialog_type) {
                        case 'f':
                        {
                                char* target = fl_file_chooser(prompt, "", "");
                                if (!target) ERROR("User cancelled file selection for target '%s'", target_path);
                                input = target;
                        }
                        break;
                        case 'd':
                        {
                                char* target = fl_dir_chooser(prompt, "");
                                if (!target) ERROR("User cancelled directory selection for target '%s'", target_path);
                                input = target;
                        }
                        break;
                        case 's':
                        {
                                char* text_input = (char*)fl_input(prompt);
                                if (!text_input) ERROR("User cancelled text input for target '%s'", target_path);
                                input = text_input;
                        }
                        break;
                }

                cmd[cmd_len++] = '"';
                int input_len = strlen(input);
                memcpy(cmd+cmd_len, input, input_len);
                cmd_len += input_len;
                cmd[cmd_len++] = '"';

                prompt_len = 0;
        }

        cmd[cmd_len] = '\0';
        exit(system(cmd));
}

typedef struct {
        char* target_path;
        char* label;
        char* input_fstring;
} Button;

char initial_working_directory[PATH_MAX];

int main(int argc, char* argv[]) {
        int screen_w, screen_h;
        {
                int _x, _y;
                Fl::screen_xywh(_x, _y, screen_w, screen_h);
        }
        if (screen_h == -1) ERROR("Unable to determine screen height");

        const int font_height = screen_h / 30;

        fl_font(FL_SCREEN, font_height);

        if (argc < 4) Fl::fatal("USAGE: <<PATH/TO/TARGET> <BUTTON_LABEL> <INPUT_FORMAT>> ...");

        if (!getcwd(initial_working_directory, PATH_MAX)) ERROR("Failed to get current working directory");
        atexit([](){
                if (chdir(initial_working_directory) != 0) {
                        ERROR(
                                "Failed to change working directory back to initial one ('%s')",
                                initial_working_directory
                        );
                }
        });

        char* path_sep = strrchr(argv[0], '/');
        if (path_sep) *path_sep = '\0';
        if (chdir(argv[0]) != 0) ERROR("Failed to change working directory to '%s'", argv[0]);


        // Buttonless mode
        if ((strlen(argv[2]) == 1) && (argv[2][0] == ' ')) ExecuteTarget(argv[1], argv[3]);


        std::vector<Button> buttons;
        for (int i = 1; i < argc;) {
                char* target_path = argv[i++];
                if (i == argc) ERROR("Target '%s' is missing a label", target_path);
                char* button_label = argv[i++];
                if (i == argc) ERROR("Target '%s' with label '%s' is missing an input format", target_path, button_label);
                char* input_format = argv[i++];

                buttons.push_back(Button{target_path, button_label, input_format});
        }

        int largest_label_width = 0;
        for (Button& button : buttons) {
                int w, _h;
                fl_measure(button.label, w, _h);
                if (w > largest_label_width) largest_label_width = w;
        }

        const int width = largest_label_width + font_height;
        const int button_height = font_height;

        const int window_height = button_height * buttons.size();
        Fl_Window* window = new Fl_Window(
                (screen_w/2) - (width/2),
                (screen_h/2) - (window_height/2),
                width,
                window_height
        );

        for (int i = 0; i < buttons.size(); i++) {
                Fl_Button* button = new Fl_Button(
                        0,
                        button_height * i,
                        width,
                        button_height,
                        buttons[i].label
                );
                button->user_data(&buttons[i]);
                button->callback([](Fl_Widget*, void* user_data){
                        Button* _button = (Button*)user_data;
                        ExecuteTarget(
                                _button->target_path,
                                _button->input_fstring
                        );
                });
        }

        window->end();
        window->show();
        return Fl::run();
}
