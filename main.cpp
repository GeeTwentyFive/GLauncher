#include <FL/Fl.h>
#include <FL/Fl_Window.h>
//#include <FL/Fl_Box.h>


int main(int argc, char* argv[]) {
        Fl_Window* window = new Fl_Window(340, 180);
        // Fl_Box *box = new Fl_Box(20, 40, 300, 100, "Hello, World!");
        // box->box(FL_UP_BOX);
        // box->labelfont(FL_BOLD + FL_ITALIC);
        // box->labelsize(36);
        // box->labeltype(FL_SHADOW_LABEL);
        // window->end();
        // window->show(argc, argv);
        return Fl::run();
}