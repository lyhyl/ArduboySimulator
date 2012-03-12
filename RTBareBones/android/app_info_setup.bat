::scan the App.cpp file to get info about this product using some batch file tricks and put them into environmental vars for other scripts

set APP_NAME=RTBareBones
set SMALL_PACKAGE_NAME=rtbarebones
set PACKAGE_NAME=com.rtsoft.rtbarebones
set EMULATOR_AVD=@AVD_16
set ANDROID_EMU=emulator-5554

::Update/write our local.properties file with our ANDROID NDK dir
call android update project -p ./