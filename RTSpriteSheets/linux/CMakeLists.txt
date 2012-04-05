cmake_minimum_required(VERSION 2.6)

project(rtspritesheets)

include(../../shared/linux/Proton.cmake)

set(APP "../source")
include_directories("${APP}")

proton_include_all_components()
proton_set_sources("${APP}/App.cpp" "${APP}/GUI/GUICommon.cpp" "${APP}/GUI/MainMenu.cpp" "${APP}/GUI/BasicSpriteFramesScreen.cpp" "${APP}/GUI/BasicSpriteAnimationScreen.cpp" "${APP}/GUI/SpriteAnimationScreen.cpp")
