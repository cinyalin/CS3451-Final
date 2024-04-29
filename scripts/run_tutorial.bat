@echo off

set tutorial=%1
if "%~1"=="" goto :error
set proj_name=%tutorial%

cmake --build ./build --config Release --target %proj_name%
if not %ERRORLEVEL% == 0 goto :error

cd tutorials\%tutorial%
..\..\build\tutorials\%tutorial%\Release\%proj_name%.exe %test%
cd ..\..\
if not %ERRORLEVEL% == 0 goto :error

goto :endofscript

:error
echo =========================
echo There was an error!
echo =========================

:endofscript
