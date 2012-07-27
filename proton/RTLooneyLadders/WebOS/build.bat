Cls
call app_info_setup.bat

REM arm-none-linux-gnueabi-g++ -o tiltodemo ..\common\accelerometer.cpp ..\common\collision.cpp ..\common\gamelogic.cpp ..\common\geometry.cpp ..\common\graphics_ogl.cpp ..\common\graphics_sdl.cpp ..\common\main.cpp ..\common\sdl_init.cpp ..\common\sound.cpp "-I%PalmPDK%\include" "-I%PalmPDK%\include\SDL" "-L%PalmPDK%\device\lib" -Wl,--allow-shlib-undefined -lSDL -lSDL_net -lSDL_image -lSDL_mixer -lpdl -lGLES_CM

REM @echo off
@rem Set the device you want to build for to 1
@rem Use Pixi to allow running on either device
set PRE=0
set PIXI=1
set DEBUG=0

set SHARED=..\..\shared
set APP=..\source
set CLANMATH=..\..\shared\ClanLib-2.0\Sources\Core\Math
set ZLIBPATH=..\..\shared\util\zlib
set PPATH=..\..\shared\Renderer\linearparticle\sources
set COMPPATH=..\..\shared\Entity

REM I really can't figure out how to make it accept a source file search path.. grr.  So I'll prepend for each.  If you know how tell me! -Seth
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
%SHARED%\FileSystem\FileSystemZip.cpp %SHARED%\FileSystem\FileManager.cpp

REM **************************************** ENGINE COMPONENT SOURCE CODE FILES
set COMPONENT_SRC=%COMPPATH%\Button2DComponent.cpp %COMPPATH%\FilterInputComponent.cpp %COMPPATH%\FocusInputComponent.cpp %COMPPATH%\FocusRenderComponent.cpp %COMPPATH%\FocusUpdateComponent.cpp ^
%COMPPATH%\HTTPComponent.cpp %COMPPATH%\InputTextRenderComponent.cpp %COMPPATH%\InterpolateComponent.cpp %COMPPATH%\OverlayRenderComponent.cpp %COMPPATH%\ProgressBarComponent.cpp ^
%COMPPATH%\RectRenderComponent.cpp %COMPPATH%\ScrollBarRenderComponent.cpp %COMPPATH%\ScrollComponent.cpp %COMPPATH%\TapSequenceDetectComponent.cpp %COMPPATH%\TextBoxRenderComponent.cpp ^
%COMPPATH%\TextRenderComponent.cpp %COMPPATH%\TouchStripComponent.cpp %COMPPATH%\TrailRenderComponent.cpp %COMPPATH%\TyperComponent.cpp %COMPPATH%\UnderlineRenderComponent.cpp ^
%COMPPATH%\TouchHandlerComponent.cpp %COMPPATH%\CustomInputComponent.cpp %COMPPATH%\SelectButtonWithCustomInputComponent.cpp %COMPPATH%\SliderComponent.cpp %COMPPATH%\RenderScissorComponent.cpp


REM **************************************** ZLIB SOURCE CODE FILES
set ZLIB_SRC=%ZLIBPATH%/deflate.c %ZLIBPATH%/gzio.c %ZLIBPATH%/infback.c %ZLIBPATH%/inffast.c %ZLIBPATH%/inflate.c %ZLIBPATH%/inftrees.c %ZLIBPATH%/trees.c %ZLIBPATH%/uncompr.c %ZLIBPATH%/zutil.c %ZLIBPATH%/adler32.c %ZLIBPATH%/compress.c %ZLIBPATH%/crc32.c


REM **************************************** PARTICLE SYSTEM SOURCE CODE FILES
set PARTICLE_SRC=%PPATH%/L_Defination.cpp %PPATH%/L_DroppingEffect.cpp %PPATH%/L_EffectEmitter.cpp %PPATH%/L_ExplosionEffect.cpp %PPATH%/L_MotionController.cpp %PPATH%/L_Particle.cpp ^
%PPATH%/L_ParticleEffect.cpp %PPATH%/L_ParticleMem.cpp %PPATH%/L_ParticleSystem.cpp %PPATH%/L_ShootingEffect.cpp %PPATH%/L_EffectManager.cpp


REM **************************************** APP SOURCE CODE FILES
set APP_SRC=%APP%\App.cpp %APP%\Component\ParticleTestComponent.cpp %APP%\GUI\DebugMenu.cpp %APP%\GUI\EnterNameMenu.cpp %APP%\GUI\MainMenu.cpp %APP%\GUI\ParticleTestMenu.cpp ^
%APP%\GUI\AboutMenu.cpp
REM **************************************** END SOURCE

set DEFINES=-D RT_WEBOS -D BOOST_ALL_NO_LIB  -D RT_WEBOS_ARM -D NDEBUG


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
) else (
   set DEVICEOPTS=
)

if %PRE% equ 1 (
   set DEVICEOPTS=%DEVICEOPTS% -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp
)

if %PIXI% equ 1 (
   set DEVICEOPTS=%DEVICEOPTS% -mcpu=arm1136jf-s -mfpu=vfp -mfloat-abi=softfp
)

echo %DEVICEOPTS%

arm-none-linux-gnueabi-gcc  %DEFINES% %DEVICEOPTS% %INCLUDE_DIRS% %LIB_DIRS% -o %OUTFILE% %APP_SRC% %SRC% %COMPONENT_SRC% %ZLIB_SRC% %PARTICLE_SRC% -Wl,--allow-shlib-undefined %LIBS% -Wno-deprecated %FLAGS%
if not exist %APPNAME% beeper.exe /p

goto :EOF

:END
echo Please select the target device by editing the PRE/PIXI variable in this file.

REM exit /b 1





