# The ARMv7 is significanly faster due to the use of the hardware FPU
APP_STL := gnustl_static
#STLPORT_FORCE_REBUILD := true
#APP_OPTIM=release
APP_OPTIM=debug
APP_ABI := armeabi
