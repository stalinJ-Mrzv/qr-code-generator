#ifndef QRCODEGEN_TYPES_H
#define QRCODEGEN_TYPES_H

#include <vector>

using namespace std;

namespace Types {
    struct SizeParams {
        int width;
        int height;
        int x;
        int y;
    };

    using uint = unsigned int;
    using uchar = unsigned char;
    using matrixInt = vector<vector<int>>;
    using matrixUInt = vector<vector<uint>>;
    using matrixUChar = vector<vector<uchar>>;
    using arrayInt = vector<int>;
    using arrayUInt = vector<uint>;
    using arrayUChar = vector<uchar>;
}
#endif //QRCODEGEN_TYPES_H
