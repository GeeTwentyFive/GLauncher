#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <linux/limits.h>
#include <unistd.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>


// Print formatted message to stderr with filename, line, and function name
#define ERROR(fmt, ...) \
        do { fprintf( \
                stderr, \
                "[ERROR] %s@%d (%s): " fmt "\n", \
                        __FILE__, __LINE__, __func__, \
                        ##__VA_ARGS__ \
        ); } while (0)


// int _OfferChoice(const char** choices, int num_of_choices) {
//         for (int i = 1; i < num_of_choices; i++) {
//                 printf("%d) %s", i, choices[i-1]);
//         }
//         // TODO
// }
// #define OfferChoice(...) \
//         _OfferChoice( \
//                 (const char*[]){__VA_ARGS__}, \
//                 sizeof((const char*[]){__VA_ARGS__}) / sizeof(const char*) \
//         )

char initial_working_directory[PATH_MAX];

int main(int argc, char* argv[]) {
        // TEMP; TEST:
        Fl_Window* window = new Fl_Window(340, 180);
        window->end();
        window->show();
        return Fl::run();

        if (argc < 4) {
                puts("USAGE: <<PATH/TO/TARGET> <BUTTON_LABEL> <INPUT_FORMAT>> ...");
                return 1;
        }

        if (!getcwd(initial_working_directory, PATH_MAX)) {
                ERROR("Failed to get current working directory");
                return 1;
        }
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
        if (!chdir(argv[0])) {
                ERROR("Failed to change woring directory to '%s'", argv[0]);
                return 1;
        }


        // TODO


        return 0;
}
