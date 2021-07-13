@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
mkdir build
pushd build
cl -FC -Zi ..\src\example.c ..\src\forgelight_lib\endian_io.c ..\src\forgelight_lib\strings.c ..\src\forgelight_lib\fl_pack2.c ..\src\forgelight_lib\os_win32.c ..\src\external\crc64.c ..\src\external\miniz.c
popd
