call app_info_setup.bat

::Update/write our local.properties file with our ANDROID NDK dir
call android update project -p ./
@echo on

:Get the emulator ready if it isn't, because it takes a freakin' long time to load
:start emulator %EMULATOR_AVD%

:Copy refresh resources, assuming the windows version had them built with update_media recently...

rmdir assets /S /Q
mkdir assets

mkdir libs

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

:optional libs we made need, you need to download and put in the appropriate place if you use them

goto skipstartapp;
:for startapp
set TEMPFILE=..\..\shared\android\optional_src\libs\startapp\SearchHelperService-1.0.14-jar-with-dependencies.jar

:Extra check to make sure we can locate the files
if exist "%TEMPFILE%" (
echo Located startapp files.
) else (
echo Cannot find the tapjoy library files.  Download from startapp and place lib into shared\android\optional_src\libs\startapp
..\..\shared\win\utils\beeper.exe /p
)
copy %TEMPFILE% libs
:skipstartapp

:goto skiphooked;
:For Hooked/wasabi
set TEMPFILE=..\..\shared\android\optional_src\libs\wasabi\wasabi.jar
:Extra check to make sure we can locate the files
if exist "%TEMPFILE%" (
echo Located startapp files.
) else (
echo Cannot find the tapjoy library files.  Download from startapp and place lib into shared\android\optional_src\libs\startapp
..\..\shared\win\utils\beeper.exe /p
)
copy %TEMPFILE% libs
:skiphooked


:for Chartboost
set TEMPFILE=..\..\shared\android\optional_src\libs\ChartBoost\chartboost.jar

:Extra check to make sure we can locate the files
if exist "%TEMPFILE%" (
echo Located startapp files.
) else (
echo Cannot find the ChartBoost .jar file.  Download and place lib into shared\android\optional_src\libs\ChartBoost
..\..\shared\win\utils\beeper.exe /p
)
copy %TEMPFILE% libs


:Next package it with the java part
call ant preprocess

:build the C/C++ parts.. you might want to remove/modify the -j7 part if you don't have a multicore processor with 8 threads...
call ndk-build -j7
if not exist libs/armeabi/lib%SMALL_PACKAGE_NAME%.so ..\..\shared\win\utils\beeper.exe /p