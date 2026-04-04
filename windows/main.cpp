#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>


int WINAPI wWinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        PWSTR pCmdLine,
        int nCmdShow
) {
        int argc;
        LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

        // TODO

        return 0;
}