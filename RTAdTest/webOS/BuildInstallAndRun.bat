call app_info_setup.bat
set IPK_NAME=%BUNDLEIDPREFIX%%APPNAME%_%VERSION%_all.ipk
del %APPNAME%
del %IPK_NAME%
call build.bat
if not exist %APPNAME% beeper.exe /p
call InstallAndRun.bat
