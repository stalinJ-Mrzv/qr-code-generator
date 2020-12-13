#ifndef QRCODEGEN_CONTENTFILLER_H
#define QRCODEGEN_CONTENTFILLER_H

#include "../consts.h"
#include "../Types.h"
#include "../Encoding/Encoder.h"

using namespace std;
using namespace Types;

class ContentFiller {
public:
    matrixUInt fillContent(uint version, arrayUInt data, int correctionLevel);

private:
    enum Direction {
        UpFirstCol = 0,
        UpSecondCol = 1,
        DownFirstCol = 3,
        DownSecondCol = 4
    };

    ConstantsForQr consts;
    Encoder encoder;

    matrixInt drawRect(matrixInt qrcode, uint x_start, uint y_start, uint x_end, uint y_end, int value);

    matrixInt fillSearchPatterns(matrixInt qrcode);

    matrixUInt generateAllAlignmentCoords(uint version, arrayUInt alignments);
    matrixInt fillAlignPatterns(matrixInt qrcode, matrixUInt alignmentCoords);
    matrixInt fillSynchLines(matrixInt qrcode);
    matrixInt fillVersionCode(matrixInt qrcode, uint version);
    matrixInt fillMaskAndCorrectionLevelCode(matrixInt qrcode, int correctionLevel);

    bool isReserved(int element);
    bool isNeedToReverse(uint row, uint col, uint size);
    matrixUInt fillData(matrixInt qrcode, arrayUInt data, uint version);
    matrixUInt addIndent(matrixUInt qrcode);
};

class Point {
public:
    uint x;
    uint y;

    Point(uint x, uint y) {
        this->x = x;
        this->y = y;
    }
};

#endif //QRCODEGEN_CONTENTFILLER_H
