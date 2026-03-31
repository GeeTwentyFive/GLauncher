#include <string>
#include <vector>

#include <FL/Fl.h>
#include <FL/Fl_Window.h>


typedef enum {
        BUTTON_INPUT_TYPE_FILE,
        BUTTON_INPUT_TYPE_DIR,
        BUTTON_INPUT_TYPE_TEXT
} ButtonInputType;

typedef struct {
        ButtonInputType input_type;
        std::string prompt;
} ButtonInput;

typedef struct {
        std::string target_path;
        std::string button_label;
        std::vector<ButtonInput> inputs;
} Button;

int main(int argc, char* argv[]) {
        if (argc < 3) Fl::fatal("Usage: <PATH/TO/TARGET> <BUTTON_LABEL> [[-file <PROMPT> | -dir <PROMPT> | -text <PROMPT>] ...] ...");

        // TODO: Parse args
        std::vector<Button> buttons;
        for (int i = 1; i < argc; i++) {
                std::string target_path = argv[i++];
                if (i == argc) Fl::fatal("Target '%s' is missing a label", target_path);
                std::string button_label = argv[i++];

                std::vector<ButtonInput> inputs;
                while (i < argc && argv[i][0] == '-') {
                        // TODO
                }

                buttons.push_back(Button{target_path, button_label, inputs});
        }

        Fl_Window *window = new Fl_Window(340, 340);
                // ^ TODO:
                // - set width based on longest target label
                // - set height based on # of targets
        // TODO: button press handler & build GUI
        window->end();
        window->show();
        return Fl::run();
}