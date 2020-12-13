#include "ContentFiller.h"

matrixUInt ContentFiller::fillContent(uint version, arrayUInt data, int correctionLevel) {
    uint size = 21;
    arrayUInt alignments = this->consts.alignPatterns[version];
    if (version >= 1) {
        size = alignments[alignments.size() - 1] + 7;
    }
    matrixInt qrcode(size, arrayInt(size, -1));
    qrcode = this->fillSearchPatterns(qrcode);
    matrixUInt alignmentCoords = this->generateAllAlignmentCoords(version, alignments);
    if (version >= 1) {
        qrcode = this->fillAlignPatterns(qrcode, alignmentCoords);
    }
    qrcode = this->fillSynchLines(qrcode);
    if (version >= 6) {
        qrcode = this->fillVersionCode(qrcode, version);
    }
    qrcode = this->fillMaskAndCorrectionLevelCode(qrcode, correctionLevel);
    matrixUInt filledQrCode = this->fillData(qrcode, data, version);
    filledQrCode = this->addIndent(filledQrCode);
    return filledQrCode;
}

matrixInt ContentFiller::drawRect(matrixInt qrcode, uint x_start, uint y_start, uint x_end, uint y_end, int value) {
    for (int i = x_start; i <= x_end; i++) {
        qrcode[y_start][i] = value;
        qrcode[y_end][i] = value;
    }

    for (int i = y_start; i <= y_end; i++) {
        qrcode[i][x_start] = value;
        qrcode[i][x_end] = value;
    }

    return qrcode;
}

matrixInt ContentFiller::fillSearchPatterns(matrixInt qrcode) {
    // left top
    qrcode = this->drawRect(qrcode, 0, 0, 6, 6, 1);
    qrcode = this->drawRect(qrcode, 2, 2, 4, 4, 1);
    qrcode = this->drawRect(qrcode, 3, 3, 3, 3, 1);
    qrcode = this->drawRect(qrcode, 1, 1, 5, 5, 0);
    qrcode = this->drawRect(qrcode, 0, 7, 7, 7, 0);
    qrcode = this->drawRect(qrcode, 7, 0, 7, 7, 0);

    // left bottom
    qrcode = this->drawRect(qrcode, 0, qrcode.size() - 7, 6, qrcode.size() - 1, 1);
    qrcode = this->drawRect(qrcode, 2, qrcode.size() - 5, 4, qrcode.size() - 3, 1);
    qrcode = this->drawRect(qrcode, 3, qrcode.size() - 4, 3, 3, 1);
    qrcode = this->drawRect(qrcode, 1, qrcode.size() - 6, 5, qrcode.size() - 2, 0);
    qrcode = this->drawRect(qrcode, 0, qrcode.size() - 8, 7, qrcode.size() - 8, 0);
    qrcode = this->drawRect(qrcode, 7, qrcode.size() - 8, 7, qrcode.size() - 1, 0);

    // right top
    qrcode = this->drawRect(qrcode, qrcode.size() - 7, 0, qrcode.size() - 1, 6, 1);
    qrcode = this->drawRect(qrcode, qrcode.size() - 5, 2, qrcode.size() - 3, 4, 1);
    qrcode = this->drawRect(qrcode, qrcode.size() - 4, 3, 3, 3, 1);
    qrcode = this->drawRect(qrcode, qrcode.size() - 6, 1, qrcode.size() - 2, 5, 0);
    qrcode = this->drawRect(qrcode, qrcode.size() - 8, 0, qrcode.size() - 8, 7, 0);
    qrcode = this->drawRect(qrcode, qrcode.size() - 8, 7, qrcode.size() - 1, 7, 0);

    return qrcode;
}

matrixUInt ContentFiller::generateAllAlignmentCoords(uint version, arrayUInt alignments) {
    matrixUInt alignmentCoords;
    arrayUInt tempCoords(2);
    for (int coord1: alignments) {
        tempCoords[0] = coord1;
        for (int coord2: alignments) {
            tempCoords[1] = coord2;
            alignmentCoords.push_back(tempCoords);
        }
    }
    if (version > 5) {
        for (int i = 0; i < alignmentCoords.size(); i++) {
            if ((alignmentCoords[i][0] == alignments[0] & alignmentCoords[i][1] == alignments[0]) ||
                (alignmentCoords[i][0] == alignments[0] & alignmentCoords[i][1] == alignments[alignments.size() - 1]) ||
                (alignmentCoords[i][0] == alignments[alignments.size() - 1] & alignmentCoords[i][1] == alignments[0])) {
                alignmentCoords.erase(alignmentCoords.begin() + i);
            }
        }
    }
    return alignmentCoords;
}

matrixInt ContentFiller::fillAlignPatterns(matrixInt qrcode, matrixUInt alignmentCoords) {
    for (arrayUInt coord: alignmentCoords) {
        qrcode = this->drawRect(qrcode, coord[0], coord[1], coord[0], coord[1], 1);
        qrcode = this->drawRect(qrcode, coord[0] - 2, coord[1] - 2, coord[0] + 2, coord[1] + 2, 1);
        qrcode = this->drawRect(qrcode, coord[0] - 1, coord[1] - 1, coord[0] + 1, coord[1] + 1, 0);
    }

    return qrcode;
}

matrixInt ContentFiller::fillSynchLines(matrixInt qrcode) {
    uint coord1 = 6;
    uint coord2 = 8;
    uint cell = 1;
    while (coord2 <= qrcode.size() - 9) {
        qrcode[coord1][coord2] = cell;
        qrcode[coord2][coord1] = cell;
        cell = !cell;
        coord2++;
    }
    return qrcode;
}

matrixInt ContentFiller::fillVersionCode(matrixInt qrcode, uint version) {
    matrixUInt codeVersion = this->consts.versionCodes[version];
    uint start_pos = qrcode.size() - 11;
    uint m = 0;
    for (int i = start_pos; i < start_pos + 3; i++) {
        for (int j = 0; j < codeVersion[0].size(); j++) {
            qrcode[i][j] = codeVersion[m][j];
            qrcode[j][i] = codeVersion[m][j];
        }
        m++;
    }
    return qrcode;
}

matrixInt ContentFiller::fillMaskAndCorrectionLevelCode(matrixInt qrcode, int correctionLevel) {
    arrayUInt maskAndCorrectionLevelCode = this->consts.maskCodesByCorrectionLevel[correctionLevel];
    int m = 0;
    for (int i = 0; i < 6; i++) {
        qrcode[8][i] = maskAndCorrectionLevelCode[m];
        m++;
    }

    for (int i = 7; i <= 8; i++) {
        qrcode[8][i] = maskAndCorrectionLevelCode[m];
        m++;
    }

    qrcode[7][8] = maskAndCorrectionLevelCode[m];
    m++;

    for (int i = 5; i >= 0; i--) {
        qrcode[i][8] = maskAndCorrectionLevelCode[m];
        m++;
    }

    m = 0;
    for (int i = 0; i < 7; i++) {
        qrcode[qrcode.size() - 1 - i][8] = maskAndCorrectionLevelCode[m];
        m++;
    }
    qrcode[qrcode.size() - 1 - 7][8] = 1;

    for (int i = 7; i >= 0; i--) {
        qrcode[8][qrcode.size() - 1 - i] = maskAndCorrectionLevelCode[m];
        m++;
    }

    return qrcode;
}

bool ContentFiller::isReserved(int element) {
    return element != -1;
}

bool ContentFiller::isNeedToReverse(uint row, uint col, uint size) {
    if (col >= size - 7 && row < 9) {
        return true;
    }
    if (row > size - 1) {
        return true;
    }
    return false;
}

matrixUInt ContentFiller::fillData(matrixInt qrcode, arrayUInt data, uint version) {
    string bitData = this->encoder.byteStreamToBitStream(data);
    uint size = qrcode.size();
    Point origin(size - 1, size - 1);
    Direction direction = Direction::UpFirstCol;
    uint moduleIndex = 0;
    while (true) {
        if (origin.x == 6) {
            origin.x--;
        }

        if (!isReserved(qrcode[origin.y][origin.x])) {
            if (moduleIndex > bitData.length()) {
                break;
            }
            qrcode[origin.y][origin.x] = bitData[moduleIndex++] == '1' ? 1 : 0;
            if (((origin.y + origin.x) % 2) == 0) {
                qrcode[origin.y][origin.x] = !qrcode[origin.y][origin.x];
            }
        }

        switch (direction) {
            case UpFirstCol:
                origin.x--;
                direction = Direction::UpSecondCol;
                break;

            case UpSecondCol:
                origin.x++;
                origin.y--;
                direction = Direction::UpFirstCol;

                if (isNeedToReverse(origin.y, origin.x, size)) {
                    origin.x -= 2;
                    origin.y++;
                    direction = Direction::DownFirstCol;
                }
                break;

            case DownFirstCol:
                origin.x--;
                direction = Direction::DownSecondCol;
                break;

            case DownSecondCol:
                direction = Direction::DownFirstCol;
                origin.x++;
                origin.y++;

                if (isNeedToReverse(origin.y, origin.x, size)) {
                    origin.y--;
                    origin.x -= 2;
                    direction = Direction::UpFirstCol;
                }
                break;
        }
    }
    matrixUInt filledQrCode(size, arrayUInt(size));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (qrcode[i][j] == -1) {
                qrcode[i][j] = 0;
            }
            filledQrCode[i][j] = qrcode[i][j];
        }
    }
    return filledQrCode;
}

matrixUInt ContentFiller::addIndent(matrixUInt qrcode) {
    matrixUInt qrCodeWithIndent(qrcode.size() + (4 * 2), arrayUInt(qrcode.size() + (4 * 2), 0));

    for (int i = 4; i < qrCodeWithIndent.size() - 4; i++) {
        for (int j = 4; j < qrCodeWithIndent[0].size() - 4; j++) {
            qrCodeWithIndent[i][j] = qrcode[i - 4][j - 4];
        }
    }

    cout << qrCodeWithIndent.size() << endl;
    return qrCodeWithIndent;
}
