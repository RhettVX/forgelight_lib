@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
mkdir build
pushd build
cl -Fe:example.exe -Zi -DFL_WIN32 -DFL_DEBUG ..\src\example.c ..\src\forgelight_lib\*.c ..\src\external\*.c user32.lib kernel32.lib
rem cl -FC -O2 -Zi -DFL_WIN32 ..\src\example.c ..\src\forgelight_lib\endian_io.c ..\src\forgelight_lib\strings.c ..\src\forgelight_lib\fl_pack2.c ..\src\forgelight_lib\os_win32.c ..\src\forgelight_lib\debug.c ..\src\external\crc64.c ..\src\external\miniz.c
popd
