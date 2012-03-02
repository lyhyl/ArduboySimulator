cmake_minimum_required(VERSION 2.6)

project(rtbarebones)

set(SHARED "../../shared")
set(APP "../source")
set(COMPPATH "${SHARED}/Entity")
set(CLANMATH "${SHARED}/ClanLib-2.0/Sources/Core/Math")
set(BOOSTSIGNALSPATH "${SHARED}/util/boost/libs/signals/src")

add_definitions(-DRTLINUX -DBOOST_ALL_NO_LIB -DC_GL_MODE)

include_directories("${SHARED}")
include_directories("${SHARED}/ClanLib-2.0/Sources")
include_directories("${SHARED}/util/boost")

add_executable(rtbarebones "${APP}/App.cpp"
	"${SHARED}/PlatformSetup.cpp" "${SHARED}/linux/LinuxUtils.cpp" "${SHARED}/linux/LinuxMain.cpp"  "${SHARED}/BaseApp.cpp" "${SHARED}/Renderer/Surface.cpp" "${SHARED}/Audio/AudioManager.cpp" "${SHARED}/GUI/RTFont.cpp" "${SHARED}/Manager/Console.cpp" "${SHARED}/FileSystem/FileManager.cpp" "${SHARED}/Manager/GameTimer.cpp" "${SHARED}/Manager/MessageManager.cpp" "${SHARED}/Manager/ResourceManager.cpp" "${SHARED}/Manager/VariantDB.cpp" "${SHARED}/Math/rtPlane.cpp" "${SHARED}/Math/rtRect.cpp" "${SHARED}/Renderer/RenderBatcher.cpp" "${SHARED}/Renderer/SoftSurface.cpp" "${SHARED}/Renderer/SurfaceAnim.cpp" "${SHARED}/util/VideoModeSelector.cpp" "${SHARED}/util/PassThroughPointerEventHandler.cpp" "${SHARED}/util/TouchDeviceEmulatorPointerEventHandler.cpp" "${SHARED}/util/CRandom.cpp" "${SHARED}/util/GLESUtils.cpp" "${SHARED}/util/MathUtils.cpp" "${SHARED}/util/MiscUtils.cpp" "${SHARED}/util/RenderUtils.cpp" "${SHARED}/util/ResourceUtils.cpp" "${SHARED}/util/Variant.cpp" "${SHARED}/FileSystem/FileSystem.cpp" "${SHARED}/FileSystem/FileSystemZip.cpp" "${SHARED}/util/unzip/unzip.c" "${SHARED}/util/unzip/ioapi.c" "${SHARED}/FileSystem/StreamingInstance.cpp" "${SHARED}/FileSystem/StreamingInstanceZip.cpp" "${SHARED}/FileSystem/StreamingInstanceFile.cpp"
	"${COMPPATH}/Entity.cpp" "${COMPPATH}/Component.cpp" "${COMPPATH}/FocusInputComponent.cpp" "${COMPPATH}/FocusRenderComponent.cpp" "${COMPPATH}/FocusUpdateComponent.cpp" "${COMPPATH}/CustomInputComponent.cpp" "${COMPPATH}/ArcadeInputComponent.cpp" "${COMPPATH}/RectRenderComponent.cpp"
	"${CLANMATH}/angle.cpp" "${CLANMATH}/mat3.cpp" "${CLANMATH}/mat4.cpp" "${CLANMATH}/rect.cpp" "${CLANMATH}/vec2.cpp" "${CLANMATH}/vec3.cpp" "${CLANMATH}/vec4.cpp"
	"${BOOSTSIGNALSPATH}/connection.cpp" "${BOOSTSIGNALSPATH}/named_slot_map.cpp" "${BOOSTSIGNALSPATH}/signal_base.cpp" "${BOOSTSIGNALSPATH}/slot.cpp" "${BOOSTSIGNALSPATH}/trackable.cpp")

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries(rtbarebones ${ZLIB_LIBRARIES})
endif(ZLIB_FOUND)

find_package(SDL REQUIRED)
if(SDL_FOUND)
  include_directories(${SDL_INCLUDE_DIR})
  target_link_libraries(rtbarebones ${SDL_LIBRARY})
endif(SDL_FOUND)

target_link_libraries(rtbarebones rt GL)
