#ifndef QRCODEGEN_EDITCONTROL_H
#define QRCODEGEN_EDITCONTROL_H

#include <windows.h>
#include <tchar.h>

#include "../../Types.h"
#include "../Control.h"

using namespace Types;

class EditControl: public Control {
public:
    HWND create(HWND hWndParent);
};

#endif //QRCODEGEN_EDITCONTROL_H
