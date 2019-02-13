@echo off

mkdir ..\build
pushd ..\build
cl /I ..\thirdParty\include -FC -Zi ..\src\windows_main.cpp User32.lib Gdi32.lib Opengl32.lib /DGLEW_STATIC ..\thirdParty\lib\glew32s.lib
popd