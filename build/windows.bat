@echo off
set /p QMAKE_DIR=qmake文件夹(默认为空)：
set /p GCC_DIR=g++文件夹(默认为空):
set path=%GCC_DIR%;%QMAKE_DIR%;%PATH%
qmake -makefile ..
make
pause