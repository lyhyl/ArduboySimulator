call app_info_setup.bat
:Get the emulator ready if it isn't, because it takes a freakin' long time to load
REM start emulator %EMULATOR_AVD%

:build the C/C++ parts (Note:  not using cygwin anymore, yay!)
call ndk-build

if not exist libs/armeabi/lib%SMALL_PACKAGE_NAME%.so ..\..\shared\win\utils\beeper.exe /p

:Copy refresh resources, assuming the windows version had them built with update_media recently...
rmdir assets /S /Q

mkdir assets

mkdir assets\interface
xcopy ..\bin\interface assets\interface /E /F /Y

:Well, we don't need game/audio dirs for this project.  But if we did...

:mkdir assets\game
:xcopy ..\bin\game assets\game /E /F /Y

:mkdir assets\audio
:xcopy ..\bin\audio assets\audio /E /F /Y

:Kill old files handing around to avoid confusion
rmdir bin /S /Q
rmdir gen /S /Q
:Next package it with the java part
call ant debug

:and finally, load it into the emulator

REM Waiting for device to get ready...
:adb wait-for-device -s emulator-5554

REM Installing...
REM adb -s %ANDROID_EMU% install -r bin\%APP_NAME%-debug.apk
call InstallOnDefaultPhone.bat
:adb logcat
pause