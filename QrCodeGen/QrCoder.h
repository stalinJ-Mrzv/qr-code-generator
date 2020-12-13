#ifndef QRCODEGEN_QRCODER_H
#define QRCODEGEN_QRCODER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <windows.h>

#include "consts.h"
#include "Encoding/Encoder.h"
#include "ErrorCorrection/ErrorCorrection.h"
#include "Types.h"
#include "Utils/Utils.h"
#include "ContentFiller/ContentFiller.h"

using namespace std;
using namespace Types;

class QrCoder {
public:
    QrCoder();
    QrCoder(string sourceText);

    matrixUInt generate();

    void setSourceText(string sourceText);
    string getSourceText();

    void setCorrectionLevel(int correctionLevel);
    int getCorrectionLevel();

    void setCodeType(int codeType);
    int getCodeType();

private:
    string sourceText;

    ConstantsForQr consts;

    ErrorCorrection errorCorrection;
    int correctionLevel = ErrorCorrection::correctionLevels::H;

    Encoder encoder;
    int codeType = Encoder::codeTypes::B;

    Utils utils;

    ContentFiller contentFiller;
};


#endif //QRCODEGEN_QRCODER_H
