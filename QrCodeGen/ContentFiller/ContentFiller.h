#ifndef QRCODEGEN_CONTENTFILLER_H
#define QRCODEGEN_CONTENTFILLER_H

#include "../consts.h"
#include "../Types.h"

using namespace std;
using namespace Types;

class ContentFiller {
public:
    matrixUInt fillContent(uint version, arrayUInt data, int correctionLevel);

private:
    ConstantsForQr consts;

    matrixInt drawRect(matrixInt qrcode, uint x_start, uint y_start, uint x_end, uint y_end, int value);

    matrixInt fillSearchPatterns(matrixInt qrcode);

    matrixUInt generateAllAlignmentCoords(uint version, arrayUInt alignments);
    matrixInt fillAlignPatterns(matrixInt qrcode, matrixUInt alignmentCoords);
    matrixInt fillSynchLines(matrixInt qrcode);
    matrixInt fillVersionCode(matrixInt qrcode, uint version);
    matrixInt fillMaskAndCorrectionLevelCode(matrixInt qrcode, int correctionLevel);

    matrixUInt fillData(matrixInt qrcode, arrayUInt data);
};


#endif //QRCODEGEN_CONTENTFILLER_H
