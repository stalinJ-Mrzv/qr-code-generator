cmake_minimum_required(VERSION 3.15)
project(QrCodeGen)

set(CMAKE_CXX_STANDARD 20)

add_executable(QrCodeGen main.cpp QrCoder.cpp QrCoder.h consts.h)