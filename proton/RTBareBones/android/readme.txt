Help on getting the Android build working

In Windows
---------

(Removed outdated info)

Go to http://www.rtsoft.com/wiki/doku.php?id=proton:android_setup  for the latest directions.

And if something is wrong, please fix it, it's a wiki afterall!


In Linux
--------

* Install Java JDK, Android SDK and Android NDK
* Install ant (e.g. 'yum install ant')
* Put to your $PATH:
  - <android-sdk-dir>/tools
  - <android-sdk-dir>/platform-tools
  - <android-ndk-dir>

In a shell:
$> cd <proton-dir>/RTBareBones/android
$> android update project -p .
$> ndk-build
$> mkdir -p assets/interface
$> cp ../bin/interface/* assets/interface/
$> ant debug
$> ant installd

The last command installs the package to your device or emulator if there is only one of them attached/running.
If all this goes without errors you can then start the application from the launcher menu of your device/emulator.