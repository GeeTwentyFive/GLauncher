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


#define ERROR(fmt, ...) \
        do { Fl::fatal( \
                "[ERROR] %s@%d (%s): " fmt "\n", \
                        __FILE__, __LINE__, __func__, \
                        ##__VA_ARGS__ \
        ); } while (0)


char cmd[ARG_MAX];
int cmd_len = 0;

void ExecuteTarget(const char* target_path, const char* input_fstring) {
        cmd_len = strlen(target_path);
        memcpy(cmd, target_path, cmd_len);
        cmd[cmd_len++] = ' '; cmd[cmd_len] = '\0';

        // TODO: Parse input_fstring -> gather inputs -> append to cmd

        puts(cmd); puts(input_fstring); exit(0); // TEMP; TEST
        //exit(system(cmd));
}

typedef struct {
        char* target_path;
        char* label;
        char* input_fstring;
} Button;

char initial_working_directory[PATH_MAX];

int main(int argc, char* argv[]) {
        ERROR("TEST - %s - %d", "wad", 727);

        const int font_height = Fl::h() / 45;

        fl_font(FL_HELVETICA, font_height);

        if (argc < 4) ERROR("USAGE: <<PATH/TO/TARGET> <BUTTON_LABEL> <INPUT_FORMAT>> ...");

        if (!getcwd(initial_working_directory, PATH_MAX)) ERROR("Failed to get current working directory");
        atexit([](){
                if (!chdir(initial_working_directory)) {
                        ERROR(
                                "Failed to change working directory back to initial one ('%s')",
                                initial_working_directory
                        );
                }
        });

        char* path_sep = strrchr(argv[0], '/');
        if (path_sep) *path_sep = '\0';
        if (!chdir(argv[0])) ERROR("Failed to change working directory to '%s'", argv[0]);


        // Buttonless mode
        if ((strlen(argv[2]) == 1) && (argv[2][0] == ' ')) ExecuteTarget(argv[1], argv[3]);


        std::vector<Button> buttons;
        for (int i = 1; i < argc;) {
                char* target_path = argv[i++];
                if (i == argc) Fl::fatal("Target '%s' is missing a label", target_path);
                char* button_label = argv[i++];
                if (i == argc) Fl::fatal("Target '%s' with label '%s' is missing an input format", target_path, button_label);
                char* input_format = argv[i++];

                buttons.push_back(Button{target_path, button_label, input_format});
        }

        int largest_label_width = 0;
        for (Button& button : buttons) {
                int w, _h;
                fl_measure(button.label, w, _h);
                if (w > largest_label_width) largest_label_width = w;
        }

        int button_height = font_height;

        Fl_Window* window = new Fl_Window(
                largest_label_width,
                button_height * buttons.size()
        );

        for (int i = 0; i < buttons.size(); i++) {
                Fl_Button* button = new Fl_Button(
                        0,
                        button_height * i,
                        largest_label_width,
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
