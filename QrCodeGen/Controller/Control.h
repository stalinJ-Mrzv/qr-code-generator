#ifndef QRCODEGEN_CONTROL_H
#define QRCODEGEN_CONTROL_H

#include <Windows.h>

#include "../Types.h"

using namespace Types;
class Control {
public:
    virtual HWND create(HWND hWndParent) = 0;
    void setSize(SizeParams newSize);
    SizeParams getSize();

    HWND getHWnd();

protected:
    HWND hWnd;
    HWND hWndParent;
    SizeParams size = {0, 0, 0, 0};
};


#endif //QRCODEGEN_CONTROL_H
