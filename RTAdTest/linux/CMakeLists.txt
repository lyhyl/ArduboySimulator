cmake_minimum_required(VERSION 2.6)

project(rtadtest)

include(../../shared/linux/Proton.cmake)

proton_include_all_components()
proton_use_iap()
proton_use_ad_framework()
proton_include_testing(GUI)

add_definitions(-DDISABLE_CHARTBOOST)

set(APP "../source")
include_directories("${APP}")
proton_set_sources("${APP}/App.cpp" "${APP}/MenuMain.cpp" "${APP}/MenuPurchase.cpp" "${APP}/MenuStore.cpp" "${APP}/MenuAdWait.cpp" "${APP}/MenuTapjoy.cpp" "${APP}/MenuTest.cpp")
