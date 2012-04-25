call app_info_setup.bat
:Get the emulator ready if it isn't, because it takes a freakin' long time to load
REM start emulator %EMULATOR_AVD%

del bin\%APP_NAME%-release.apk

:build the C/C++ parts
call ndk-build -j7
if not exist libs/armeabi/lib%SMALL_PACKAGE_NAME%.so ..\..\shared\win\utils\beeper.exe /p

:Copy refresh resources, assuming the windows version had them built with update_media recently...
rmdir assets /S /Q

mkdir assets

mkdir assets\interface
xcopy ..\bin\interface assets\interface /E /F /Y

mkdir assets\game
xcopy ..\bin\game assets\game /E /F /Y

mkdir assets\audio
xcopy ..\bin\audio assets\audio /E /F /Y

:Kill old files handing around to avoid confusion
rmdir bin /S /Q
rmdir gen /S /Q
:Next package it with the java part
call ant release

call InstallOnDefaultPhone.bat
:adb logcat
pause