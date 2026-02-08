#include <stdio.h>

#define TB_IMPL
#include "termbox2.h"


#ifdef _WIN32
#define RUN_PREFIX ".\\"
#else
#define RUN_PREFIX "./"
#endif


int main(int argc, const char* argv[]) {
        if (argc == 1) {
                puts("USAGE: <TARGET_1> [TARGET_2] [...]");
                return 0;
        }
        if (argc > 8) {
                puts("ERROR: More than 8 targets provided");
                return 1;
        }

        int err = tb_init();
        if (err != TB_OK) {
                printf("ERROR: Failed to initialize termbox2\n%d", err);
                return 1;
        }

        unsigned int input;
        struct tb_event ev;
        for (;;) {
                for (int i = 1; i < argc; i++) {
                        tb_printf(0, i-1, 0, 0, "%d - %s\n", i, argv[i]);
                }
                tb_printf(0, argc, 0, 0, "%d - Exit", argc);
                tb_present();

                tb_poll_event(&ev);
                if (ev.type != TB_EVENT_KEY) continue;
                input = (ev.ch - '0');
                if (input > argc) continue;
                if (input == argc) {tb_shutdown(); return 0;}
                break;
        }
        tb_shutdown();

        char cmd[256] = RUN_PREFIX;
        strcat(cmd, argv[input]);
        system(cmd);

        return 0;
}