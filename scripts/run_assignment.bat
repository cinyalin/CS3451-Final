@echo off

set assignment=%1
if "%~1"=="" goto :error
set proj_name=%assignment%

cmake --build ./build --config Release --target %proj_name%
if not %ERRORLEVEL% == 0 goto :error

cd assignments\%assignment%
..\..\build\assignments\%assignment%\Release\%proj_name%.exe
cd ..\..\

if not %ERRORLEVEL% == 0 goto :error

goto :endofscript

:error
echo =========================
echo There was an error!
echo =========================

:endofscript
