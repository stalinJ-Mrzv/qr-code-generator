#include <cstdio>
#include <tchar.h>
#include <windows.h>

#include "QrCoder.h"
#include "Controller/Edit/EditControl.h"
#include "Controller/Qr/QrControl.h"
#include "Controller/Button/ButtonControl.h"
#include "Controller/RadioButton/RadioButtonControl.h"

#define MAX_SYMBOLS 2956

SizeParams wndSize = {500, 700, 0, 0};

// controls:
EditControl edit;
WNDPROC lpEditWndProc;
TCHAR textData[MAX_SYMBOLS];

QrControl qrControl;

ButtonControl btn;
WNDPROC lpBtnWndProc;

RadioButtonControl rbL, rbM, rbQ, rbH;
vector<string> rbNames = {"L(7%)", "M(15%)", "Q(25%)", "H(30%)"};
arrayInt rbIds = {100, 101, 102, 103};

vector<RadioButtonControl> rbControls = {rbL, rbM, rbQ, rbH};

void drawHeaders(HWND hWnd) {
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc = BeginPaint(hWnd, &ps);

    string str1 = "QR Code Generator";
    string str2 = "Correction level:";
    SetRect(&rect, wndSize.width / 2 - 100, 10, wndSize.width / 2 + 100, 10 + 20);
    DrawTextA(hdc, str1.c_str(), str1.length(), &rect, DT_CENTER);
    SetRect(&rect, wndSize.width / 2 - 100, qrControl.getSize().y + qrControl.getSize().height + 10, wndSize.width / 2 + 100, qrControl.getSize().y + qrControl.getSize().height + 10 + 20);
    DrawTextA(hdc, str2.c_str(), str2.length(), &rect, DT_CENTER);
    EndPaint(hWnd, &ps);

}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK BtnGenProc(HWND, UINT, WPARAM, LPARAM);

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
    static HWND hBtn;
    static HWND hRBL;
    static HWND hRBM;
    static HWND hRBQ;
    static HWND hRBH;
    static vector<HWND> hRB = {hRBL, hRBM, hRBQ, hRBH};
    switch(message) {
        case WM_CREATE:
            hEdit = edit.create(hWnd);
            lpEditWndProc = (WNDPROC) SetWindowLongPtr(hEdit, GWL_WNDPROC, (LONG_PTR)&EditProc);
            hQr = qrControl.create(hWnd);
            hBtn = btn.create(hWnd);
            lpBtnWndProc = (WNDPROC) SetWindowLong(hBtn, GWL_WNDPROC, (LONG_PTR)&BtnGenProc);
            for (int i = 0; i < hRB.size(); i++) {
                hRB[i] = rbControls[i].create(hWnd, rbNames[i], rbIds[i]);
            }
            CheckRadioButton(hWnd, rbIds[0], rbIds[rbIds.size() - 1], rbIds[0]);
            break;

        case WM_PAINT:
            drawHeaders(hWnd);
            qrControl.drawQrCode();
            break;

        case WM_SIZE:
            wndSize.width = LOWORD(lParam);
            wndSize.height = HIWORD(lParam);
            edit.setSize({wndSize.width / 2, wndSize.height / 5, wndSize.width / 2 - wndSize.width / 4, 40});
            qrControl.setSize({wndSize.width - wndSize.width / 5, wndSize.height - edit.getSize().height - 180, wndSize.width / 2 - (wndSize.width - wndSize.width / 5) / 2, edit.getSize().y + edit.getSize().height + 10});
            for (int i = 0; i < hRB.size(); i++) {
                rbControls[i].setSize({70, 30, wndSize.width / 2 - 75 * i + 75, qrControl.getSize().y + qrControl.getSize().height + 20 + 10});
            }
            btn.setSize({120, 30, wndSize.width / 2 - 60, rbControls[0].getSize().y + rbControls[0].getSize().height + 10});
            break;

        case BM_CLICK:
            GetWindowText(hEdit, textData, MAX_SYMBOLS);
            qrControl.generate(textData);
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case EM_GETMODIFY:
            GetWindowText(hEdit, textData, MAX_SYMBOLS);
            qrControl.generate(textData);
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 100:
                case 101:
                case 102:
                case 103:
                    qrControl.setCorrectionLevel(LOWORD(wParam) - 100);
                    GetWindowText(hEdit, textData, MAX_SYMBOLS);
                    qrControl.generate(textData);
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                default:
                    break;
            }
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

LRESULT CALLBACK BtnGenProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
        case WM_LBUTTONDOWN:
            PostMessage(GetParent(hWnd), BM_CLICK, wParam, lParam);
            break;

        default:
            return CallWindowProc(lpBtnWndProc, hWnd, message, wParam, lParam);
    }
    return 0;
}