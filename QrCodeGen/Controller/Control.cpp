#include "Control.h"

void Control::setSize(SizeParams newSize) {
    this->size = newSize;
    SetWindowPos(this->hWnd, HWND_TOP, this->size.x, this->size.y, this->size.width, this->size.height, NULL);
}

SizeParams Control::getSize() {
    return this->size;
}

HWND Control::getHWnd() {
    return this->hWnd;
}