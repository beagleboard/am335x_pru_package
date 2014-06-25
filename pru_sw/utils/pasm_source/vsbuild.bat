@echo off
setlocal
set failed=0
echo Rebuilding pasm.exe...
REM This is assuming Visual Studio 2008, modify it as required for your system
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\Tools\vsvars32.bat"
devenv pasm.vcproj /rebuild "Release|Win32"
if not %ERRORLEVEL%==0 (
    set failed=true
    echo.
    echo Unable to build pasm.exe!
)
REM Visual Studio always puts its program database alongside the output
if exist ..\pasm.pdb del ..\pasm.pdb
exit /b %failed%
