call app_info_setup.bat

set DEBUG_MODE=1
set RUN_GDB_DEBUGGER=0
set FULL_REBUILD=1

call build_prepare.bat

call %PATH_TO_FLASCC_DIR%\run.bat '%PATH_TO_PROJECT_DIR_FROM_CYGWIN%/build.sh'
if not exist temp.swf type temp.txt
if not exist temp.swf ..\..\shared\win\utils\beeper.exe /p
rename temp.swf %FILENAME%.swf
call ViewInProjector.bat

pause

