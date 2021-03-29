@echo off

call "D:\WindowsPrograms\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
mkdir build
pushd build
cl -FC -Zi ..\src\fl_pack2.c
popd