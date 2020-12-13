#include "Encoder.h"

string Encoder::byteEncode(string sourceStr) {
    string res;
    for (char item : sourceStr) {
        for (int j = 8; j--;) {
            res.append((1 << j) & uint8_t(item) ? "1" : "0");
        }
    }
    return res;
}

string Encoder::byteStreamToBitStream(arrayUInt byteStream) {
    string bitStream = "";
    for (uint byte : byteStream) {
        bitset<8> bs(byte);
        bitStream += bs.to_string();
    }
    return bitStream;
}