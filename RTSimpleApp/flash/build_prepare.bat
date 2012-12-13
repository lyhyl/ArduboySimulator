del %FILENAME%.swf

if "%FULL_REBUILD%" == "1" (
echo f | del build
rmdir build /S /Q
mkdir build
)


echo d | xcopy ..\..\shared\flash\app\flash build /E /F /Y

:Copy our files to the virtual flash filesystem
mkdir build\vfs
mkdir build\webvfs

mkdir build\vfs\interface
echo d | xcopy ..\bin\interface build\vfs\interface /E /F /Y

mkdir build\vfs\game
xcopy ..\bin\game build\vfs\game /E /F /Y

mkdir build\vfs\audio
xcopy ..\bin\audio build\vfs\audio /E /F /Y

:Flash currently can only load mp3s, so let's remove the .wav and oggs
del build\vfs\audio\*.ogg
del build\vfs\audio\*.wav
