#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <strsafe.h>


#define ERROR(fmt, ...) \
        do { \
                const WCHAR _fmt[] = L"[ERROR] %s@%d (%s): " fmt L"\n"; \
                WCHAR msg[sizeof(_fmt)]; \
                StringCchPrintfW(&msg, sizeof(msg), _fmt, ##__VA_ARGS__); \
                MessageBoxW(NONE, msg, "", MB_OK); \
                ExitProcess(1); \
        } while (0)


int WINAPI wWinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        PWSTR pCmdLine,
        int nCmdShow
) {
        int argc;
        LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

        RegisterClassExW(&(WNDCLASSEXW){
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
        });

        HWND hWnd = CreateWindowExW(
                // TODO
        );
        if (!hWnd) { } // TODO: ERROR()

        ShowWindow(hWnd, nCmdShow);

        MSG msg = {};
        while (GetMessageW(&msg, NULL, 0, 0) > 0) {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
        }

        return 0;
}