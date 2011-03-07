#ifndef platformenums_h__
#define platformenums_h__

//these are defines and utility functions that are implemented on every platform. (Implemented in AndroidUtils.cpp, or WinUtils.cpp, or ioSUtils.cpp, etc)

#ifdef __cplusplus


#include <string>
#include <vector>

//using namespace std;

#define IsPixiSize ((GetScreenSizeX() == 400 && GetScreenSizeY() ==  320)|| (GetScreenSizeX() == 320 && GetScreenSizeY() ==  480))
#define IsNexusOneSize (GetScreenSizeX() == 800 || GetScreenSizeY() ==  480)
#define IsIphoneSize ((GetScreenSizeX() == 320 && GetScreenSizeY() ==  480)|| (GetScreenSizeX() == 480 && GetScreenSizeY() ==  320))
#define IsIphone4Size ((GetScreenSizeX() == 640 && GetScreenSizeY() ==  960)|| (GetScreenSizeX() == 960 && GetScreenSizeY() ==  640))
#define IsIPADSize ((GetScreenSizeX() == 1024 && GetScreenSizeY() ==  768)|| (GetScreenSizeX() == 1024 && GetScreenSizeY() ==  768))

enum eNetworkType
{
	C_NETWORK_NONE,
	C_NETWORK_CELL,
	C_NETWORK_WIFI
};


enum eDeviceMemoryClass
{
	C_DEVICE_MEMORY_CLASS_1, //iphone 3g, ipod touch, 128 mb
	C_DEVICE_MEMORY_CLASS_2, //iphone 3gs, iPad, ipod touch 3g, 256 mb
	C_DEVICE_MEMORY_CLASS_3, //iphone 4, iPod Touch 4g, 512 mb
	C_DEVICE_MEMORY_CLASS_4, //PC, Linux, OSX.. assuming to be a lot
};

enum ePlatformID
{
	PLATFORM_ID_UNKNOWN = -1,
	PLATFORM_ID_WINDOWS,
	PLATFORM_ID_IOS, //iPhone/iPad etc
	PLATFORM_ID_OSX,
	PLATFORM_ID_LINUX,
	PLATFORM_ID_ANDROID,
	PLATFORM_ID_WINDOWS_MOBILE, //yeah, right.  Doesn't look like we'll be porting here anytime soon.
	PLATFORM_ID_WEBOS

};

enum eMarketID
{
	MARKET_ID_UNSPECIFIED,
	MARKET_ID_ANDROID_MARKET, 
	MARKET_ID_APPLE_APPSTORE, 
	MARKET_AMAZON_MARKET
};

enum eOrientationMode
{
	//these match the iphone versions
	ORIENTATION_PORTRAIT = 1,
	ORIENTATION_PORTRAIT_UPSIDE_DOWN,
	ORIENTATION_LANDSCAPE_LEFT,
	ORIENTATION_LANDSCAPE_RIGHT
};


//stuff available to platform specific code:

ePlatformID GetPlatformID();
ePlatformID GetEmulatedPlatformID(); //defaults to GetPlatformID if not set

//platform include must support these functions:
void NotifyOSOfOrientationPreference(eOrientationMode orientation);

std::string GetBaseAppPath();
std::string GetSavePath();
std::string GetAppCachePath(); //on iPhone, returns path to save user data that isn't backed up, on Android, returns external sd card path or "" if not available
void CreateAppCacheDirIfNeeded();
int GetYOffset();
unsigned int GetSystemTimeTick();
double GetSystemTimeAccurate();
unsigned int GetFreeMemory();
void LaunchURL(std::string url);
void LogMsg(const char *lpFormat, ...);
void SetLastStringInput(std::string s);
std::string GetLastStringInput();
void RemoveFile(std::string fileName, bool bAddSavePath = true);
void CreateDirectoryRecursively(std::string basePath, std::string path);
bool RemoveDirectoryRecursively(std::string path);
std::vector<std::string> GetDirectoriesAtPath(std::string path);
std::vector<std::string> GetFilesAtPath(std::string path);
std::string GetRegionString();

bool IsIphone();
bool IsIPAD();
bool IsIphoneOriPad();
bool IsIPhone3GS();
bool IsIphone4(); //also returns true if a iPod Touch g4, or anything else with retina screen dimensions
int GetPrimaryGLX();
int GetPrimaryGLY();
#define C_PIRATED_YES 0
#define C_PIRATED_NO 18263
int GetSystemData(); //returns C_PIRATED_YES or C_PIRATED_NO
void CreateDirectoryRecursively(std::string basePath, std::string path);
eNetworkType IsNetReachable(std::string url);
void InitDeviceScreenInfo();
eDeviceMemoryClass GetDeviceMemoryClass();
bool IsIPodTouchThirdGen();
std::string GetClipboardText();
bool IsDesktop();
bool HasVibration();

#endif // platformenums_h__
#endif