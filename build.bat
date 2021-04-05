@echo off

call "D:\WindowsPrograms\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
mkdir build
pushd build
rem TODO(rhett): Don't know if I need to do this. Likely temporary. Maybe even a bad idea.
rem TODO(rhett): Evaluate if I should stick external code in seperate translation units
rem cl /c -FC -Zi ..\src\external\lookup2.c
rem cl /c -FC -Zi ..\src\external\crc64.c
rem cl -FC -Zi ..\example.c lookup2.obj crc64.obj
cl -FC -Zi ..\example.c
popd