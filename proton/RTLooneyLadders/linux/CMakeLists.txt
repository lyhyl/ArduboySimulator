cmake_minimum_required(VERSION 2.6)

project(rtlooneyladders)

set(SHARED "../../shared")
set(APP "../source")
set(COMPPATH "${SHARED}/Entity")
set(CLANMATH "${SHARED}/ClanLib-2.0/Sources/Core/Math")
set(BOOSTSIGNALSPATH "${SHARED}/util/boost/libs/signals/src")
set(PPATH "${SHARED}/Renderer/linearparticle/sources")

add_definitions(-DRTLINUX -DBOOST_ALL_NO_LIB -DC_GL_MODE -DRT_USE_SDL_AUDIO)

include_directories("${APP}")
include_directories("${SHARED}")
include_directories("${SHARED}/ClanLib-2.0/Sources")
include_directories("${SHARED}/util/boost")

add_executable(rtlooneyladders "${APP}/App.cpp" "${APP}/Component/BuildingComponent.cpp" "${APP}/Component/CharComponent.cpp" "${APP}/Component/CharManagerComponent.cpp" "${APP}/Component/ExplosionComponent.cpp" "${APP}/Component/OverlayRenderComponentSpy.cpp" "${APP}/GUI/AboutMenu.cpp" "${APP}/GUI/GameMenu.cpp" "${APP}/GUI/MainMenu.cpp"
"${SHARED}/PlatformSetup.cpp" "${SHARED}/BaseApp.cpp"
"${SHARED}/linux/LinuxUtils.cpp" "${SHARED}/linux/LinuxMain.cpp"
"${SHARED}/Audio/AudioManager.cpp" "${SHARED}/Audio/AudioManagerSDL.cpp"
"${SHARED}/GUI/RTFont.cpp"
"${SHARED}/Manager/Console.cpp" "${SHARED}/Manager/GameTimer.cpp" "${SHARED}/Manager/MessageManager.cpp" "${SHARED}/Manager/ResourceManager.cpp" "${SHARED}/Manager/VariantDB.cpp"
"${SHARED}/Math/rtPlane.cpp" "${SHARED}/Math/rtRect.cpp"
"${SHARED}/Renderer/RenderBatcher.cpp" "${SHARED}/Renderer/SoftSurface.cpp" "${SHARED}/Renderer/Surface.cpp" "${SHARED}/Renderer/SurfaceAnim.cpp"
"${SHARED}/util/CRandom.cpp" "${SHARED}/util/GLESUtils.cpp" "${SHARED}/util/MathUtils.cpp" "${SHARED}/util/MiscUtils.cpp" "${SHARED}/util/RenderUtils.cpp" "${SHARED}/util/ResourceUtils.cpp" "${SHARED}/util/Variant.cpp" "${SHARED}/util/TextScanner.cpp"
"${SHARED}/util/unzip/unzip.c" "${SHARED}/util/unzip/ioapi.c"
"${SHARED}/Network/NetHTTP.cpp" "${SHARED}/Network/NetSocket.cpp" "${SHARED}/Network/NetUtils.cpp"
"${SHARED}/FileSystem/StreamingInstance.cpp" "${SHARED}/FileSystem/StreamingInstanceZip.cpp" "${SHARED}/FileSystem/StreamingInstanceFile.cpp" "${SHARED}/FileSystem/FileSystem.cpp" "${SHARED}/FileSystem/FileSystemZip.cpp" "${SHARED}/FileSystem/FileManager.cpp"
"${CLANMATH}/angle.cpp" "${CLANMATH}/mat3.cpp" "${CLANMATH}/mat4.cpp" "${CLANMATH}/rect.cpp" "${CLANMATH}/vec2.cpp" "${CLANMATH}/vec3.cpp" "${CLANMATH}/vec4.cpp"
"${BOOSTSIGNALSPATH}/connection.cpp" "${BOOSTSIGNALSPATH}/named_slot_map.cpp" "${BOOSTSIGNALSPATH}/signal_base.cpp" "${BOOSTSIGNALSPATH}/slot.cpp" "${BOOSTSIGNALSPATH}/trackable.cpp" 
"${COMPPATH}/EntityUtils.cpp" "${COMPPATH}/Entity.cpp" "${COMPPATH}/Component.cpp" "${COMPPATH}/Button2DComponent.cpp" "${COMPPATH}/FilterInputComponent.cpp" "${COMPPATH}/FocusInputComponent.cpp" "${COMPPATH}/FocusRenderComponent.cpp" "${COMPPATH}/FocusUpdateComponent.cpp" "${COMPPATH}/HTTPComponent.cpp" "${COMPPATH}/InputTextRenderComponent.cpp" "${COMPPATH}/InterpolateComponent.cpp" "${COMPPATH}/OverlayRenderComponent.cpp" "${COMPPATH}/ProgressBarComponent.cpp" "${COMPPATH}/RectRenderComponent.cpp" "${COMPPATH}/ScrollBarRenderComponent.cpp" "${COMPPATH}/ScrollComponent.cpp" "${COMPPATH}/TapSequenceDetectComponent.cpp" "${COMPPATH}/TextBoxRenderComponent.cpp" "${COMPPATH}/TextRenderComponent.cpp" "${COMPPATH}/TouchStripComponent.cpp" "${COMPPATH}/TrailRenderComponent.cpp" "${COMPPATH}/TyperComponent.cpp" "${COMPPATH}/UnderlineRenderComponent.cpp" "${COMPPATH}/TouchHandlerComponent.cpp" "${COMPPATH}/SelectButtonWithCustomInputComponent.cpp" "${COMPPATH}/CustomInputComponent.cpp" "${COMPPATH}/SliderComponent.cpp" "${COMPPATH}/ArcadeInputComponent.cpp" "${COMPPATH}/EmitVirtualKeyComponent.cpp" "${COMPPATH}/DPadComponent.cpp"
"${PPATH}/L_Defination.cpp" "${PPATH}/L_DroppingEffect.cpp" "${PPATH}/L_EffectEmitter.cpp" "${PPATH}/L_ExplosionEffect.cpp" "${PPATH}/L_MotionController.cpp" "${PPATH}/L_Particle.cpp" "${PPATH}/L_ParticleEffect.cpp" "${PPATH}/L_ParticleMem.cpp" "${PPATH}/L_ParticleSystem.cpp" "${PPATH}/L_ShootingEffect.cpp" "${PPATH}/L_EffectManager.cpp")

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries(rtlooneyladders ${ZLIB_LIBRARIES})
endif(ZLIB_FOUND)

find_package(SDL REQUIRED)
if(SDL_FOUND)
  include_directories(${SDL_INCLUDE_DIR})
  target_link_libraries(rtlooneyladders ${SDL_LIBRARY})
endif(SDL_FOUND)

find_package(SDL_mixer REQUIRED)
if(SDLMIXER_FOUND)
  include_directories(${SDLMIXER_INCLUDE_DIR})
  target_link_libraries(rtlooneyladders ${SDLMIXER_LIBRARY})
else(SDLMIXER_FOUND)
  message(FATAL_ERROR "Couldn't find SDL_mixer. Make sure the SDL_mixer development headers are installed.")
endif(SDLMIXER_FOUND)

target_link_libraries(rtlooneyladders rt GL)
