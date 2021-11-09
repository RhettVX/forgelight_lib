@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

setlocal EnableDelayedExpansion
setlocal EnableExtensions

mkdir build
pushd build

set OUTPUT_NAME=forgelight_lib.lib
set SOURCES=..\src\*.c
set FL_DEFINES=/DFL_WIN32
set COMPILATION_FLAGS=/Od /Zi /W3

cl /nologo /c !FL_DEFINES! /I"../src/" /I"../src/external/" !COMPILATION_FLAGS! !SOURCES!
lib /nologo /OUT:!OUTPUT_NAME! *.obj
rem copy ..\src\forgelight_lib.h .

popd
