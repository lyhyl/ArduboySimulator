call app_info_setup.bat
set IPK_NAME=%BUNDLEIDPREFIX%%APPNAME%_1.0.0_all.ipk
::I stopped killing the process because it seemed to sometimes crash the phone
::plink -P 10022 root@localhost -pw "" "killall -9 gdbserver %APPNAME%"
del %APPNAME%
del %IPK_NAME%
call build.bat
if not exist %APPNAME% beeper.exe /p
call packageit.bat
call palm-install.bat -r %BUNDLEIDPREFIX%%APPNAME%
if not exist %IPK_NAME% beeper.exe /p
call palm-install.bat %IPK_NAME%
call RunOnDevice.bat

