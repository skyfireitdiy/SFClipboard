@echo off
set /p QMAKE_DIR=qmake dir：
set /p GCC_DIR=g++ dir:
set path=%GCC_DIR%;%QMAKE_DIR%;%PATH%
qmake -makefile ..
make
pause