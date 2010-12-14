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

#ifdef RT_WEBOS_ARM
#include "WebOS/PlatformSetupWebOS.h"
#endif

#ifdef ANDROID_NDK
#include "android/PlatformSetupAndroid.h"
#endif

#if defined(__cplusplus) || defined(__OBJC__)
	#include <cstdio>
	#include <string>
	#include <vector>
	#include <cmath>
	#include <deque>
	#include <cassert>
	#include <list>
	#include <map>
	#include <deque>
	#include <stdlib.h>
	#include <iostream>
	#include <sstream>
	using namespace std;
	



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
void LogError(const char *lpFormat, ...);

void SetEmulatedPlatformID(ePlatformID platformID);
string AddPlatformNameURL();
string GetPlatformName();

void SetMarketID(eMarketID marketID);
eMarketID GetMarketID();
#endif