#include "QrCoder.h"

QrCoder::QrCoder() {}

QrCoder::QrCoder(string sourceText) {
    this->sourceText = sourceText;
}

string QrCoder::generate() {
    string generatedStrBits = this->generateStrToBits();
    string codeTypeInBits = this->getCodeTypeInBits();

    vector<int> versionSizes = this->getVersionSizes();
    int version;
    int amOfBitsForLen;
    unsigned int versionSize;
    unsigned int len = generatedStrBits.length() + codeTypeInBits.length();
    do {
        version = this->calcVersion(versionSizes, len);
        amOfBitsForLen = this->getAmOfBitsForLen(version);
        versionSize = versionSizes[version];
    } while (len + amOfBitsForLen > versionSize);


    string lenOfSourceTextInBits = this->getBitsByNumber(generatedStrBits.length() / 8, amOfBitsForLen);

    string generatedString = codeTypeInBits + lenOfSourceTextInBits + generatedStrBits;
    generatedString = this->extendStringWithZeros(generatedString);
    generatedString = this->extendStringWithBytes(generatedString, versionSize);
    vector<vector<byte>> splittedBlocks = this->splitDataOnBlocks(generatedString, version);
    vector<vector<byte>> correctionBytes = this->createCorrectionBytes(splittedBlocks, version);
    vector<byte> mergedBlocks = this->mergeBlocksAndCorrections(splittedBlocks, correctionBytes);
    return generatedString;
}

vector<::byte> QrCoder::mergeBlocksAndCorrections(vector<vector<byte>> blocks, vector<vector<byte>> corrections) {
    vector<::byte> mergedBlocks;
    while (!blocks.empty()) {
        for (int i = 0; i < blocks.size(); i++) {
            if (blocks[i].empty()) {
                blocks.erase(blocks.begin() + i);
                continue;
            }
            mergedBlocks.push_back(blocks[i][0]);
            blocks[i].erase(blocks[i].begin());
        }
    }

    while (!corrections.empty()) {
        for (int i = 0; i < corrections.size(); i++) {
            if (corrections[i].empty()) {
                corrections.erase(corrections.begin() + i);
                continue;
            }
            mergedBlocks.push_back(corrections[i][0]);
            corrections[i].erase(corrections[i].begin());
        }
    }
    return mergedBlocks;
}

vector<::byte> QrCoder::generateGaloisField(int size) {
    vector<::byte> galoisField(size);
    galoisField[0] = 1;
    ::byte xorBase = 0b00011101;
    for (int i = 1; i < size; i++) {
        galoisField[i] = galoisField[i - 1] << 1;
        if ((galoisField[i - 1] >> 7) == 1) {
            galoisField[i] = galoisField[i] ^ xorBase;
        }
    }
    return galoisField;
}

vector<::byte> QrCoder::prepareArray(vector<byte> array, int amOfCorrectionBytes) {
    vector<byte> preparedArray;
    int max = array.size() > amOfCorrectionBytes ? array.size() : amOfCorrectionBytes;
    for (byte elem : array) {
        preparedArray.push_back(elem);
    }

    while (preparedArray.size() < max) {
        preparedArray.push_back(0);
    }
    return preparedArray;
}

vector<vector<::byte>> QrCoder::createCorrectionBytes(vector<vector<byte>> dataBlocks, int version) {
    vector<vector<byte>> correctionBytes;
    int amOfCorrectionBytes = this->consts.amountOfCorrectionBytesPerBlock[this->getIndexOfCorrectionLevel()][version];
    vector<int> polynoms = this->consts.genPolynoms[amOfCorrectionBytes];
    vector<byte> galoisField = this->generateGaloisField(256);
    for (vector<byte> block : dataBlocks) {
        vector<byte> preparedArray = this->prepareArray(block, amOfCorrectionBytes);
        for (int m = 0; m < block.size(); m ++) {
            byte A = preparedArray[0];
            preparedArray.erase(preparedArray.begin());
            preparedArray.push_back(0);
            if (A == 0) {
                continue;
            }
            int B = this->consts.invGaloisField[A];
            for (int i = 0; i < amOfCorrectionBytes; i++) {
                int C = polynoms[i] + B;
                if (C > 254) {
                    C %= 255;
                }
                preparedArray[i] ^= galoisField[C];
            }
        }
        vector<byte> newCorrectionBytes(preparedArray.begin(), preparedArray.begin() + amOfCorrectionBytes);
        correctionBytes.push_back(newCorrectionBytes);
    }
    return correctionBytes;
}

vector<vector<::byte>> QrCoder::splitDataOnBlocks(string str, int version) {
    int amOfBlocks = this->consts.amountOfBlocksByVersionAndCorrectionLevel[this->getIndexOfCorrectionLevel()][version];
    vector<vector<byte>> dataBlocks(amOfBlocks);
    int amOfBytesInBlocksDiv = (this->consts.versionSizesByVersionAndCorrectionLevel[this->getIndexOfCorrectionLevel()][version] / 8) / amOfBlocks;
    int amOfBytesInBlocksMod = (this->consts.versionSizesByVersionAndCorrectionLevel[this->getIndexOfCorrectionLevel()][version] / 8) % amOfBlocks;

    vector<int> bytesInBlocks(amOfBlocks);
    for (int i = 0; i < amOfBlocks; i++) {
        bytesInBlocks[i] = amOfBytesInBlocksDiv;
    }
    if (amOfBytesInBlocksMod >= 1) {
        for (int i = 0; i < amOfBytesInBlocksMod; i++) {
            bytesInBlocks[(amOfBlocks - 1) - i] += 1;
        }
    }

    string tmpBitString;
    for (int i = 0; i < amOfBlocks; i++) {
        vector<byte> block;
        for (int j = 0; j < bytesInBlocks[i]; j++) {
            tmpBitString = str.substr(j * 8, 8);
            bitset<8> b(tmpBitString);
            block.push_back(b.to_ulong() & 0xFF);
        }
        dataBlocks[i] = block;
    }
    return dataBlocks;
}

string QrCoder::extendStringWithBytes(string str, unsigned int requiredSize) {
    vector<string> alternatedBytes = {"11101100", "00010001"};
    int iters = (requiredSize - str.length()) / 8;
    for (int i = 0; i < iters; i++) {
        str += alternatedBytes[i % alternatedBytes.size()];
    }
    return str;
}

string QrCoder::extendStringWithZeros(string str) {
    while (str.length() % 8 != 0) {
        str += "0";
    }
    return str;
}

string QrCoder::getBitsByNumber(unsigned int num, int amOfBits) {
    string bits;
    while (num != 0) {
        bits.insert(0, (num % 2 == 0 ? "0" : "1"));
        num /= 2;
    }
    while (bits.length() < amOfBits) {
        bits.insert(0, "0");
    }

    return bits;
}

int QrCoder::calcVersion(vector<int> versionSizes, unsigned int length) {
    int version = 0;
    while (length > versionSizes[version]) {
        version++;
    }
    return version;
}

int QrCoder::getIndexOfCorrectionLevel() {
    int index;
    auto it = find(this->correctionLevels.begin(), this->correctionLevels.end(), this->correctionLevel);
    if (it != this->correctionLevels.end()) {
        index = it - this->correctionLevels.begin();
    } else {
        index = -1;
    }
    return index;
}

int QrCoder::getIndexOfCodeType() {
    int index;
    auto it = find(this->codeTypes.begin(), this->codeTypes.end(), this->codeType);
    if (it != this->codeTypes.end()) {
        index = it - this->codeTypes.begin();
    } else {
        index = -1;
    }
    return index;
}

int QrCoder::getAmOfBitsForLen(int version) {
    vector<vector<int>> amounts = this->consts.amountOfBitsByVersionAndCodeType;
    int amount = amounts[this->getIndexOfCodeType()][version];
    return amount;
}

string QrCoder::generateStrToBits() {
    string sourceText = this->sourceText;
    return this->byteEncode(sourceText);
}

string QrCoder::byteEncode(string sourceStr) {
    string res;
    for (char item : sourceStr) {
        for (int j = 8; j--;) {
            res.append((1 << j) & uint8_t(item) ? "1" : "0");
        }
    }
    return res;
}

string QrCoder::getCodeTypeInBits() {
    string codeTypeInBits;
    switch (this->codeType) {
        case 'N':
            codeTypeInBits = "0001";
            break;
        case 'A':
            codeTypeInBits = "0010";
            break;
        case 'B':
            codeTypeInBits = "0100";
            break;
    }
    return codeTypeInBits;
}

vector<int> QrCoder::getVersionSizes() {
    vector<vector<int>> versions = this->consts.versionSizesByVersionAndCorrectionLevel;
    vector<int> versionSizes = versions[this->getIndexOfCorrectionLevel()];
    return versionSizes;
}

void QrCoder::setSourceText(string sourceText) {
    this->sourceText = sourceText;
}

string QrCoder::getSourceText() {
    return this->sourceText;
}

void QrCoder::setCorrectionLevel(char correctionLevel) {
    this->correctionLevel = correctionLevel;
}
char QrCoder::getCorrectionLevel() {
    return this->correctionLevel;
}

void QrCoder::setCodeType(char codeType) {
    this->codeType = codeType;
}
char QrCoder::getCodeType() {
    return this->codeType;
}