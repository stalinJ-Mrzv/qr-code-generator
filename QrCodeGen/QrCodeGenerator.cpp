#include <cstdio>
#include <tchar.h>
#include <windows.h>
#include <commctrl.h>

#include "QrCoder.h"
#include "Controller/Edit/EditControl.h"
#include "Controller/Qr/QrControl.h"

#define MAX_SYMBOLS 1024

SizeParams wndSize = {500, 700, 0, 0};

// controls:
EditControl edit;
WNDPROC lpEditWndProc;
TCHAR textData[MAX_SYMBOLS];

QrControl qrControl;

void drawText(HWND hWnd, string text, int x, int y, int textWidth, int textHeigh) {
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc = BeginPaint(hWnd, &ps);
    SetRect(&rect, x - textWidth, y, x + textWidth, y + textHeigh);
    DrawText(hdc, text.c_str(), text.length(), &rect, DT_CENTER);
    EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

int WinMain(HINSTANCE This, HINSTANCE Prev, LPTSTR cmd, int mode) {
    HWND hWnd;
    MSG msg;
    WNDCLASS wc;
    wc.hInstance = This;
    wc.lpszClassName = WinName;
    wc.lpfnWndProc = WndProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;

    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    if (!RegisterClass(&wc)) {
        return 0;
    }

    hWnd = CreateWindow(
            WinName,
            _T("QR Code Generator"),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            wndSize.width,
            wndSize.height,
            HWND_DESKTOP,
            NULL,
            This,
            NULL);

    ShowWindow(hWnd, mode);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit;
    static HWND hQr;
    switch(message) {
        case WM_CREATE:
            hEdit = edit.create(hWnd);
            lpEditWndProc = (WNDPROC) SetWindowLongPtr(hEdit, GWL_WNDPROC, (LONG_PTR)&EditProc);
            hQr = qrControl.create(hWnd);
            //qrControl.generate("this is test qr code generator");
            break;

        case WM_PAINT:
            drawText(hWnd, "QR Code Generator", wndSize.width / 2, 10, 200, 50);
            qrControl.drawQrCode();
            break;

        case WM_SIZE:
            wndSize.width = LOWORD(lParam);
            wndSize.height = HIWORD(lParam);
            edit.setSize({wndSize.width / 2, wndSize.height / 5, wndSize.width / 2 - wndSize.width / 4, 50});
            qrControl.setSize({wndSize.width - wndSize.width / 5, wndSize.height - edit.getSize().height - 120, wndSize.width / 2 - (wndSize.width - wndSize.width / 5) / 2, edit.getSize().y + edit.getSize().height + 10});
            break;

        case EM_GETMODIFY:
            GetWindowText(hEdit, textData, MAX_SYMBOLS);
            qrControl.generate(textData);
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
        case WM_KEYDOWN:
            PostMessage(GetParent(hWnd), EM_GETMODIFY, wParam, lParam);
            break;

        default:
            return CallWindowProc(lpEditWndProc, hWnd, message, wParam, lParam);
    }
    return 0;
}