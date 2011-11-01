#ifdef WIN32
#include "win/PlatformSetupWin.h"
#endif

#ifdef __APPLE__

#include "TargetConditionals.h"

	#if TARGET_OS_IPHONE == 1
		#include "iOS/PlatformSetupIOS.h"
	#else
		#include "OSX/PlatformSetupOSX.h"
	#endif
#endif
#ifdef RTLINUX
#include "linux/PlatformSetupLinux.h"

#endif

#ifdef RT_WEBOS_ARM
#include "WebOS/PlatformSetupWebOS.h"
#endif

#ifdef ANDROID_NDK
#include "android/PlatformSetupAndroid.h"
#endif

#ifdef PLATFORM_BBX
#include "bbx/PlatformSetupBBX.h"
#endif

#if defined(__cplusplus) || defined(__OBJC__)
	#include <cstdio>
	#include <string>
	#include <vector>
	#include <cmath>
	#include <deque>
	#include <cassert>
	#include <map>
	#include <deque>
	#include <stdlib.h>
	#include <iostream>
	#include <sstream>

//hack for making irrBullet compile without needing irr::core prefix on its
//list.  It's because I include this file in the irrlicht main config stuff.
	
#ifdef _CONSOLE
#include <list>
using namespace std;
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)      { if(p) { delete (p); (p)=NULL; } }
#endif

#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

#ifndef SAFE_FREE
#define SAFE_FREE(p)      { if(p) { free (p); (p)=NULL; } }
#endif

#define MAKE_RGB(r, g, b)      ( ((uint32)(r) << 8) + ((uint32)(g) << 16) + ((uint32)(b) << 24) )
#define MAKE_RGBA(r, g, b, a) ( ((uint32)(r) << 8) + ((uint32)(g) << 16) + ((uint32)(b) << 24) + ((uint32)(a)))


#define GET_BLUE(p)        ( (p)               >> 24)
#define GET_GREEN(p)          (((p) & 0x00FF0000) >> 16)
#define GET_RED(p)        (((p) & 0x0000FF00) >>  8)
#define GET_ALPHA(p)         ( (p) & 0x000000FF       )

#define DEG2RAD(x) (M_PI * (x) / 180.0)
#define RAD2DEG(x) (x * (180/M_PI))


//this must exist somewhere, used for log messages	
void LogMsg ( const char* traceStr, ... );
void LogError(const char* traceStr, ...);

void SetEmulatedPlatformID(ePlatformID platformID);
std::string AddPlatformNameURL();
std::string GetPlatformName();

void SetMarketID(eMarketID marketID);
eMarketID GetMarketID();
bool GetForceAspectRatioWhenResizing();

//copy these here, so I don't have to include ResourceUtils.cpp in my console only apps
int GetScreenSizeY();
int GetScreenSizeX();
float GetScreenSizeYf();
float GetScreenSizeXf();
bool IsLargeScreen();
std::string PlatformIDAsString(ePlatformID platformID); //will never change
std::string PlatformIDAsStringDisplay(ePlatformID platformID);  //may change..
#endif
