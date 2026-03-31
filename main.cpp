#include <string>
#include <vector>

#include <FL/Fl.h>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.h>
#include <FL/Fl_Button.H>


#define BUTTON_HEIGHT 40


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
        fl_font(FL_HELVETICA, 14);

        if (argc < 3) Fl::fatal("Usage: <PATH/TO/TARGET> <BUTTON_LABEL> [[-file <PROMPT> | -dir <PROMPT> | -text <PROMPT>] ...] ...");

        std::vector<Button> buttons;
        for (int i = 1; i < argc;) {
                std::string target_path = argv[i++];
                if (i == argc) Fl::fatal("Target '%s' is missing a label", target_path.c_str());
                std::string button_label = argv[i++];

                std::vector<ButtonInput> inputs;
                while (i < argc && argv[i][0] == '-') {
                        ButtonInput input;

                        switch (argv[i++][1]) {
                                case 'f': input.input_type = BUTTON_INPUT_TYPE_FILE; break;
                                case 'd': input.input_type = BUTTON_INPUT_TYPE_DIR; break;
                                case 't': input.input_type = BUTTON_INPUT_TYPE_TEXT; break;
                                default: Fl::fatal("Invalid input type '%s' for target '%s'", argv[i-1], target_path.c_str());
                        }

                        if (i == argc) Fl::fatal("Input '%s' in target '%s' is missing a prompt", argv[i-1], target_path.c_str());
                        input.prompt = std::string(argv[i++]);

                        inputs.push_back(input);
                }

                buttons.push_back(Button{target_path, button_label, inputs});
        }

        // std::string out; // vvv TEMP; TEST vvv
        // for (Button& button : buttons) {
        //         out += button.target_path + '\n';
        //         out += button.button_label + '\n';
        //         for (ButtonInput& input : button.inputs) {
        //                 out += std::to_string(input.input_type) + '\n';
        //                 out += input.prompt + '\n';
        //         }
        //         out += '\n';
        // }
        // Fl::fatal(out.c_str()); // ^^^ TEMP; TEST ^^^

        int largest_label_width = 0;
        for (Button& button : buttons) {
                int w, _h;
                fl_measure(button.button_label.c_str(), w, _h);
                if (w > largest_label_width) largest_label_width = w;
        }

        Fl_Window* window = new Fl_Window(
                largest_label_width,
                BUTTON_HEIGHT * buttons.size()
        );

        Fl_Button* _TEST = new Fl_Button(0, 0, largest_label_width, BUTTON_HEIGHT, "TEST"); // TEMP; TEST
        _TEST->callback([](Fl_Widget*){ Fl::fatal("Button clicked!"); }); // TEMP; TEST
        // TODO: button press handler & build GUI

        window->end();
        window->show();
        return Fl::run();
}