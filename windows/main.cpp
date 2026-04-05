#include <vector>

#ifndef UNICODE
#define UNICODE
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <strsafe.h>
#include <shlwapi.h>
#include <shobjidl.h>


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
                DestroyWindow(hWnd); \
                ExitProcess(1); \
        } while (0)


WCHAR cmd[32767];
size_t cmd_len = 0;

WCHAR prompt[32767];
size_t prompt_len = 0;

void ExecuteTarget(const WCHAR* target_path, const WCHAR* input_fstring) {
        cmd_len = wcslen(target_path);
        CopyMemory(cmd, target_path, cmd_len*sizeof(WCHAR));
        cmd[cmd_len++] = L' ';

        size_t _input_fstring_len = wcslen(input_fstring);
        for (int i = 0; i < _input_fstring_len; i++) {
                if (
                        i+2 >= _input_fstring_len ||  // at end

                        input_fstring[i] != L'%' ||
                        (
                                input_fstring[i+1] != L'f' &&
                                input_fstring[i+1] != L'd' &&
                                input_fstring[i+1] != L's'
                        ) ||
                        input_fstring[i+2] != L'{'
                ) {
                        cmd[cmd_len++] = input_fstring[i];
                        continue;
                }

                WCHAR dialog_type = input_fstring[i+1];

                i += 3; // skip L"%x{"

                while(
                        (input_fstring[i] != L'}') &&
                        (input_fstring[i] != L'\0')
                ) prompt[prompt_len++] = input_fstring[i++];

                if (input_fstring[i] == L'\0') ERR(
                        L"ERROR: Input format '%s' for target '%s' is missing closing '}'",
                        input_fstring, target_path
                );

                WCHAR input[32767];
                size_t input_len = 0;

                switch (dialog_type) {
                        case L'f':
                        {
                                IFileDialog* pFileOpen;
                                HRESULT _res = CoCreateInstance(
                                        CLSID_FileOpenDialog,
                                        NULL,
                                        CLSCTX_ALL,
                                        IID_IFileOpenDialog,
                                        (LPVOID*)&pFileOpen
                                );
                                if (_res != S_OK) ERR(
                                        L"Failed to create file open dialog instance (Win32 CoCreateInstance() return code: %d)",
                                        _res
                                );

                                HRESULT _res = pFileOpen->Show(hWnd);
                                if (_res != S_OK) ERR(
                                        L"Failed to show file open dialog (return code: %d)",
                                        _res
                                );

                                IShellItem* pItem;
                                HRESULT _res = pFileOpen->GetResult(&pItem);
                                if (_res != S_OK) ERR(
                                        L"Failed to get result from file open dialog (return code: %d)",
                                        _res
                                );

                                PWSTR pszFilePath;
                                HRESULT _res = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                                if (_res != S_OK) ERR(
                                        L"Failed to get path from file open dialog result (return code: %d)",
                                        _res
                                );

                                // TODO

                                CoTaskMemFree(pszFilePath);
                                pItem->Release();
                                pFileOpen->Release();
                        }
                        break;
                        case L'd':
                        {
                                // TODO
                        }
                        break;
                        case L's':
                        {
                                // TODO
                        }
                        break;
                }

                cmd[cmd_len++] = L'"';
                CopyMemory(cmd+cmd_len, input, input_len*sizeof(WCHAR));
                cmd_len += input_len;
                cmd[cmd_len++] = L'"';

                prompt_len = 0;
        }

        cmd[cmd_len] = L'\0';
        ExitProcess(_wsystem(cmd));
}

typedef struct {
        WCHAR* target_path;
        WCHAR* label;
        WCHAR* input_fstring;
} Button;

int argc;
LPWSTR* argv;
WCHAR initial_working_directory[32767];
WCHAR exe_path[32767];
std::vector<Button> buttons;
int width;
int button_height;

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
        WCHAR* path_sep = StrRChrW(exe_path, NULL, L'\\');
        if (path_sep) *path_sep = L'\0';
        if (!SetCurrentDirectoryW(exe_path)) ERR(
                L"Failed to set working dir to launcher exe's parent dir ('%s') (Win32 GetLastError(): %d)",
                exe_path, GetLastError()
        );

        HRESULT _res = CoInitializeEx(
                NULL,
                (
                        COINIT_APARTMENTTHREADED |
                        COINIT_DISABLE_OLE1DDE |
                        COINIT_SPEED_OVER_MEMORY
                )
        );
        if (_res != S_OK) ERR(
                L"Failed to initialize Win32 COM (return code: %d)",
                _res
        );


        // Buttonless mode
        if ((wcslen(argv[2]) == 1) && (argv[2][0] == L' ')) ExecuteTarget(argv[1], argv[3]);


        for (int i = 1; i < argc;) {
                WCHAR* target_path = argv[i++];
                if (i == argc) ERR(L"Target '%s' is missing a label", target_path);
                WCHAR* button_label = argv[i++];
                if (i == argc) ERR(L"Target '%s' with label '%s' is missing an input format", target_path, button_label);
                WCHAR* input_format = argv[i++];

                buttons.push_back(Button{target_path, button_label, input_format});
        }

        int largest_label_width = 0;
        for (Button& button : buttons) {
                SIZE text_size;
                GetTextExtentPointW(GetDC(NULL), button.label, wcslen(button.label), &text_size);
                if (text_size.cx > largest_label_width) largest_label_width = text_size.cx;
        }

        const int screen_w = GetSystemMetrics(SM_CXSCREEN);
        const int screen_h = GetSystemMetrics(SM_CYSCREEN);
        if (!screen_h) ERR( // screen_h is the important one (used for UI scaling)
                L"Failed to get screen height (Win32 GetLastError(): %d)",
                GetLastError()
        );

        const int font_height = screen_h / 30;

        width = largest_label_width + font_height;
        button_height = font_height;

        WNDCLASSEXW wc = {
                .cbSize = sizeof(WNDCLASSEXW),
                .lpfnWndProc = [](
                        HWND hWnd,
                        UINT uMsg,
                        WPARAM wParam,
                        LPARAM lParam
                ) -> LRESULT {
                        switch (uMsg) {
                                case WM_CREATE:
                                        for (size_t i = 0; i < buttons.size(); i++) {
                                                if (!CreateWindowExW(
                                                        0,
                                                        L"BUTTON",
                                                        buttons[i].label,
                                                        (WS_CHILD | WS_VISIBLE),
                                                        0, (button_height * i),
                                                        width, button_height,
                                                        hWnd,
                                                        (HMENU)i,
                                                        (HINSTANCE)GetWindowLongPtrW(hWnd, GWLP_HINSTANCE),
                                                        NULL
                                                )) ERR(
                                                        L"Failed to create button for target '%s' with label '%s' and input fstring '%s' (Win32 GetLastError(): %d)",
                                                        buttons[i].target_path, buttons[i].label, buttons[i].input_fstring, GetLastError()
                                                );
                                        }
                                        return 0;

                                case WM_COMMAND:
                                        switch (HIWORD(wParam)) {
                                                case BN_CLICKED:
                                                {
                                                        ExecuteTarget(
                                                                buttons[LOWORD(wParam)].target_path,
                                                                buttons[LOWORD(wParam)].input_fstring
                                                        );
                                                }
                                                break;
                                        }
                                        return 0;

                                case WM_DESTROY:
                                        if (!SetCurrentDirectoryW(initial_working_directory)) ERR(
                                                L"Failed to change back working directory to '%s' (Win32 GetLastError(): %d)",
                                                initial_working_directory, GetLastError()
                                        );

                                        CoUninitialize();

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
                WS_CAPTION | WS_SYSMENU | WS_POPUP,

                (screen_w/2 - width/2), (screen_h/2 - (button_height * (buttons.size() + 2))/2),
                width, (button_height * (buttons.size() + 1)),

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