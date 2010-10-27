#pragma once

const char * GetAppName();
extern bool	g_bAppFinished;
void InitVideoSize();
extern int winVideoScreenX;
extern int winVideoScreenY;
extern bool g_landScapeNoNeckHurtMode;
extern string g_videoModeNameForce; //if set, video mode will be forced to this instead of what is set in main.cpp

class VideoModeEntry
{
public:
	VideoModeEntry(string _name, int _x, int _y, ePlatformID _platformID) : name(_name), x(_x), y(_y), platformID (_platformID){};
	string name;
	int x, y;
	ePlatformID platformID;
};
