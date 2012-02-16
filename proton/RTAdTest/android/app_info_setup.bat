::scan the App.cpp file to get info about this product using some batch file tricks and put them into environmental vars for other scripts

set APP_NAME=RTAdTest
set SMALL_PACKAGE_NAME=rtadtest
set COMPANY_PACKAGE_NAME=rtsoft
set PACKAGE_NAME=com.%COMPANY_PACKAGE_NAME%.%SMALL_PACKAGE_NAME%
set PATH_TO_PROJECT_DIR_FROM_CYGWIN=/cygdrive/d/projects/proton/RTAdTest
set EMULATOR_AVD=@AVD_16


:CYGWIN_DIR should already be set to your cygwin directory as a windows environmental var.

:Use below to set your ant home dir if it isn't already an environmental var!
:set ANT_HOME=d:\pro\ant
copy ..\..\shared\android\util\javapp.jar %ANT_HOME%\lib\
