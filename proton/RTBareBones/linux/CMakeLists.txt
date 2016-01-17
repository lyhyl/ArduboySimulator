cmake_minimum_required(VERSION 2.6)
OPTION(RASPBERRYPI_GLES11 "Compile for Raspberry PI 2" ON)
set(CMAKE_BUILD_TYPE "Debug") #use Release or Debug

project(rtbarebones)
include(../../shared/linux/Proton.cmake)
include(../source)

proton_include_components(FocusInputComponent)
proton_include_components(CustomInputComponent ArcadeInputComponent)

set(APP "../source")
proton_set_sources("${APP}/App.cpp")
target_link_libraries(rtbarebones)
