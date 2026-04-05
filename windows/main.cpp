#ifndef UNICODE
#define UNICODE
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <strsafe.h>


HWND hWnd = NULL;

#define ERR(fmt, ...) \
        do { \
                WCHAR title[8192]; \
                StringCchPrintfW( \
                        title, \
                        ARRAYSIZE(title), \
                        L"GLauncher [ERROR] %S@%d (%S)", \
                        __FILE__, __LINE__, __func__ \
                ); \
                WCHAR msg[8192]; \
                StringCchPrintfW( \
                        msg, \
                        ARRAYSIZE(msg), \
                        fmt, \
                        ##__VA_ARGS__ \
                ); \
                MessageBoxW(NULL, msg, title, MB_OK | MB_ICONERROR); \
                CloseWindow(hWnd); \
                ExitProcess(1); \
        } while (0)


int argc;
LPWSTR* argv;
WCHAR initial_working_directory[32767];
WCHAR exe_path[32767];

int WINAPI wWinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        PWSTR pCmdLine,
        int nCmdShow
) {
        argv = CommandLineToArgvW(GetCommandLineW(), &argc);

        if (argc < 4) ERR(L"USAGE: <<PATH/TO/TARGET> <BUTTON_LABEL> <INPUT_FORMAT>> ...");

        if (!GetCurrentDirectoryW(
                ARRAYSIZE(initial_working_directory),
                initial_working_directory
        )) ERR(
                L"Failed to get current working directory (Win32 GetLastError(): %d)",
                GetLastError()
        );

        if (!GetModuleFileNameW(NULL, exe_path, ARRAYSIZE(exe_path))) ERR(
                L"Failed to get path to exe (Win32 GetLastError(): %d)",
                GetLastError()
        );
        // TODO: Change dir to app dir

        const int screen_h = GetSystemMetrics(SM_CYSCREEN);
        if (!screen_h) ERR(L"Failed to get screen height (Win32 GetLastError(): %d)", GetLastError());

        const int font_height = screen_h / 30;

        WNDCLASSEXW wc = {
                .cbSize = sizeof(WNDCLASSEXW),
                .lpfnWndProc = [](
                        HWND hWnd,
                        UINT uMsg,
                        WPARAM wParam,
                        LPARAM lParam
                ) -> LRESULT {
                        switch (uMsg) {
                                // TODO

                                case WM_DESTROY:
                                        if (!SetCurrentDirectoryW(initial_working_directory)) ERR(
                                                L"Failed to change back working directory to '%s' (Win32 GetLastError(): %d)",
                                                initial_working_directory, GetLastError()
                                        );

                                        PostQuitMessage(0);
                                        return 0;
                        }

                        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
                },
                .hInstance = hInstance,
                .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
                .lpszClassName = L"MainWindowClass"
        };
        if (!RegisterClassExW(&wc)) ERR(
                L"Failed to register WNDCLASSEXW for window (Win32 GetLastError(): %d)",
                GetLastError()
        );

        hWnd = CreateWindowExW(
                0, wc.lpszClassName, L"",
                WS_CAPTION | WS_SYSMENU,

                // TODO: Calculate size & pos for:
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

                NULL, NULL, hInstance, NULL
        );
        if (!hWnd) ERR(
                L"Failed to create window (Win32 GetLastError(): %d)",
                GetLastError()
        );

        const HFONT font = CreateFontW(font_height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FF_SWISS, 0);
        if (!font) ERR(L"Failed to create font");
        SendMessageW(
                hWnd,
                WM_SETFONT,
                (WPARAM)font,
                FALSE
        );

        ShowWindow(hWnd, nCmdShow);

        MSG msg = {};
        while (GetMessageW(&msg, NULL, 0, 0) > 0) {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
        }

        return 0;
}