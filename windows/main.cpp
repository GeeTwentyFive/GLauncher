#ifndef UNICODE
#define UNICODE
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <strsafe.h>


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
                ExitProcess(1); \
        } while (0)


int argc;
LPWSTR* argv;

int WINAPI wWinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        PWSTR pCmdLine,
        int nCmdShow
) {
        argv = CommandLineToArgvW(GetCommandLineW(), &argc);

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
                                        PostQuitMessage(0);
                                        return 0;
                        }

                        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
                },
                .hInstance = hInstance,
                .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
                .lpszClassName = L"MainWindowClass"
        };
        if (!RegisterClassExW(&wc)) ERR(L"Failed to register WNDCLASSEXW for window");

        HWND hWnd = CreateWindowExW(
                0, wc.lpszClassName, L"",
                WS_CAPTION | WS_SYSMENU,

                // TODO: Calculate size & pos for:
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

                NULL, NULL, hInstance, NULL
        );
        if (!hWnd) ERR(L"Failed to create window");

        ShowWindow(hWnd, nCmdShow);

        MSG msg = {};
        while (GetMessageW(&msg, NULL, 0, 0) > 0) {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
        }

        return 0;
}