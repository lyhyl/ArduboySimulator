SWF_SIZE := 480x320

# -DRT_JPG_SUPPORT if we use JPG support
CUSTOM_FLAGS := -DPLATFORM_FLASH -DBOOST_ALL_NO_LIB -DRT_JPG_SUPPORT
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

#Release mode is default   Add -fllvm-opt-opt=-strip to hide function names.  -O4 introduces weird delays? Flascc bug?
COMP_MODE := -O4 -DNDEBUG

#-fno-exceptions
MORE_LINKER_OPTIONS := -jvmopt=-Xmx1500M -flto-api=exports.txt

#debug mode is set through the bat/sh file before calling this ($DEBUG_MODE should be 0 or 1)

ifeq (${DEBUG_MODE}, 1)
 	COMP_MODE := -D_DEBUG -O0
	MORE_LINKER_OPTIONS := -jvmopt=-Xmx1500M

endif

SHARED_FLAGS := $(COMP_MODE) $(CUSTOM_FLAGS)

$?CFLAGS=$(SHARED_FLAGS)
$?CPPFLAGS=$(SHARED_FLAGS)

INCLUDES := -I$(SHARED) -I$(SHARED)/flash -I$(APP) -I$(SHARED)/ClanLib-2.0/Sources -I$(SHARED)/util/boost

OBJS = $(SHARED)/flash/app/cpp/main.o $(SHARED)/flash/app/cpp/GLFlashAdaptor.o $(SHARED)/flash/FlashUtils.o \
$(SHARED)/Audio/AudioManagerFlash.o \
\
$(SHARED)/PlatformSetup.o $(SHARED)/Audio/AudioManager.o \
$(CLANMATH)/angle.o $(CLANMATH)/mat3.o $(CLANMATH)/mat4.o $(CLANMATH)/rect.o $(CLANMATH)/vec2.o $(CLANMATH)/vec3.o $(CLANMATH)/vec4.o \
$(SHARED)/Entity/Entity.o $(SHARED)/Entity/Component.o $(SHARED)/GUI/RTFont.o $(SHARED)/Manager/Console.o \
$(SHARED)/Manager/GameTimer.o $(SHARED)/Manager/MessageManager.o $(SHARED)/Manager/ResourceManager.o $(SHARED)/Manager/VariantDB.o $(SHARED)/Math/rtPlane.o \
$(SHARED)/Math/rtRect.o $(SHARED)/Renderer/RenderBatcher.o $(SHARED)/Renderer/SoftSurface.o $(SHARED)/Renderer/Surface.o $(SHARED)/Renderer/SurfaceAnim.o \
$(SHARED)/util/CRandom.o $(SHARED)/util/GLESUtils.o $(SHARED)/util/MathUtils.o $(SHARED)/util/MiscUtils.o $(SHARED)/util/RenderUtils.o $(SHARED)/util/ResourceUtils.o \
$(SHARED)/util/Variant.o $(SHARED)/util/boost/libs/signals/src/connection.o $(SHARED)/util/boost/libs/signals/src/named_slot_map.o $(SHARED)/util/boost/libs/signals/src/signal_base.o \
$(SHARED)/util/boost/libs/signals/src/slot.o $(SHARED)/util/boost/libs/signals/src/trackable.o $(SHARED)/BaseApp.o \
$(SHARED)/util/unzip/unzip.o $(SHARED)/util/unzip/ioapi.o $(SHARED)/util/TextScanner.o $(SHARED)/Entity/EntityUtils.o \
$(SHARED)/FileSystem/StreamingInstance.o $(SHARED)/Renderer/JPGSurfaceLoader.o \
$(SHARED)/FileSystem/StreamingInstanceZip.o $(SHARED)/FileSystem/StreamingInstanceFile.o $(SHARED)/FileSystem/FileSystem.o $(SHARED)/FileSystem/FileSystemZip.o \
$(SHARED)/FileSystem/FileManager.o \
\
$(COMPPATH)/Button2DComponent.o $(COMPPATH)/FilterInputComponent.o $(COMPPATH)/FocusInputComponent.o $(COMPPATH)/FocusRenderComponent.o $(COMPPATH)/FocusUpdateComponent.o \
$(COMPPATH)/InputTextRenderComponent.o $(COMPPATH)/InterpolateComponent.o $(COMPPATH)/OverlayRenderComponent.o $(COMPPATH)/ProgressBarComponent.o \
$(COMPPATH)/RectRenderComponent.o $(COMPPATH)/ScrollBarRenderComponent.o $(COMPPATH)/ScrollComponent.o $(COMPPATH)/TapSequenceDetectComponent.o $(COMPPATH)/TextBoxRenderComponent.o \
$(COMPPATH)/TextRenderComponent.o $(COMPPATH)/TouchStripComponent.o $(COMPPATH)/TrailRenderComponent.o $(COMPPATH)/TyperComponent.o $(COMPPATH)/UnderlineRenderComponent.o \
$(COMPPATH)/TouchHandlerComponent.o $(COMPPATH)/SelectButtonWithCustomInputComponent.o $(COMPPATH)/CustomInputComponent.o $(COMPPATH)/SliderComponent.o $(COMPPATH)/EmitVirtualKeyComponent.o \
$(COMPPATH)/RenderScissorComponent.o \
\
$(JPGSRC)/jcapimin.o $(JPGSRC)/jcapistd.o $(JPGSRC)/jccoefct.o $(JPGSRC)/jccolor.o $(JPGSRC)/jcdctmgr.o $(JPGSRC)/jchuff.o $(JPGSRC)/jcinit.o $(JPGSRC)/jcmainct.o \
$(JPGSRC)/jcmarker.o $(JPGSRC)/jcmaster.o $(JPGSRC)/jcomapi.o $(JPGSRC)/jcparam.o $(JPGSRC)/jcphuff.o $(JPGSRC)/jcprepct.o $(JPGSRC)/jcsample.o $(JPGSRC)/jctrans.o \
$(JPGSRC)/jdapimin.o $(JPGSRC)/jdapistd.o $(JPGSRC)/jdatadst.o $(JPGSRC)/jdatasrc.o $(JPGSRC)/jdcoefct.o $(JPGSRC)/jdcolor.o $(JPGSRC)/jddctmgr.o \
$(JPGSRC)/jdhuff.o $(JPGSRC)/jdinput.o $(JPGSRC)/jdmainct.o $(JPGSRC)/jdmarker.o $(JPGSRC)/jdmaster.o $(JPGSRC)/jdmerge.o $(JPGSRC)/jdphuff.o $(JPGSRC)/jdpostct.o \
$(JPGSRC)/jdsample.o $(JPGSRC)/jdtrans.o $(JPGSRC)/jerror.o $(JPGSRC)/jfdctflt.o $(JPGSRC)/jfdctfst.o $(JPGSRC)/jfdctint.o $(JPGSRC)/jidctflt.o $(JPGSRC)/jidctfst.o \
$(JPGSRC)/jidctint.o $(JPGSRC)/jidctred.o $(JPGSRC)/jmemmgr.o $(JPGSRC)/jmemnobs.o $(JPGSRC)/jquant1.o $(JPGSRC)/jquant2.o $(JPGSRC)/jutils.o \
\
$(ZLIBPATH)/deflate.o $(ZLIBPATH)/gzio.o $(ZLIBPATH)/infback.o $(ZLIBPATH)/inffast.o $(ZLIBPATH)/inflate.o \
$(ZLIBPATH)/inftrees.o $(ZLIBPATH)/trees.o $(ZLIBPATH)/uncompr.o $(ZLIBPATH)/zutil.o $(ZLIBPATH)/adler32.o \
$(ZLIBPATH)/compress.o $(ZLIBPATH)/crc32.o \
\
$(PPATH)/L_Defination.o $(PPATH)/L_DroppingEffect.o $(PPATH)/L_EffectEmitter.o $(PPATH)/L_ExplosionEffect.o $(PPATH)/L_MotionController.o $(PPATH)/L_Particle.o \
$(PPATH)/L_ParticleEffect.o $(PPATH)/L_ParticleMem.o $(PPATH)/L_ParticleSystem.o $(PPATH)/L_ShootingEffect.o $(PPATH)/L_EffectManager.o \
\
$(APP)/App.o $(APP)/Component/ParticleTestComponent.o $(APP)/GUI/DebugMenu.o $(APP)/GUI/EnterNameMenu.o $(APP)/GUI/MainMenu.o \
$(APP)/GUI/ParticleTestMenu.o $(APP)/GUI/AboutMenu.o $(APP)/GUI/TouchTestMenu.o $(APP)/Component/TouchTestComponent.o


T03: check
	@echo "-------- MAKING STUFF --------"
	#mkdir -p $(BUILDDIR)
	#mkdir -p $(BUILDDIR)/vfs
	
	cd $(BUILDDIR) &&$(AS3COMPILERARGS) \
	-import $(call nativepath,$(FLASCC)/usr/lib/builtin.abc) \
	-import $(call nativepath,$(FLASCC)/usr/lib/playerglobal.abc) \
	-import $(call nativepath,$(FLASCC)/usr/lib/BinaryData.abc) \
	 com/rtsoft/*.as \
	 com/adobe/utils/AGALMiniAssembler.as \
	 org/audiofx/mp3/ByteArraySegment.as \
	 org/audiofx/mp3/MP3FileReferenceLoader.as \
	 org/audiofx/mp3/MP3Parser.as \
	 org/audiofx/mp3/MP3SoundEvent.as \
	 org/audiofx/mp3/SoundClassSwfByteCode.as \
		 	 -outdir . -out rtsoft

	
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
				-import rtsoft.abc \
				Console.as -outdir . -out Console

	@echo "-------- Building a misc file we need --------"


	make real_build

as3_build: $(ACTIONSCRIPT)

	@echo Building .abc files out of the .as files

real_build: $(OBJS)

#

	@echo Making Swf version
	"$(FLASCC)/usr/bin/g++" $(CPPFLAGS) $(FLASCC)/usr/lib/AlcVFSZip.abc \
		-symbol-abc=$(BUILDDIR)/Console.abc $(subst .c, .o,$(subst .cpp,.o,$(OBJS))) $(BUILDDIR)/*.abc \
		-Wl,--warn-unresolved-symbols -o temp.swf $(MORE_LINKER_OPTIONS) -emit-swf \
		-swf-size=$(SWF_SIZE) -lFlash++ -lAS3++
	
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

	"$(FLASCC)/usr/bin/gcc" $(MORE_LINKER_OPTIONS) $(CFLAGS) $(INCLUDES) $(BASECFLAGS) -c $< -o $(@D)/$(@F) 2>> temp.txt
	
.cpp.o:
	"$(FLASCC)/usr/bin/g++" $(MORE_LINKER_OPTIONS) $(CPPFLAGS) $(INCLUDES) $(BASECPPFLAGS) -c $< -o $(@D)/$(@F) 2>> temp.txt

endif
	