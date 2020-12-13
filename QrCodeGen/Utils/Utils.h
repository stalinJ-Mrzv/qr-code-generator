#ifndef QRCODEGEN_UTILS_H
#define QRCODEGEN_UTILS_H

#include <string>
#include <bitset>

#include "../Types.h"
#include "../consts.h"
#include "../Encoding/Encoder.h"

using namespace std;
using namespace Types;

class Utils {
public:
    string getCodeTypeInBits(int codeType);
    arrayUInt getVersionSizes(int correctionLevel);

    uint calcVersion(arrayUInt versionSizes, uint length);
    uint getAmOfBitsForLen(uint version, int codeType);

    string getBitsByNumber(uint num, uint amOfBits);
    string extendStringWithZeros(string str);
    string extendStringWithBytes(string str, uint requiredSize);

    matrixUInt splitDataOnBlocks(string str, uint version, int correctionLevel);
    arrayUInt mergeBlocksAndCorrections(matrixUInt blocks, matrixUInt corrections);

private:
    ConstantsForQr consts;
};


#endif //QRCODEGEN_UTILS_H
