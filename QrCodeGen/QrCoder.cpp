#include "QrCoder.h"

QrCoder::QrCoder() {}

QrCoder::QrCoder(string sourceText) {
    this->sourceText = sourceText;
}

matrixUInt QrCoder::generate() {
    string generatedStrBits = this->encoder.byteEncode(this->sourceText);
    string codeTypeInBits = this->utils.getCodeTypeInBits(this->codeType);

    arrayUInt versionSizes = this->utils.getVersionSizes(this->correctionLevel);
    uint version;
    uint amOfBitsForLen = 0;
    uint versionSize;
    uint len = generatedStrBits.length() + codeTypeInBits.length();
    do {
        version = this->utils.calcVersion(versionSizes, len + amOfBitsForLen);
        amOfBitsForLen = this->utils.getAmOfBitsForLen(version, this->codeType);
        versionSize = versionSizes[version];
    } while (len + amOfBitsForLen > versionSize);

    string lenOfSourceTextInBits = this->utils.getBitsByNumber(generatedStrBits.length() / 8, amOfBitsForLen);

    string generatedString = codeTypeInBits + lenOfSourceTextInBits + generatedStrBits;
    generatedString = this->utils.extendStringWithZeros(generatedString);
    generatedString = this->utils.extendStringWithBytes(generatedString, versionSize);

    matrixUInt splittedBlocks = this->utils.splitDataOnBlocks(generatedString, version, this->correctionLevel);
    matrixUInt correctionBytes = this->errorCorrection.createCorrectionBytes(splittedBlocks, version, this->correctionLevel);
    arrayUInt mergedBlocks = this->utils.mergeBlocksAndCorrections(splittedBlocks, correctionBytes);

    matrixUInt qrcode = this->contentFiller.fillContent(version, mergedBlocks, this->correctionLevel);

    return qrcode;
}

void QrCoder::setSourceText(string sourceText) {
    this->sourceText = sourceText;
}

string QrCoder::getSourceText() {
    return this->sourceText;
}

void QrCoder::setCorrectionLevel(int correctionLevel) {
    this->correctionLevel = correctionLevel;
}

int QrCoder::getCorrectionLevel() {
    return this->correctionLevel;
}

void QrCoder::setCodeType(int codeType) {
    this->codeType = codeType;
}

int QrCoder::getCodeType() {
    return this->codeType;
}