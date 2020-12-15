#include "ButtonControl.h"

HWND ButtonControl::create(HWND hWndParent) {
    this->hWndParent = hWndParent;
    HWND hEdit = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("BUTTON"),
            "Generate",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0,
            0,
            0,
            0,
            hWndParent,
            (HMENU) 0,
            (HINSTANCE) GetWindowLongPtr(hWndParent, GWLP_HINSTANCE),
            NULL);

    this->hWnd = hEdit;
    return hEdit;
}



