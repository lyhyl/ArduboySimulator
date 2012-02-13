call app_info_setup.bat
:Get the emulator ready if it isn't, because it takes a freakin' long time to load
:start emulator %EMULATOR_AVD%

:Copy refresh resources, assuming the windows version had them built with update_media recently...

rmdir assets /S /Q
mkdir assets

:It's ok if you get "0 files copied" messages, there just aren't files in some of these
mkdir assets\interface
xcopy ..\bin\interface assets\interface /E /F /Y

mkdir assets\game
xcopy ..\bin\game assets\game /E /F /Y

mkdir assets\audio
xcopy ..\bin\audio assets\audio /E /F /Y

:Kill old files handing around to avoid confusion
rmdir bin /S /Q
rmdir gen /S /Q
rmdir temp_final_src /S /Q

:build temp src directory, which will preprocess and output to staging_src a bit later with ant

rmdir temp_src /S /Q
mkdir temp_src
mkdir temp_src\com
mkdir temp_src\com\%COMPANY_PACKAGE_NAME%
mkdir temp_src\com\%COMPANY_PACKAGE_NAME%\%SMALL_PACKAGE_NAME%

:move in our app specific java file(s)
xcopy src temp_src\com\%COMPANY_PACKAGE_NAME%\%SMALL_PACKAGE_NAME%  /E /F /Y

mkdir temp_final_src
mkdir temp_final_src\com

:copy pre processed shared java files over
xcopy ..\..\shared\android\v2_src\java temp_src\com\%COMPANY_PACKAGE_NAME%\%SMALL_PACKAGE_NAME% /E /F /Y

:copy any extra libraries we need over - skip the preprocessing step for these, move them directly to the final dir

:for IAP
echo d | xcopy ..\..\shared\android\optional_src\com\android temp_final_src\com\android /E /F /Y

:for tapjoy
echo d | xcopy ..\..\shared\android\optional_src\com\tapjoy temp_final_src\com\tapjoy /E /F /Y

:there is a single .cpp file that we need to preprocess as well (to modify the jni function names to match our class path), C++ macros just can't do it
rmdir temp_final_cpp_src  /S /Q
call ant preprocess_cpp

:Next package it with the java part
call ant preprocess

:build the C/C++ parts
%CYGWIN_DIR%\bin\bash --login -i -c '%PATH_TO_PROJECT_DIR_FROM_CYGWIN%/android/build.sh'
if not exist libs/armeabi/lib%SMALL_PACKAGE_NAME%.so ..\..\shared\win\utils\beeper.exe /p