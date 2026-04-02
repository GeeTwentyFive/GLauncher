#include <string>
#include <filesystem>
#include <cstdlib>
#ifdef _WIN32
        #include <direct.h>
        #define chdir _chdir
#else
        #include <unistd.h>
#endif
#include <vector>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>


void ExecuteTarget(std::string target_path, std::string input_fstring) {
        std::string inputs = "";

        // TODO: Parse input fstring -> gather inputs

        Fl::fatal("'%s' + '%s'", target_path.c_str(), input_fstring.c_str()); // TEMP; TEST
        // std::string cmd = target_path + ' ' + inputs;
        // int return_code = std::system(cmd.c_str());
        // if (return_code != 0) Fl::fatal("ERROR: Failed to execute `%s` with code %d", cmd.c_str(), return_code);

        exit(0);
}

typedef struct {
        std::string target_path;
        std::string label;
        std::string input_fstring;
} Button;

std::string initial_working_directory;
int main(int argc, char* argv[]) {
        int font_height = Fl::h() / 45;

        fl_font(FL_HELVETICA, font_height);

        if (argc < 4) Fl::fatal("Usage: <PATH/TO/TARGET> <BUTTON_LABEL> <INPUT_FORMAT> [PATH/TO/TARGET2] ...");

        initial_working_directory = std::filesystem::current_path();
        std::atexit([](){
                if (chdir(initial_working_directory.c_str()) != 0) {
                        Fl::error("ERROR: Failed to change back to initial working directory '%s'", initial_working_directory.c_str());
                }
        });

        std::string launcher_path = std::filesystem::path(argv[0]).parent_path();
        if (chdir(launcher_path.c_str()) != 0) {
                Fl::fatal("ERROR: Failed to change working directory to '%s'", launcher_path.c_str());
        }


        // Buttonless mode
        if (std::string(argv[2]) == " ") ExecuteTarget(argv[1], argv[3]);


        std::vector<Button> buttons;
        for (int i = 1; i < argc;) {
                std::string target_path = argv[i++];
                if (i == argc) Fl::fatal("Target '%s' is missing a label", target_path.c_str());
                std::string button_label = argv[i++];
                if (i == argc) Fl::fatal("Target '%s' with label '%s' is missing an input format", target_path.c_str(), button_label.c_str());
                std::string input_format = argv[i++];

                buttons.push_back(Button{target_path, button_label, input_format});
        }

        int largest_label_width = 0;
        for (Button& button : buttons) {
                int w, _h;
                fl_measure(button.label.c_str(), w, _h);
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
                        buttons[i].label.c_str()
                );
                button->user_data(&buttons[i]);
                button->callback([](Fl_Widget*, void* user_data){
                        Button* _button = (Button*)user_data;
                        ExecuteTarget(_button->target_path, _button->input_fstring);
                });
        }

        window->end();
        window->show();

        return Fl::run();
}