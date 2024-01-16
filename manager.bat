@echo off

rem
:build
mkdir build
cd build
cmake ..
make
goto :eof

rem
:clean
rmdir /s /q build
goto :eof

rem
:run
cd build
start e-gradebook.exe
goto :eof

rem
if "%1"=="run" goto run
if "%1"=="build" goto build
if "%1"=="clean" goto clean

echo Usage: %0 {run|build|clean}
