#!/bin/bash

PACK_EXE=$(cd `dirname $0`/../RTPack/build/; pwd)/RTPack

if [[ ! -x ${PACK_EXE} ]];
then
	echo "Missing RTPack tool (from ${PACK_EXE})"
	echo "Go to tools/RTPack and run the build.sh script to build the tool"
	exit 1
fi

usage=$(
cat <<EOF
Usage: $0 [options]

Options:
-h               Print this help and exit
EOF
)

while getopts "h" OPTION; do
	case "$OPTION" in
		h)
			echo "$usage"
			exit 0
			;;
		*)
			echo "$usage"
			exit 1
			;;
	esac
done

echo Make fonts

find . -depth -name 'font*.txt' -exec ${PACK_EXE} -make_font '{}' ';'

echo Process our images and textures and copy them into the bin directory

process_directory_images() {
	if [[ -d "$1" ]];
	then
		cd "$1"
		TEXTURE_CONVERSION_OPTS=$(cat texture_conversion_flags.txt)
		find . -depth \( -name '*.jpg' -o -name '*.bmp' -o -name '*.png' \) -exec ${PACK_EXE} ${TEXTURE_CONVERSION_OPTS} '{}' ';'
		cd -
	fi
}

process_directory_images game
process_directory_images interface

echo Final compression

find . -depth -name '*.rttex' -exec ${PACK_EXE} '{}' ';'

echo Delete things we do not want copied
rm -f interface/font_*.rttex

echo Copy the stuff we care about

copy_media_to_bin() {
	if [[ -d "$1" ]];
	then
		sed -e 's/^\..*$/*&/g' "$2" | rsync -v --delete --delete-excluded --recursive --exclude-from=- "$1" ../bin
	fi
}

copy_media_to_bin interface exclude.txt
copy_media_to_bin audio exclude.txt
copy_media_to_bin game game_exclude.txt

rm -f icon.rttex
rm -f default.rttex
