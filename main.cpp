#include <FL/Fl.h>
#include <FL/fl_ask.H>
#include <FL/Fl_Window.h>


int main(int argc, char* argv[]) {
        // TODO: Parse args

        Fl_Window *window = new Fl_Window(340, 340);
                // ^ TODO:
                // - set width based on longest target label
                // - set height based on # of targets
        // TODO: button press handler & build GUI
        window->end();
        window->show();
        return Fl::run();
}