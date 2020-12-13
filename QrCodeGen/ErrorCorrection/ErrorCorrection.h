#ifndef QRCODEGEN_ERRORCORRECTION_H
#define QRCODEGEN_ERRORCORRECTION_H

#include <vector>

#include "../consts.h"
#include "../Types.h"

using namespace std;
using namespace Types;

class ErrorCorrection {
public:
    enum correctionLevels {
        // L(7%), M(15%), Q(25%), H(30%)
        L = 0,
        M = 1,
        Q = 2,
        H = 3
    };
    matrixUInt createCorrectionBytes(matrixUInt dataBlocks, uint version, uint correctionLevel);

private:
    ConstantsForQr consts;
    arrayUChar generateGaloisField(uint size);
    arrayUInt prepareArray(arrayUInt array, uint amOfCorrectionBytes);
};


#endif //QRCODEGEN_ERRORCORRECTION_H
