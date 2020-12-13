#ifndef QRCODEGEN_ENCODER_H
#define QRCODEGEN_ENCODER_H

#include <iostream>
#include <bitset>

#include "../Types.h"

using namespace std;
using namespace Types;

class Encoder {
public:
    enum codeTypes {
        // N - numerical, A - alphanumerical, B - bytes
        N = 0,
        A = 1,
        B = 2
    };

    string byteEncode(string str);
    string byteStreamToBitStream(arrayUInt byteStream);
};


#endif //QRCODEGEN_ENCODER_H
