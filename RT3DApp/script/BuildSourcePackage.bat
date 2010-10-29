set FNAME=%RTBACKUP%\iPhone\iPhoneWinSimpleFramework_%DATE:~4,2%_%DATE:~7,2%.zip
cd ..\..
del %FNAME%
shared\win\utils\7za.exe a -r -tzip %FNAME% shared\* *.txt RT3DApp\* -x!*.zip -x!*.svn -x!*.ncb -x!*.bsc -x!*.pdb -x!*.sbr -x!*.ilk -x!*.idb -x!*.obj -x!*.DS_Store -x!._* -x!oglespcviewer.cfg -x!shared\win\powerVR  -x!shared\mac\FMOD  -x!PVRT*.* -x!Common-Debug -x!RT3DApp\build -x!GLESUtils.* -x!ResourceUtils.*

pause