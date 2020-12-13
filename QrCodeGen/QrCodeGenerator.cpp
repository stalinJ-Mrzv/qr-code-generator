#include <cstdio>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include "QrCoder.h"


QrCoder qrCoder("https://cdnimg.rg.ru/img/content/193/95/65/1_(9)_d_850.jpg");
vector<vector<unsigned int>> qrcode;

int width = 0;
int height = 0;

void drawQrCode(HWND hWnd, vector<vector<unsigned int>> qrcode) {
    uint moduleWidth = width / qrcode[0].size();
    uint moduleHeight = height / qrcode.size();
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    SelectObject(hdc, hBrush);
    for (int i = 0; i < qrcode.size(); i++) {
        for (int j = 0; j < qrcode[0].size(); j++) {
            if (qrcode[i][j] == 1) {
                Rectangle(hdc, j * moduleWidth, i * moduleHeight, (j + 1) * moduleWidth, (i + 1) * moduleHeight);
            }
        }
    }
    EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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
            700,
            700,
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

    switch(message) {
        case WM_CREATE:
            qrcode = qrCoder.generate();
            break;

        case WM_PAINT:
            drawQrCode(hWnd, qrcode);
            break;

        case WM_SIZE:
            width = LOWORD(lParam);
            height = HIWORD(lParam);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}