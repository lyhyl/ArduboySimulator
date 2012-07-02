#!/bin/bash

../../tools/linux/prepareAndroid.sh

ndk-build

cd ../media
../../tools/linux/update_media.sh
cd ../android

../../tools/linux/androidSyncAssets.sh

ant debug

echo "Build finished"
echo "Command 'ant installd' to install the newly created package to a device"
