call app_info_setup.bat

SET FULL_REBUILD=0
call build_prepare.bat

del temp.txt
%CYGWIN_DIR%\bin\bash --login -i -c '%PATH_TO_PROJECT_DIR_FROM_CYGWIN%/build.sh'
if not exist temp.swf type temp.txt
if not exist temp.swf ..\..\shared\win\utils\beeper.exe /p
rename temp.swf %FILENAME%.swf

call ViewInProjector.bat
:call ViewInBrowser.bat
:pause
