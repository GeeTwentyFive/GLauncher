#include <string>
#include <vector>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>


int main(int argc, char* argv[]) {
        fl_font(FL_HELVETICA, (Fl::h() / 45));

        if (argc < 3) Fl::fatal("Usage: <PATH/TO/TARGET> <BUTTON_LABEL> <INPUT_FORMAT> [INPUT(S)...] ...");

        // TODO

        return Fl::run();
}