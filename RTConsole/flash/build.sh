#!/bin/sh 

#just get away from  the bin dir to be safe..yes, I did delete my cygwin/bin dir accidentally thank you very much
cd ..
cd $PATH_TO_PROJECT_DIR_FROM_CYGWIN

#sometimes the build dir has funky permissions (because it was made in win?), fix just in case
chmod +wxr -R build

if [ $FULL_REBUILD -eq 1 ]; then
	make clean
	fi

make -j7 VERBOSE=1 FLASCC=$FLASCC_SDK FLEX=$CYGWIN_FLEX_PATH

if [ $RUN_GDB_DEBUGGER -eq 1 ]; then
	export FLASCC_GDB_RUNTIME="$CYGWIN_GDB_SWF_VIEWER_FOR_DEBUGGING"
	$FLASCC_SDK/usr/bin/gdb -x gdb_script.txt temp.swf
	fi
	

	
