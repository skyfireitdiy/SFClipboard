@echo off
set /p QMAKE_DIR=qmake�ļ���(Ĭ��Ϊ��)��
set /p GCC_DIR=g++�ļ���(Ĭ��Ϊ��):
set path=%GCC_DIR%;%QMAKE_DIR%;%PATH%
qmake -makefile ..
make
pause