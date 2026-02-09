#include <iostream>
#include <filesystem>
#include <unistd.h>

#include "webview.h"


#ifdef _WIN32
#define RUN_PREFIX ".\\"
#else
#define RUN_PREFIX "./"
#endif


int main(int argc, const char* argv[]) {
try {
        if (argc == 1) {
                std::cout << "USAGE: <TARGET_1> [TARGET_2] [...]" << std::endl;
                return 0;
        }

        std::string target_dir = std::filesystem::path(argv[0]).parent_path().string();
        int err = chdir(target_dir.c_str());
        if (err != 0) throw std::runtime_error(
                std::string("ERROR: Failed to change current working directory to ") + target_dir
                + "\n\twith chdir error code " + std::to_string(err)
        );

        webview::webview w(false, nullptr);
        std::string html = ""; // TODO
        for (int i = 0; i < argc; i++) {
                // TODO
        }
        html += ""; // TODO
        w.set_html(html);
        w.run();

        return 0;
} catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
}
}