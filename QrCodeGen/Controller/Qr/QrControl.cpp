#include "QrControl.h"

HWND QrControl::create(HWND hWndParent) {
    this->hWndParent = hWndParent;
    HWND hQr = CreateWindowEx(
            SS_WHITEFRAME,
            _T("STATIC"),
            NULL,
            WS_BORDER | WS_CHILD | WS_VISIBLE,
            0,
            0,
            0,
            0,
            hWndParent,
            NULL,
            (HINSTANCE) GetWindowLongPtr(hWndParent, GWLP_HINSTANCE),
            NULL);
    this->hWnd = hQr;
    this->setSize(size);
    return hQr;
}

void QrControl::drawQrCode() {
    uint moduleWidth = this->size.width / this->qrcode[0].size();
    uint moduleHeight = this->size.height / this->qrcode.size();
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    SelectObject(hdc, hBrush);
    for (int i = 0; i < this->qrcode.size(); i++) {
        for (int j = 0; j < this->qrcode[0].size(); j++) {
            if (this->qrcode[i][j] == 1) {
                Rectangle(hdc, j * moduleWidth, i * moduleHeight, (j + 1) * moduleWidth, (i + 1) * moduleHeight);
            }
        }
    }
    EndPaint(hWnd, &ps);
}

matrixUInt QrControl::generate(string sourceText) {
    this->qrCoder.setSourceText(sourceText);
    this->qrcode = this->qrCoder.generate();
    return this->qrcode;
}
