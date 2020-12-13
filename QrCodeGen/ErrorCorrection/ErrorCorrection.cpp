#include "ErrorCorrection.h"

matrixUInt ErrorCorrection::createCorrectionBytes(matrixUInt dataBlocks, uint version, uint correctionLevel) {
    matrixUInt correctionBytes;
    uint amOfCorrectionBytes = this->consts.amountOfCorrectionBytesPerBlock[correctionLevel][version];
    arrayUInt polynoms = this->consts.genPolynoms[amOfCorrectionBytes];
    arrayUChar galoisField = this->generateGaloisField(256);
    for (arrayUInt block : dataBlocks) {
        arrayUInt preparedArray = this->prepareArray(block, amOfCorrectionBytes);
        for (uint m = 0; m < block.size(); m ++) {
            uint A = preparedArray[0];
            preparedArray.erase(preparedArray.begin());
            preparedArray.push_back(0);
            if (A == 0) {
                continue;
            }
            uint B = this->consts.invGaloisField[A];
            for (uint i = 0; i < amOfCorrectionBytes; i++) {
                uint C = polynoms[i] + B;
                if (C > 254) {
                    C %= 255;
                }
                // cout << preparedArray[i] << ' ' << galoisField[C] << endl;
                preparedArray[i] ^= galoisField[C];
            }
        }
        arrayUInt newCorrectionBytes(preparedArray.begin(), preparedArray.begin() + amOfCorrectionBytes);
        correctionBytes.push_back(newCorrectionBytes);
    }
    return correctionBytes;
}

arrayUChar ErrorCorrection::generateGaloisField(uint size) {
    arrayUChar galoisField(size);
    galoisField[0] = 1;
    uint xorBase = 0b00011101;
    for (uint i = 1; i < size; i++) {
        galoisField[i] = galoisField[i - 1] << 1u;
        if ((galoisField[i - 1] >> 7u) == 1) {
            galoisField[i] ^= xorBase;
        }
    }
    return galoisField;
}

arrayUInt ErrorCorrection::prepareArray(arrayUInt array, uint amOfCorrectionBytes) {
    arrayUInt preparedArray;
    uint max = array.size() > amOfCorrectionBytes ? array.size() : amOfCorrectionBytes;
    for (uint elem : array) {
        preparedArray.push_back(elem);
    }

    while (preparedArray.size() < max) {
        preparedArray.push_back(0);
    }
    return preparedArray;
}