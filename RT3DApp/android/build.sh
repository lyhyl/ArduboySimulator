#!/bin/sh 

#export NDK_PROJECT_PATH="/cygdrive/o/projects/proton/RT3DApp/android";
#just get away from  the bin dir to be safe..yes, I did delete my cygwin/bin dir accidentally thank you very much
cd ..
cd /cygdrive/o/projects/proton/RT3DApp/android
#rm -rf ./bin ./obj ./libs

#ndk-build clean
ndk-build NDK_LOG=1