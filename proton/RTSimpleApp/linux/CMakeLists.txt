cmake_minimum_required(VERSION 2.6)

project(rtsimpleapp)

set(SHARED "../../shared")
set(APP "../source")
set(COMPPATH "${SHARED}/Entity")
set(CLANMATH "${SHARED}/ClanLib-2.0/Sources/Core/Math")
set(BOOSTSIGNALSPATH "${SHARED}/util/boost/libs/signals/src")
set(PPATH "${SHARED}/Renderer/linearparticle/sources")
set(JPGSRC "${SHARED}/Irrlicht/source/Irrlicht/jpeglib")

add_definitions(-DRTLINUX -DBOOST_ALL_NO_LIB -DC_GL_MODE -DRT_JPG_SUPPORT -DRT_USE_SDL_AUDIO)

include_directories("${APP}")
include_directories("${SHARED}")
include_directories("${SHARED}/ClanLib-2.0/Sources")
include_directories("${SHARED}/util/boost")

add_executable(rtsimpleapp "${APP}/App.cpp" "${APP}/Component/ParticleTestComponent.cpp" "${APP}/GUI/DebugMenu.cpp" "${APP}/GUI/EnterNameMenu.cpp" "${APP}/GUI/MainMenu.cpp" "${APP}/GUI/ParticleTestMenu.cpp" "${APP}/GUI/AboutMenu.cpp" "${APP}/GUI/TouchTestMenu.cpp" "${APP}/Component/TouchTestComponent.cpp"
	"${SHARED}/BaseApp.cpp" "${SHARED}/PlatformSetup.cpp"
	"${SHARED}/linux/LinuxUtils.cpp" "${SHARED}/linux/LinuxMain.cpp" 
	"${SHARED}/util/VideoModeSelector.cpp" "${SHARED}/util/MathUtils.cpp" "${SHARED}/util/MiscUtils.cpp" "${SHARED}/util/ResourceUtils.cpp" "${SHARED}/util/RenderUtils.cpp" "${SHARED}/util/GLESUtils.cpp" "${SHARED}/util/Variant.cpp"
	"${SHARED}/FileSystem/FileManager.cpp" "${SHARED}/FileSystem/StreamingInstance.cpp" "${SHARED}/FileSystem/StreamingInstanceFile.cpp"
	"${SHARED}/GUI/RTFont.cpp"
	"${SHARED}/Audio/AudioManager.cpp" "${SHARED}/Audio/AudioManagerSDL.cpp"
	"${SHARED}/Manager/Console.cpp" "${SHARED}/Manager/GameTimer.cpp" "${SHARED}/Manager/MessageManager.cpp" "${SHARED}/Manager/ResourceManager.cpp" "${SHARED}/Manager/VariantDB.cpp" 
	"${SHARED}/Math/rtRect.cpp"
	"${SHARED}/Renderer/Surface.cpp" "${SHARED}/Renderer/SoftSurface.cpp" "${SHARED}/Renderer/SurfaceAnim.cpp" "${SHARED}/Renderer/RenderBatcher.cpp" "${SHARED}/Renderer/JPGSurfaceLoader.cpp"
	"${COMPPATH}/Entity.cpp" "${COMPPATH}/Component.cpp" "${COMPPATH}/Button2DComponent.cpp" "${COMPPATH}/EntityUtils.cpp"  "${COMPPATH}/CustomInputComponent.cpp" "${COMPPATH}/FocusRenderComponent.cpp" "${COMPPATH}/TouchHandlerComponent.cpp" "${COMPPATH}/FilterInputComponent.cpp" "${COMPPATH}/ScrollComponent.cpp" "${COMPPATH}/ScrollBarRenderComponent.cpp" "${COMPPATH}/RenderScissorComponent.cpp" "${COMPPATH}/OverlayRenderComponent.cpp" "${COMPPATH}/InterpolateComponent.cpp" "${COMPPATH}/TextRenderComponent.cpp" "${COMPPATH}/InputTextRenderComponent.cpp" "${COMPPATH}/RectRenderComponent.cpp" "${COMPPATH}/UnderlineRenderComponent.cpp" "${COMPPATH}/FocusInputComponent.cpp" "${COMPPATH}/FocusUpdateComponent.cpp" "${COMPPATH}/TextBoxRenderComponent.cpp" "${COMPPATH}/TyperComponent.cpp" "${COMPPATH}/SelectButtonWithCustomInputComponent.cpp" "${COMPPATH}/EmitVirtualKeyComponent.cpp" "${COMPPATH}/SliderComponent.cpp"
	"${CLANMATH}/angle.cpp" "${CLANMATH}/mat3.cpp" "${CLANMATH}/mat4.cpp" "${CLANMATH}/rect.cpp" "${CLANMATH}/vec2.cpp" "${CLANMATH}/vec3.cpp" "${CLANMATH}/vec4.cpp"
	"${BOOSTSIGNALSPATH}/connection.cpp" "${BOOSTSIGNALSPATH}/named_slot_map.cpp" "${BOOSTSIGNALSPATH}/signal_base.cpp" "${BOOSTSIGNALSPATH}/slot.cpp" "${BOOSTSIGNALSPATH}/trackable.cpp" 
	"${PPATH}/L_Defination.cpp" "${PPATH}/L_DroppingEffect.cpp" "${PPATH}/L_EffectEmitter.cpp" "${PPATH}/L_ExplosionEffect.cpp" "${PPATH}/L_MotionController.cpp" "${PPATH}/L_Particle.cpp" "${PPATH}/L_ParticleEffect.cpp" "${PPATH}/L_ParticleMem.cpp" "${PPATH}/L_ParticleSystem.cpp" "${PPATH}/L_ShootingEffect.cpp" "${PPATH}/L_EffectManager.cpp"
	"${JPGSRC}/jcapimin.c" "${JPGSRC}/jcapistd.c" "${JPGSRC}/jccoefct.c" "${JPGSRC}/jccolor.c" "${JPGSRC}/jcdctmgr.c" "${JPGSRC}/jchuff.c" "${JPGSRC}/jcinit.c" "${JPGSRC}/jcmainct.c" "${JPGSRC}/jcmarker.c" "${JPGSRC}/jcmaster.c" "${JPGSRC}/jcomapi.c" "${JPGSRC}/jcparam.c" "${JPGSRC}/jcphuff.c" "${JPGSRC}/jcprepct.c" "${JPGSRC}/jcsample.c" "${JPGSRC}/jctrans.c" "${JPGSRC}/jdapimin.c" "${JPGSRC}/jdapistd.c" "${JPGSRC}/jdatadst.c" "${JPGSRC}/jdatasrc.c" "${JPGSRC}/jdcoefct.c" "${JPGSRC}/jdcolor.c" "${JPGSRC}/jddctmgr.c" "${JPGSRC}/jdhuff.c" "${JPGSRC}/jdinput.c" "${JPGSRC}/jdmainct.c" "${JPGSRC}/jdmarker.c" "${JPGSRC}/jdmaster.c" "${JPGSRC}/jdmerge.c" "${JPGSRC}/jdphuff.c" "${JPGSRC}/jdpostct.c" "${JPGSRC}/jdsample.c" "${JPGSRC}/jdtrans.c" "${JPGSRC}/jerror.c" "${JPGSRC}/jfdctflt.c" "${JPGSRC}/jfdctfst.c" "${JPGSRC}/jfdctint.c" "${JPGSRC}/jidctflt.c" "${JPGSRC}/jidctfst.c" "${JPGSRC}/jidctint.c" "${JPGSRC}/jidctred.c" "${JPGSRC}/jmemmgr.c" "${JPGSRC}/jmemnobs.c" "${JPGSRC}/jquant1.c" "${JPGSRC}/jquant2.c" "${JPGSRC}/jutils.c")

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries(rtsimpleapp ${ZLIB_LIBRARIES})
endif(ZLIB_FOUND)

find_package(SDL REQUIRED)
if(SDL_FOUND)
  include_directories(${SDL_INCLUDE_DIR})
  target_link_libraries(rtsimpleapp ${SDL_LIBRARY})
endif(SDL_FOUND)

find_package(SDL_mixer REQUIRED)
if(SDLMIXER_FOUND)
  include_directories(${SDLMIXER_INCLUDE_DIR})
  target_link_libraries(rtsimpleapp ${SDLMIXER_LIBRARY})
else(SDLMIXER_FOUND)
  message(FATAL_ERROR "Couldn't find SDL_mixer. Make sure the SDL_mixer development headers are installed.")
endif(SDLMIXER_FOUND)

target_link_libraries(rtsimpleapp rt GL)
