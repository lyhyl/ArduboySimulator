Cls
call app_info_setup.bat

REM arm-none-linux-gnueabi-g++ -o tiltodemo ..\common\accelerometer.cpp ..\common\collision.cpp ..\common\gamelogic.cpp ..\common\geometry.cpp ..\common\graphics_ogl.cpp ..\common\graphics_sdl.cpp ..\common\main.cpp ..\common\sdl_init.cpp ..\common\sound.cpp "-I%PalmPDK%\include" "-I%PalmPDK%\include\SDL" "-L%PalmPDK%\device\lib" -Wl,--allow-shlib-undefined -lSDL -lSDL_net -lSDL_image -lSDL_mixer -lpdl -lGLES_CM

REM @echo off
@rem Set the device you want to build for to 1
@rem Use Pixi to allow running on either device
set PRE=0
set PIXI=1
REM set DEBUG=0 //set in app_info_setup.bat, set to 1 for a debug build that isn't stripped during packging

set SHARED=..\..\shared
set APP=..\source
set CLANMATH=..\..\shared\ClanLib-2.0\Sources\Core\Math
set ZLIBPATH=..\..\shared\util\zlib
set PPATH=..\..\shared\Renderer\linearparticle\sources
set COMPPATH=..\..\shared\Entity

set PNGSRC=..\..\shared\Irrlicht\source\Irrlicht\libpng

set JPGSRC=..\..\shared\Irrlicht\source\Irrlicht\jpeglib

set LZMASRC=..\..\shared\Irrlicht\source\Irrlicht\lzma


@rem List your source files here


REM **************************************** ENGINE SOURCE CODE FILES
set SRC= %SHARED%\PlatformSetup.cpp  %SHARED%\WebOS\SDLMain.cpp %SHARED%\win\app\main.cpp %SHARED%\webOS\WebOSUtils.cpp ^
%SHARED%\Audio\AudioManager.cpp %CLANMATH%\angle.cpp %CLANMATH%\mat3.cpp %CLANMATH%\mat4.cpp %CLANMATH%\rect.cpp %CLANMATH%\vec2.cpp %CLANMATH%\vec3.cpp ^
%CLANMATH%\vec4.cpp %SHARED%\Entity\Entity.cpp %SHARED%\Entity\Component.cpp %SHARED%\GUI\RTFont.cpp %SHARED%\Manager\Console.cpp ^
%SHARED%\Manager\GameTimer.cpp %SHARED%\Manager\MessageManager.cpp %SHARED%\Manager\ResourceManager.cpp %SHARED%\Manager\VariantDB.cpp %SHARED%\Math\rtPlane.cpp ^
%SHARED%\Math\rtRect.cpp %SHARED%\Renderer\RenderBatcher.cpp %SHARED%\Renderer\SoftSurface.cpp %SHARED%\Renderer\Surface.cpp %SHARED%\Renderer\SurfaceAnim.cpp ^
%SHARED%\util\CRandom.cpp %SHARED%\util\GLESUtils.cpp %SHARED%\util\MathUtils.cpp %SHARED%\util\MiscUtils.cpp %SHARED%\util\RenderUtils.cpp %SHARED%\util\ResourceUtils.cpp ^
%SHARED%\util\Variant.cpp %SHARED%\util\boost\libs\signals\src\connection.cpp %SHARED%\util\boost\libs\signals\src\named_slot_map.cpp %SHARED%\util\boost\libs\signals\src\signal_base.cpp ^
%SHARED%\util\boost\libs\signals\src\slot.cpp %SHARED%\util\boost\libs\signals\src\trackable.cpp %SHARED%\BaseApp.cpp %SHARED%\util\TextScanner.cpp %SHARED%\Entity\EntityUtils.cpp ^
%SHARED%\Network\NetHTTP.cpp %SHARED%\Network\NetSocket.cpp %SHARED%\Network\NetUtils.cpp %SHARED%\Audio\AudioManagerSDL.cpp %SHARED%\util\unzip\unzip.c %SHARED%\util\unzip\ioapi.c ^
%SHARED%\FileSystem\StreamingInstance.cpp %SHARED%\FileSystem\StreamingInstanceZip.cpp %SHARED%\FileSystem\StreamingInstanceFile.cpp %SHARED%\FileSystem\FileSystem.cpp ^
%SHARED%\FileSystem\FileSystemZip.cpp %SHARED%\FileSystem\FileManager.cpp %SHARED%\Renderer\JPGSurfaceLoader.cpp

REM **************************************** ENGINE COMPONENT SOURCE CODE FILES
set COMPONENT_SRC=%COMPPATH%\Button2DComponent.cpp %COMPPATH%\FilterInputComponent.cpp %COMPPATH%\FocusInputComponent.cpp %COMPPATH%\FocusRenderComponent.cpp %COMPPATH%\FocusUpdateComponent.cpp ^
%COMPPATH%\HTTPComponent.cpp %COMPPATH%\InputTextRenderComponent.cpp %COMPPATH%\InterpolateComponent.cpp %COMPPATH%\OverlayRenderComponent.cpp %COMPPATH%\ProgressBarComponent.cpp ^
%COMPPATH%\RectRenderComponent.cpp %COMPPATH%\ScrollBarRenderComponent.cpp %COMPPATH%\ScrollComponent.cpp %COMPPATH%\TapSequenceDetectComponent.cpp %COMPPATH%\TextBoxRenderComponent.cpp ^
%COMPPATH%\TextRenderComponent.cpp %COMPPATH%\TouchStripComponent.cpp %COMPPATH%\TrailRenderComponent.cpp %COMPPATH%\TyperComponent.cpp %COMPPATH%\UnderlineRenderComponent.cpp ^
%COMPPATH%\TouchHandlerComponent.cpp %COMPPATH%\CustomInputComponent.cpp %COMPPATH%\SelectButtonWithCustomInputComponent.cpp %COMPPATH%\SliderComponent.cpp %COMPPATH%\EmitVirtualKeyComponent.cpp ^
%COMPPATH%\RenderScissorComponent.cpp

REM **************************************** JPEG SOURCE CODE FILES
set JPG_SRC=%JPGSRC%\jcapimin.c %JPGSRC%\jcapistd.c %JPGSRC%\jccoefct.c %JPGSRC%\jccolor.c %JPGSRC%\jcdctmgr.c %JPGSRC%\jchuff.c %JPGSRC%\jcinit.c %JPGSRC%\jcmainct.c ^
%JPGSRC%\jcmarker.c %JPGSRC%\jcmaster.c %JPGSRC%\jcomapi.c %JPGSRC%\jcparam.c %JPGSRC%\jcphuff.c %JPGSRC%\jcprepct.c %JPGSRC%\jcsample.c %JPGSRC%\jctrans.c ^
%JPGSRC%\jdapimin.c %JPGSRC%\jdapistd.c %JPGSRC%\jdatadst.c %JPGSRC%\jdatasrc.c %JPGSRC%\jdcoefct.c %JPGSRC%\jdcolor.c %JPGSRC%\jddctmgr.c ^
%JPGSRC%\jdhuff.c %JPGSRC%\jdinput.c %JPGSRC%\jdmainct.c %JPGSRC%\jdmarker.c %JPGSRC%\jdmaster.c %JPGSRC%\jdmerge.c %JPGSRC%\jdphuff.c %JPGSRC%\jdpostct.c ^
%JPGSRC%\jdsample.c %JPGSRC%\jdtrans.c %JPGSRC%\jerror.c %JPGSRC%\jfdctflt.c %JPGSRC%\jfdctfst.c %JPGSRC%\jfdctint.c %JPGSRC%\jidctflt.c %JPGSRC%\jidctfst.c ^
%JPGSRC%\jidctint.c %JPGSRC%\jidctred.c %JPGSRC%\jmemmgr.c %JPGSRC%\jmemnobs.c %JPGSRC%\jquant1.c %JPGSRC%\jquant2.c %JPGSRC%\jutils.c


REM **************************************** ZLIB SOURCE CODE FILES
set ZLIB_SRC=%ZLIBPATH%/deflate.c %ZLIBPATH%/gzio.c %ZLIBPATH%/infback.c %ZLIBPATH%/inffast.c %ZLIBPATH%/inflate.c %ZLIBPATH%/inftrees.c %ZLIBPATH%/trees.c %ZLIBPATH%/uncompr.c %ZLIBPATH%/zutil.c %ZLIBPATH%/adler32.c %ZLIBPATH%/compress.c %ZLIBPATH%/crc32.c


REM **************************************** PARTICLE SYSTEM SOURCE CODE FILES
set PARTICLE_SRC=%PPATH%/L_Defination.cpp %PPATH%/L_DroppingEffect.cpp %PPATH%/L_EffectEmitter.cpp %PPATH%/L_ExplosionEffect.cpp %PPATH%/L_MotionController.cpp %PPATH%/L_Particle.cpp ^
%PPATH%/L_ParticleEffect.cpp %PPATH%/L_ParticleMem.cpp %PPATH%/L_ParticleSystem.cpp %PPATH%/L_ShootingEffect.cpp %PPATH%/L_EffectManager.cpp


REM **************************************** APP SOURCE CODE FILES
set APP_SRC=%APP%\App.cpp %APP%\Component\ParticleTestComponent.cpp %APP%\GUI\DebugMenu.cpp %APP%\GUI\EnterNameMenu.cpp %APP%\GUI\MainMenu.cpp %APP%\GUI\ParticleTestMenu.cpp ^
%APP%\GUI\AboutMenu.cpp %APP%\GUI\TouchTestMenu.cpp %APP%\Component\TouchTestComponent.cpp
REM **************************************** END SOURCE

set DEFINES=-D RT_WEBOS -D BOOST_ALL_NO_LIB  -D RT_WEBOS_ARM -D PLATFORM_WEBOS -D RT_JPG_SUPPORT


@rem List the libraries needed
set LIBS=-lSDL -lSDL_net -lSDL_image -lSDL_mixer -lpdl -lGLES_CM

@rem Name your output executable
set OUTFILE=%APPNAME%

set INCLUDE_DIRS=-I..\..\shared\util\boost -I%SHARED% -I%APP% "-I%PALMPDK%\include" "-I%PALMPDK%\include\SDL" -I..\..\shared\ClanLib-2.0\Sources -I%ZLIBPATH%

set LIB_DIRS="-L%PALMPDK%\device\lib"

set FLAGS=-O2

if %PRE% equ 0 if %PIXI% equ 0 goto :END


if %DEBUG% equ 1 (
   set DEVICEOPTS=-g
   set FLAGS=-O
   set DEFINES=%DEFINES% -D _DEBUG
) else (
   set DEVICEOPTS=
   set DEFINES=%DEFINES% -D NDEBUG
)


if %PRE% equ 1 (
   set DEVICEOPTS=%DEVICEOPTS% -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp
)

if %PIXI% equ 1 (
   set DEVICEOPTS=%DEVICEOPTS% -mcpu=arm1136jf-s -mfpu=vfp -mfloat-abi=softfp
)

echo %DEVICEOPTS%

arm-none-linux-gnueabi-gcc  %DEFINES% %DEVICEOPTS% %INCLUDE_DIRS% %LIB_DIRS% -o %OUTFILE% %APP_SRC% %SRC% %COMPONENT_SRC% %ZLIB_SRC% %JPG_SRC% %PARTICLE_SRC% -Wl,--allow-shlib-undefined %LIBS% -Wno-deprecated %FLAGS%
if not exist %APPNAME% beeper.exe /p

goto :EOF

:END
echo Please select the target device by editing the PRE/PIXI variable in this file.

REM exit /b 1





