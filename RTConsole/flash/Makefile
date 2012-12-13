SWF_SIZE := 640x480
#-falcon
# -DRT_JPG_SUPPORT if we use JPG support
CUSTOM_FLAGS := -DPLATFORM_FLASH -DBOOST_ALL_NO_LIB -D_CONSOLE
SHARED := ../../shared
APP := ../source
COMPPATH := $(SHARED)/Entity
CLANMATH := $(SHARED)/ClanLib-2.0/Sources/Core/Math
ZLIBPATH := $(SHARED)/util/zlib
PNGSRC :=  $(SHARED)/Irrlicht/source/Irrlicht/libpng
JPGSRC :=  $(SHARED)/Irrlicht/source/Irrlicht/jpeglib
LZMASRC :=  $(SHARED)/Irrlicht/source/Irrlicht/lzma
PPATH := $(SHARED)/Renderer/linearparticle/sources
AS3COMPILERARGS := java  -jar $(FLASCC)/usr/lib/asc2.jar -merge -md

$?BUILDDIR=build
$?BASECFLAGS=$(EXTRACFLAGS)
$?BASECPPFLAGS=$(EXTRACPPFLAGS)

#Release mode is default
COMP_MODE := -O4 -DNDEBUG
MORE_LINKER_OPTIONS := -flto-api=exports.txt -disable-telemetry

#debug mode is set through the bat/sh file before calling this ($DEBUG_MODE should be 0 or 1)

ifeq (${DEBUG_MODE}, 1)
 	COMP_MODE := -D_DEBUG -g -O0
 	MORE_LINKER_OPTIONS :=
endif

SHARED_FLAGS := $(COMP_MODE) $(CUSTOM_FLAGS)

$?CFLAGS=$(SHARED_FLAGS)
$?CPPFLAGS=$(SHARED_FLAGS)

INCLUDES := -I$(SHARED) -I$(SHARED)/flash -I$(APP) -I$(SHARED)/ClanLib-2.0/Sources -I$(SHARED)/util/boost

OBJS = $(SHARED)/flash/app/cpp/main.o $(SHARED)/flash/FlashUtils.o $(SHARED)/util/MiscUtils.o $(SHARED)/util/ResourceUtils.o \
$(SHARED)/PlatformSetup.o $(SHARED)/FileSystem/FileManager.o $(SHARED)/FileSystem/FileSystem.o $(SHARED)/FileSystem/StreamingInstance.o \
$(SHARED)/FileSystem/StreamingInstanceFile.o $(SHARED)/util/TextScanner.o \
\
$(CLANMATH)/angle.o $(CLANMATH)/vec2.o $(CLANMATH)/vec3.o \
\
$(SHARED)/util/Variant.o $(SHARED)/util/boost/libs/signals/src/connection.o $(SHARED)/util/boost/libs/signals/src/named_slot_map.o $(SHARED)/util/boost/libs/signals/src/signal_base.o \
$(SHARED)/util/boost/libs/signals/src/slot.o $(SHARED)/util/boost/libs/signals/src/trackable.o \
$(SHARED)/util/unzip/unzip.o $(SHARED)/util/unzip/ioapi.o $(SHARED)/util/MathUtils.o \
\
$(ZLIBPATH)/deflate.o $(ZLIBPATH)/gzio.o $(ZLIBPATH)/infback.o $(ZLIBPATH)/inffast.o $(ZLIBPATH)/inflate.o \
$(ZLIBPATH)/inftrees.o $(ZLIBPATH)/trees.o $(ZLIBPATH)/uncompr.o $(ZLIBPATH)/zutil.o $(ZLIBPATH)/adler32.o \
$(ZLIBPATH)/compress.o $(ZLIBPATH)/crc32.o \
\
$(APP)/App.o

T03: check
	@echo "-------- MAKING STUFF --------"
	#mkdir -p $(BUILDDIR)
	#mkdir -p $(BUILDDIR)/vfs
	
	#cd $(BUILDDIR) &&$(AS3COMPILERARGS) \
	#-import $(call nativepath,$(FLASCC)/usr/lib/builtin.abc) \
	#-import $(call nativepath,$(FLASCC)/usr/lib/playerglobal.abc) \
	#-import $(call nativepath,$(FLASCC)/usr/lib/BinaryData.abc) \
	# com/rtsoft/*.as \
	# com/adobe/utils/AGALMiniAssembler.as \
	# org/audiofx/mp3/ByteArraySegment.as \
	# org/audiofx/mp3/MP3FileReferenceLoader.as \
	# org/audiofx/mp3/MP3Parser.as \
	# org/audiofx/mp3/MP3SoundEvent.as \
	# org/audiofx/mp3/SoundClassSwfByteCode.as \
#		 	 -outdir . -out rtsoft

	
	cd $(BUILDDIR) && "$(FLASCC)/usr/bin/genfs" vfs vfs
	cd $(BUILDDIR) && "$(FLASCC)/usr/bin/genfs" --type=http webvfs webvfs
	cp "$(FLASCC)/usr/share/HTTPBackingStore.as" $(BUILDDIR)
	cp "$(FLASCC)/usr/share/LSOBackingStore.as" $(BUILDDIR)
	cd $(BUILDDIR) && mv webvfs_manifest.as manifest.as

	cd $(BUILDDIR) &&$(AS3COMPILERARGS)	-import $(call nativepath,$(FLASCC)/usr/lib/builtin.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/playerglobal.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/BinaryData.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/ISpecialFile.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/IBackingStore.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/IVFS.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/InMemoryBackingStore.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/PlayerKernel.abc) \
				HTTPBackingStore.as LSOBackingStore.as vfsRootFSBackingStore.as -outdir . -out VFS

	cd $(BUILDDIR) &&$(AS3COMPILERARGS)	-import $(call nativepath,$(FLASCC)/usr/lib/builtin.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/playerglobal.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/BinaryData.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/ISpecialFile.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/IBackingStore.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/IVFS.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/InMemoryBackingStore.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/PlayerKernel.abc) \
				-import VFS.abc \
				RTConsoleVersion/Console.as -outdir . -out Console

	@echo "-------- Building a misc file we need --------"


	make real_build

as3_build: $(ACTIONSCRIPT)

	@echo Building .abc files out of the .as files

real_build: $(OBJS)

	#Uncomment below if you want a weird cygwin projector .exe that isn't good for anything
	#cd $(BUILDDIR) && "$(FLASCC)/usr/bin/g++" $(CPPFLAGS) $(FLASCC)/usr/lib/AlcVFSZip.abc -symbol-abc=Console.abc \
	#$(notdir $(OBJS) )*.abc -Wl,--warn-unresolved-symbols $(MORE_LINKER_OPTIONS) -swf-size=$(SWF_SIZE) \
	#-o ../temp.exe
	
	@echo Making Swf version
	"$(FLASCC)/usr/bin/g++" $(CPPFLAGS) $(FLASCC)/usr/lib/AlcVFSZip.abc \
		-symbol-abc=$(BUILDDIR)/Console.abc $(subst .c, .o,$(subst .cpp,.o,$(OBJS))) $(BUILDDIR)/*.abc \
		-Wl,--warn-unresolved-symbols -o temp.swf $(MORE_LINKER_OPTIONS) -emit-swf \
		-swf-size=$(SWF_SIZE)
	
include ../../shared/flash/Makefile.common

clean:
	rm -f *.swf *.bc temp.txt 
	rm $(subst .c, .o,$(subst .cpp,.o,$(OBJS)))
	#rm -rf $(BUILDDIR) *.swf 

#unused, but another, slower way, to compile .as files
#%.abc: %.as
#	@echo Making a as...
#	java  -jar $(call nativepath,$(FLASCC)/usr/lib/asc.jar) -import $(call nativepath,$(FLASCC)/usr/lib/builtin.abc) -import $(call nativepath,$(FLASCC)/usr/lib/playerglobal.abc) -import $(call nativepath,$(FLASCC)/usr/lib/BinaryData.abc) $*.as -outdir $(BUILDDIR) -out $(notdir $*)

ifeq (${AS3_ONLY}, 1)

.c.o:
	
.cpp.o:

else

.c.o:

	"$(FLASCC)/usr/bin/gcc" $(CFLAGS) $(INCLUDES) $(BASECFLAGS) -c $< -o $(@D)/$(@F) 2>> temp.txt
	
.cpp.o:
	"$(FLASCC)/usr/bin/g++" $(CPPFLAGS) $(INCLUDES) $(BASECPPFLAGS) -c $< -o $(@D)/$(@F) 2>> temp.txt

endif
	