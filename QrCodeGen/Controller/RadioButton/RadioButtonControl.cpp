#include "RadioButtonControl.h"

HWND RadioButtonControl::create(HWND hWndParent) {
    this->hWndParent = hWndParent;
    HWND hRadioButton = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("BUTTON"),
            """",
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            0,
            0,
            0,
            0,
            hWndParent,
            (HMENU) id,
            (HINSTANCE) GetWindowLongPtr(hWndParent, GWLP_HINSTANCE),
            NULL);
    this->hWnd = hRadioButton;
    return hRadioButton;
}

HWND RadioButtonControl::create(HWND hWndParent, string name, int id) {
    this->hWndParent = hWndParent;
    HWND hRadioButton = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("BUTTON"),
            name.c_str(),
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            0,
            0,
            0,
            0,
            hWndParent,
            (HMENU) id,
            (HINSTANCE) GetWindowLongPtr(hWndParent, GWLP_HINSTANCE),
            NULL);
    this->id = id;
    this->hWnd = hRadioButton;
    return hRadioButton;
}

void RadioButtonControl::setId(int id) {
    this->id = id;
}

int RadioButtonControl::getId() {
    return this->id;
}