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