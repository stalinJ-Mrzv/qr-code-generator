#include "EditControl.h"

HWND EditControl::create(HWND hWndParent) {
    this->hWndParent = hWndParent;
    HWND hEdit = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            NULL,
            WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
            0,
            0,
            0,
            0,
            hWndParent,
            NULL,
            (HINSTANCE) GetWindowLongPtr(hWndParent, GWLP_HINSTANCE),
            NULL);

    this->hWnd = hEdit;
    return hEdit;
}



