#include "PlatformPrecomp.h"

bool g_forceAspectRatioWhenChangingWindowSize = true; //applicable to manually dragging corners/sides.  Can hold Shift while dragging to toggle.

bool GetForceAspectRatioWhenResizing()
{
	return g_forceAspectRatioWhenChangingWindowSize;
}
ePlatformID g_emulatedPlatformID = PLATFORM_ID_UNKNOWN;
eMarketID g_marketID = MARKET_ID_UNSPECIFIED;

//usually the same as PlatFormID, unless we're using one OS to test another OS's stuff
ePlatformID GetEmulatedPlatformID()
{
	if (g_emulatedPlatformID == PLATFORM_ID_UNKNOWN)
	{
		//assume no emulation, give them the real platform ID
		return GetPlatformID();
	}

	return g_emulatedPlatformID;
}

string AddPlatformNameURL()
{
	if (GetEmulatedPlatformID() != PLATFORM_ID_WINDOWS)
	{
		return "/"+GetPlatformName();
	}
	return "";
}

string PlatformIDAsString(ePlatformID platformID)
{

	switch (platformID)
	{
	case PLATFORM_ID_UNKNOWN: return "unknown";
	case PLATFORM_ID_ANDROID: return "android";
	case PLATFORM_ID_WEBOS: return "webos";
	case PLATFORM_ID_WINDOWS: return "windows";
	case PLATFORM_ID_LINUX: return "linux";
	case PLATFORM_ID_IOS: return "iphone";
	case PLATFORM_ID_OSX: return "osx";

	}

	return "";
}
string GetPlatformName()
{
	return PlatformIDAsString(GetEmulatedPlatformID());
}


void SetMarketID(eMarketID marketID)
{
	g_marketID = marketID;
}

eMarketID GetMarketID()
{
	return g_marketID;
}

void SetEmulatedPlatformID(ePlatformID platformID)
{
	g_emulatedPlatformID = platformID;
}

#ifdef _CONSOLE
float GetScreenSizeXf() { assert(0); return 0;}
float GetScreenSizeYf() { assert(0); return 0;}
int GetScreenSizeX() { assert(0); return 0;}
int GetScreenSizeY() { assert(0); return 0;}

#endif