REM Make fonts

set PACK_EXE=..\..\.\shared\win\utils\RTPack.exe

REM Delete all existing packed textures from this dir
cd interface
for /r %%f in (*.rttex) do del %%f
cd ..

for /r %%f in (font*.txt) do %PACK_EXE% -make_font %%f

REM Process our images and textures and copy them into the bin directory

REM -pvrtc4 for compressed, -pvrt4444 or -pvrt8888 (32 bit)  for uncompressed

cd game

:use this one to make pvrtc textures, these only work on iOS and Windows with the PowerVR GLES driver (Common Debug setting in Proton apps)
:for /r %%f in (*.bmp *.png *.jpg *.tga) do ..\%PACK_EXE% -flipv -mipmaps -stretch -4444_if_not_square_or_too_big -pvrtc4 %%f

:Use this for raw bmp textures, should work on everything
for /r %%f in (*.bmp *.png *.jpg *.tga) do ..\%PACK_EXE% -flipv -mipmaps -stretch -pvrt4444 %%f
cd ..

cd interface
for /r %%f in (*.bmp *.png) do ..\%PACK_EXE%  -pvrt8888 %%f
cd ..

REM Custom things that don't need preprocessing


REM Final compression
for /r %%f in (*.rttex) do %PACK_EXE% %%f

REM Delete things we don't want copied
del interface\font_*.rttex

rmdir ..\bin\interface /S /Q
rmdir ..\bin\audio /S /Q
rmdir ..\bin\game /S /Q

REM copy the stuff we care about

mkdir ..\bin\interface
xcopy interface ..\bin\interface /E /F /Y /EXCLUDE:exclude.txt

mkdir ..\bin\audio
xcopy audio ..\bin\audio /E /F /Y /EXCLUDE:exclude.txt

mkdir ..\bin\game
xcopy game ..\bin\game /E /F /Y /EXCLUDE:game_exclude.txt
copy game\dwarf.jpg ..\bin\game

REM Convert everything to lowercase, otherwise the iphone will choke on the files
REM for /r %%f in (*.*) do ..\media\LowerCase.bat  %%f

del icon.rttex
del default.rttex
pause
