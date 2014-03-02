@echo off
setlocal
set failed=0
echo Rebuilding path_utils.exe...
REM This is assuming Visual Studio 2008, modify it as required for your system
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\Tools\vsvars32.bat"
devenv path_utils_test.vcproj /rebuild "Release|Win32"
if not %ERRORLEVEL%==0 (
    set failed=1
    echo.
    echo Unable to build path_utils.exe!  Aborting test...
    goto :done
)

Release\path_utils.exe
if %errorlevel%==0 (
    echo Passed!
    goto done
)
echo Failed!
set failed=1

:done
exit /b %failed%
