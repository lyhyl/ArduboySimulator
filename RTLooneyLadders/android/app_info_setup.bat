::scan the App.cpp file to get info about this product using some batch file tricks and put them into environmental vars for other scripts

set APP_NAME=RTLooneyLadders
set SMALL_PACKAGE_NAME=RTLooneyLadders
set PACKAGE_NAME=com.rtsoft.RTLooneyLadders
set EMULATOR_AVD=@AVD_22
set ANDROID_EMU=emulator-5554

::Update/write our local.properties file with our ANDROID NDK dir
call android update project -p ./