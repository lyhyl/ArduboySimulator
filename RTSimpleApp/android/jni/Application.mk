# The ARMv7 is significanly faster due to the use of the hardware FPU
APP_STL := gnustl_static
APP_CPPFLAGS = -fexceptions
#STLPORT_FORCE_REBUILD := trueAPP_OPTIM=release
APP_ABI := armeabi armeabi-v7a
