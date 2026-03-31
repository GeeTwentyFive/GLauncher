#include <iostream>

// #include <FL/Fl.h>
// #include <FL/Fl_Window.h>
#include <nfd.hpp>


int main() {
        if (NFD::Init() == NFD_ERROR) {
                std::cout << "ERROR: Failed to initialize nativefiledialogs-extended" << std::endl;
                // TODO
                return 1;
        }

        nfdu8char_t* path;
        if (NFD::OpenDialog(path) == NFD_OKAY) {
                std::cout << path << std::endl;
        }

        NFD::Quit();
        return 0;
}