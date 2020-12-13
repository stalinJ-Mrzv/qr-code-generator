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

    vector<vector<int>> generate();

    void setSourceText(string sourceText);
    string getSourceText();

    void setCorrectionLevel(char correctionLevel);
    char getCorrectionLevel();

    void setCodeType(char codeType);
    char getCodeType();
    vector<vector<int>> generateAllAlignmentCoords(int version, vector<int> alignments);
private:
    ConstantsForQr consts;

    char correctionLevel = 'H'; // L(7%), M(15%), Q(25%), H(30%)
    vector<char> correctionLevels = {'L', 'M', 'Q', 'H'};

    char codeType = 'B'; // N - numerical, A - alphanumerical, B - bytes
    vector<char> codeTypes = {'N', 'A', 'B'};

    string sourceText;
    string byteEncode(string str);
    string generateStrToBits();

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

    vector<vector<int>> createQrCodeBase(int version);
    vector<vector<int>> fillSearchPatterns(vector<vector<int>> qrcode);
    vector<vector<int>> drawRect(vector<vector<int>> qrcode, int x_start, int y_start, int x_end, int y_end, int value);
    vector<vector<int>> fillAlignPatterns(vector<vector<int>> qrcode, vector<vector<int>> alignmentCoords);
    vector<vector<int>> fillSynchLines(vector<vector<int>> qrcode);
    vector<vector<int>> fillVersionCode(vector<vector<int>> qrcode, int version);
    vector<vector<int>> fillMaskAndCorrectionLevelCode(vector<vector<int>> qrcode);
};


#endif //QRCODEGEN_QRCODER_H
