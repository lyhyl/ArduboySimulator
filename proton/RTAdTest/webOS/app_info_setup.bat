::scan the App.cpp file to get info about this product using some batch file tricks and put them into environmental vars for other scripts

set RT_UTILS=..\..\shared\win\utils

:The environmental vars BUNDLEIDPREFIX and APPNAME get set below by scanning the source files

%RT_UTILS%\ctoenv.exe ..\source\App.cpp "char * bundlePrefix = \"" BUNDLEIDPREFIX
if errorlevel 1  %RT_UTILS%\beeper.exe /p
call setenv.bat
del setenv.bat

%RT_UTILS%\ctoenv.exe ..\source\App.cpp "char * bundleName = \"" APPNAME
if errorlevel 1  %RT_UTILS%\beeper.exe /p
call setenv.bat
del setenv.bat

echo Package %BUNDLEIDPREFIX% detected for app %APPNAME%
set VERSION=1.0.0
set DEBUG=1
