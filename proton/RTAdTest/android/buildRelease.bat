call build_prepare.bat
:Next package it with the java part
call ant release

:and finally, load it into the emulator

REM Waiting for device to get ready...
:adb wait-for-device -s emulator-5554

REM Installing...
:adb -s %ANDROID_EMU% install -r bin\%APP_NAME%-release.apk
call InstallOnDefaultPhoneRelease.bat
:adb logcat
pause