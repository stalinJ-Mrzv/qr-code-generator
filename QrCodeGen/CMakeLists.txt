cmake_minimum_required(VERSION 3.15)
project(QrCodeGen)

set(CMAKE_CXX_STANDARD 20)

add_executable(QrCodeGen QrCodeGenerator.cpp QrCoder.cpp QrCoder.h consts.h Encoding/Encoder.cpp Encoding/Encoder.h ErrorCorrection/ErrorCorrection.cpp ErrorCorrection/ErrorCorrection.h Utils/Utils.cpp Utils/Utils.h Types.h ContentFiller/ContentFiller.cpp ContentFiller/ContentFiller.h Controller/Edit/EditControl.cpp Controller/Edit/EditControl.h Controller/Qr/QrControl.cpp Controller/Qr/QrControl.h Controller/Control.cpp Controller/Control.h)