#!/bin/sh

PACK_EXE=$(cd ../../tools/RTPack/build/; pwd)/RTPack

if [[ ! -x ${PACK_EXE} ]];
then
	echo "Missing RTPack tool (from ${PACK_EXE})"
	echo "Go to tools/RTPack and run the build.sh script to build the tool"
	exit 1
fi

echo Make fonts

echo Delete all existing packed textures from directory interface
find interface -depth -name '*.rttex' -delete

find . -depth -name 'font*.txt' -exec ${PACK_EXE} -make_font '{}' ';'

echo Process our images and textures and copy them into the bin directory

# Note:  You'd probably also want to add .jpg to below, but I don't because I want to test the .jpg loader, not turn it into an .rttex.

process_directory_images() {
	if [[ -d "$1" ]];
	then
		cd "$1"
		find . -depth \( -name '*.bmp' -o -name '*.png' \) -exec ${PACK_EXE} -pvrt8888 -ultra_compress 90 '{}' ';'
		cd -
	fi
}

process_directory_images game
process_directory_images interface


echo Final compression

find . -depth -name '*.rttex' -exec ${PACK_EXE} '{}' ';'

echo Delete things we do not want copied
rm interface/font_*.rttex

rm -rf ../bin/interface
rm -rf ../bin/audio
rm -rf ../bin/game

echo copy the stuff we care about

copy_media_to_bin() {
	if [[ -d "$1" ]];
	then
		sed -e 's/^\..*$/*&/g' "$2" | rsync -v --recursive --exclude-from=- "$1" ../bin
	fi
}

copy_media_to_bin interface exclude.txt
copy_media_to_bin audio exclude.txt
copy_media_to_bin game game_exclude.txt

rm icon.rttex
rm default.rttex
