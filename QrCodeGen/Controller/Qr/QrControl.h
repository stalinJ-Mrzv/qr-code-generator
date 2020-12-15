#ifndef QRCODEGEN_QRCONTROL_H
#define QRCODEGEN_QRCONTROL_H

#include <Windows.h>
#include <tchar.h>

#include "../../Types.h"
#include "../../QrCoder.h"
#include "../Control.h"

using namespace Types;

class QrControl: public Control {
public:
    HWND create(HWND hWndParent);

    matrixUInt generate(string sourceText);
    void drawQrCode();
    void setCorrectionLevel(int correctionLevel);

private:
    QrCoder qrCoder;
    matrixUInt qrcode = {{0}};
};


#endif //QRCODEGEN_QRCONTROL_H
