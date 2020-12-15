#ifndef QRCODEGEN_RADIOBUTTONCONTROL_H
#define QRCODEGEN_RADIOBUTTONCONTROL_H


#include <Windows.h>
#include <tchar.h>

#include "../../Types.h"
#include "../../QrCoder.h"
#include "../Control.h"

using namespace Types;

class RadioButtonControl: public Control {
public:
    HWND create(HWND hWndParent);
    HWND create(HWND hWndParent, string name, int id);
    void setId(int id);
    int getId();

private:
    int id = 0;
};


#endif //QRCODEGEN_RADIOBUTTONCONTROL_H
