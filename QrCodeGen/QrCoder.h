#ifndef QRCODEGEN_QRCODER_H
#define QRCODEGEN_QRCODER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <windows.h>

#include "consts.h"


using namespace std;

class QrCoder {
public:
    typedef unsigned char byte;

    QrCoder();
    QrCoder(string sourceText);

    string generate();
    string generateStrToBits();

    void setSourceText(string sourceText);
    string getSourceText();

    void setCorrectionLevel(char correctionLevel);
    char getCorrectionLevel();

    void setCodeType(char codeType);
    char getCodeType();

private:
    ConstantsForQr consts;

    char correctionLevel = 'H'; // L(7%), M(15%), Q(25%), H(30%)
    vector<char> correctionLevels = {'L', 'M', 'Q', 'H'};

    char codeType = 'B'; // N - numerical, A - alphanumerical, B - bytes
    vector<char> codeTypes = {'N', 'A', 'B'};

    string sourceText;
    string byteEncode(string str);

    vector<int> getVersionSizes();
    string getCodeTypeInBits();

    int getIndexOfCorrectionLevel();
    int getIndexOfCodeType();

    int calcVersion(vector<int> versionSizes, unsigned int length);
    int getAmOfBitsForLen(int version);

    string getBitsByNumber(unsigned int num, int amOfBits);
    string extendStringWithZeros(string str);
    string extendStringWithBytes(string str, unsigned int requiredSize);

    vector<vector<byte>> splitDataOnBlocks(string str, int version);
    vector<byte> generateGaloisField(int size);
    vector<byte> prepareArray(vector<byte> array, int amOfCorrectionBytes);
    vector<vector<byte>> createCorrectionBytes(vector<vector<byte>> dataBlocks, int version);
    vector<byte> mergeBlocksAndCorrections(vector<vector<byte>> blocks, vector<vector<byte>> corrections);
};


#endif //QRCODEGEN_QRCODER_H
