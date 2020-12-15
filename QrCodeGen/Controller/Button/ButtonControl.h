#ifndef QRCODEGEN_BUTTONCONTROL_H
#define QRCODEGEN_BUTTONCONTROL_H

#include <windows.h>
#include <tchar.h>

#include "../../Types.h"
#include "../Control.h"

using namespace Types;

class ButtonControl: public Control {
public:
    HWND create(HWND hWndParent);
};



#endif //QRCODEGEN_BUTTONCONTROL_H
