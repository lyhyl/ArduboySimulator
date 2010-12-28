::scan the App.cpp file to get info about this product using some batch file tricks and put them into environmental vars for other scripts

ctoenv.exe ..\source\App.cpp "char * bundlePrefix = \"" BUNDLEIDPREFIX
if errorlevel 1  beeper.exe /p
call setenv.bat
del setenv.bat

ctoenv.exe ..\source\App.cpp "char * bundleName = \"" APPNAME
if errorlevel 1  beeper.exe /p
call setenv.bat
del setenv.bat
