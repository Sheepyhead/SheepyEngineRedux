@echo off

mkdir ..\build
pushd ..\build
cl -FC -Zi ..\src\windows_main.cpp
popd