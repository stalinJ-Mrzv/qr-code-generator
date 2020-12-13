#include "Utils.h"

string Utils::getCodeTypeInBits(int codeType) {
    string codeTypeInBits;
    switch (codeType) {
        case Encoder::codeTypes::N:
            codeTypeInBits = "0001";
            break;
        case Encoder::codeTypes::A:
            codeTypeInBits = "0010";
            break;
        case Encoder::codeTypes::B:
            codeTypeInBits = "0100";
            break;
    }
    return codeTypeInBits;
}

arrayUInt Utils::getVersionSizes(int correctionLevel) {
    matrixUInt versions = this->consts.versionSizesByVersionAndCorrectionLevel;
    arrayUInt versionSizes = versions[correctionLevel];
    return versionSizes;
}

uint Utils::calcVersion(arrayUInt versionSizes, uint length) {
    uint version = 0;
    while (length > versionSizes[version]) {
        version++;
    }
    return version;
}

uint Utils::getAmOfBitsForLen(uint version, int codeType) {
    matrixUInt amounts = this->consts.amountOfBitsByVersionAndCodeType;
    uint amount = amounts[codeType][version];
    return amount;
}

string Utils::getBitsByNumber(uint num, uint amOfBits) {
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

string Utils::extendStringWithZeros(string str) {
    while (str.length() % 8 != 0) {
        str += "0";
    }
    return str;
}

string Utils::extendStringWithBytes(string str, uint requiredSize) {
    vector<string> alternatedBytes = {"11101100", "00010001"};
    uint iters = (requiredSize - str.length()) / 8;
    for (int i = 0; i < iters; i++) {
        str += alternatedBytes[i % alternatedBytes.size()];
    }
    return str;
}

matrixUInt Utils::splitDataOnBlocks(string str, uint version, int correctionLevel) {
    uint amOfBlocks = this->consts.amountOfBlocksByVersionAndCorrectionLevel[correctionLevel][version];
    matrixUInt dataBlocks(amOfBlocks);
    uint amOfBytesInBlocksDiv = (this->consts.versionSizesByVersionAndCorrectionLevel[correctionLevel][version] / 8) / amOfBlocks;
    uint amOfBytesInBlocksMod = (this->consts.versionSizesByVersionAndCorrectionLevel[correctionLevel][version] / 8) % amOfBlocks;

    arrayUInt bytesInBlocks(amOfBlocks);
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
        arrayUInt block;
        for (int j = 0; j < bytesInBlocks[i]; j++) {
            tmpBitString = str.substr(j * 8, 8);
            bitset<8> b(tmpBitString);
            block.push_back(b.to_ulong() & 0xFF);
        }
        dataBlocks[i] = block;
    }
    return dataBlocks;
}

arrayUInt Utils::mergeBlocksAndCorrections(matrixUInt blocks, matrixUInt corrections) {
    arrayUInt mergedBlocks;
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