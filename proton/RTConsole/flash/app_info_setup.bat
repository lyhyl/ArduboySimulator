:Support unicode, needed to show some linux errors when we type them later, because I can't get cygwin to show them.
:If you see garbage, you need to set your win dos prompt console font to "Lucida Console"!

chcp 65001

:The flash C++ Compiler setup

set FILENAME=RTConsole

:Cygwin Path to Flascc 1.0+
set PATH_TO_PROJECT_DIR_FROM_CYGWIN=/cygdrive/d/projects/proton/RTConsole/flash

:Flascc dir, used in BuildDebug.bat and BuildRelease.bat
set PATH_TO_FLASCC_DIR=d:\pro\flascc

:used in build.sh
set FLASCC_SDK=/cygdrive/d/pro/flascc/sdk

:Flex 4.6 or higher
set CYGWIN_FLEX_PATH=/cygdrive/d/pro/flex

:Optional stuff, but makes the extra .bat's work right
set SWF_PROJECTOR_FILENAME=..\..\shared\flash\flashplayer_11_sa_debug.exe
set SWF_VIEWER_FILENAME="C:\Program Files (x86)\Mozilla Firefox\firefox.exe"
:set CYGWIN_GDB_SWF_VIEWER_FOR_DEBUGGING=/cygdrive/c/Program Files (x86)/Mozilla Firefox/firefox.exe
set CYGWIN_GDB_SWF_VIEWER_FOR_DEBUGGING=/cygdrive/d/projects/proton/shared/flash/flashplayer_11_sa_debug.exe


:Control build modes here
set DEBUG_MODE=0
set RUN_GDB_DEBUGGER=0
SET FULL_REBUILD=1
