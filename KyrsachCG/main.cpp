#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <wchar.h>
#include "Engine.h"

const double PI = 3.141592653;
const int window_w = 600;
const int window_h = 400;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInstance, LoadIcon(NULL, IDI_APPLICATION),
        CLASS_NAME, WindowProc);

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, L"Cannot register class", L"Error", MB_OK);
        return -1;
    }

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"ComputerGraphic_4",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, window_w, window_h,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
    WNDCLASS NWC = { 0 };

    NWC.hIcon = Icon;
    NWC.hCursor = Cursor;
    NWC.hInstance = hInst;
    NWC.lpszClassName = Name;
    NWC.hbrBackground = BGColor;
    NWC.lpfnWndProc = Procedure;

    return NWC;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    RECT Rect;
    static Engine* engine;
    wchar_t msg[32];
    switch (uMsg)
    {
    case WM_CREATE:
        engine = new Engine(window_w, window_h);
        break;

    case WM_PAINT:
    {
        GetClientRect(hwnd, &Rect);
        HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 4));

        engine->draw(hdc);

        ValidateRect(hwnd, NULL);
        EndPaint(hwnd, &ps);

        break;
    }
    case WM_KEYDOWN:
        swprintf_s(msg, L"WM_KEYDOWN: 0x%x\n", wParam);
        OutputDebugString(msg);
        int keyPressed;
        keyPressed = int(wParam);
        switch (keyPressed) {
        case (int('D')):
            engine->move(-0.1, 0, 0);
            break;

        case (int('A')):
            engine->move(0.1, 0, 0);
            break;

        case (int('W')):
            engine->move(0, 0.1, 0);
            break;

        case (int('S')):
            engine->move(0, -0.1, 0);
            break;

        case (int('R')):
            engine->move(0, 0, 0.1);
            break;

        case (int('F')):
            engine->move(0, 0, -0.1);
            break;

        case (0x51):
            engine->rotZ(25.0f);
            break;

        case (0x45):
            engine->rotZ(-25.0f);
            break;

        case (0x5A):
            engine->scale(0.5f);
            break;

        case (0x58):
            engine->scale(2.0f);
            break;

        case (0x26):
            engine->rotX(25.0f);
            break;

        case (0x28):
            engine->rotX(-25.0f);
            break;

        case (0x25):
            engine->rotY(-25.0f);
            break;

        case (0x27):
            engine->rotY(25.0f);
            break;

        case(int('1')):
            engine->setActive(0);
            break;

        case(int('2')):
            engine->setActive(1);
            break;

        case(int('3')):
            engine->setActive(2);
            break;

        case(int('4')):
            engine->setActive(3);
            break;
        }


        InvalidateRect(hwnd, NULL, FALSE); //גûחמג סמבûעטÿ WM_PAINT
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}