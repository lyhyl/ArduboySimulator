#include "PlatformPrecomp.h"

ePlatformID g_emulatedPlatformID = PLATFORM_ID_UNKNOWN;


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
string GetPlatformName()
{
	switch (GetEmulatedPlatformID())
	{
		case PLATFORM_ID_UNKNOWN: return "unknown";
		case PLATFORM_ID_ANDROID: return "android";
		case PLATFORM_ID_WEBOS: return "webos";
		case PLATFORM_ID_WINDOWS: return "windows";
		case PLATFORM_ID_LINUX: return "linux";
		case PLATFORM_ID_IOS: return "iphone";
			
	}
	return "";
}

void SetEmulatedPlatformID(ePlatformID platformID)
{
	g_emulatedPlatformID = platformID;
}
