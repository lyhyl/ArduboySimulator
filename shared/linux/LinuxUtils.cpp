#include "LinuxUtils.h"
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/time.h>
#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include "PlatformSetup.h"

const char * GetAppName();
const char * GetBundlePrefix();
const char * GetBundleName();

bool g_preferSDCardForUserStorage = false;

uint32 g_callAppResumeASAPTimer = 0;
bool g_pauseASAP = false;
string g_musicToPlay;
int g_musicPos = 0;

//this delay fixes a problem with restoring surfaces before Linux 1.6 devices are ready for it resulting
//in white textures -  update: unneeded
#define C_DELAY_BEFORE_RESTORING_SURFACES_MS 1

//out how to make/pass java structs

bool g_landScapeNoNeckHurtMode = false;

int g_winVideoScreenX = 0;
int g_winVideoScreenY = 0;


void StringReplace(const std::string& what, const std::string& with, std::string& in);
vector<string> StringTokenize (const  string  & theString,  const  string  & theDelimiter );

string GetDateAndTimeAsString();
void AppendStringToFile(string filename, string text);

void LogMsg ( const char* traceStr, ... )
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	memset ( (void*)buffer, 0, logSize );

	va_start ( argsVA, traceStr );
	vsnprintf( buffer, logSize, traceStr, argsVA );
	va_end( argsVA );
	
	
	//__Linux_log_write(Linux_LOG_ERROR,GetAppName(), buffer);
	printf ((char*)buffer);
	printf ("\r\n");
	fflush(stdout);

	AppendStringToFile( GetBaseAppPath()+"log.txt", GetDateAndTimeAsString()+": "+string(buffer)+"\r\n");

}


void LogError ( const char* traceStr, ... )
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	memset ( (void*)buffer, 0, logSize );

	va_start ( argsVA, traceStr );
	vsnprintf( buffer, logSize, traceStr, argsVA );
	va_end( argsVA );


	//__Linux_log_write(Linux_LOG_ERROR,GetAppName(), buffer);
	LogMsg( "ERROR: %s", buffer);
	AppendStringToFile( GetBaseAppPath()+"error.txt", GetDateAndTimeAsString()+": "+string(buffer)+"\r\n");
	//printf ((char*)buffer);
}


string GetBaseAppPath()
{
	char szDir[1024];
	getcwd( szDir, 1024);
	return string(szDir)+"/";

}


string GetSavePath()
{
	return "";
}


//returns the SD card user save path (will be deleted when app is uninstalled on 2.2+)
//returns "" if no SD card/external writable storage available

string GetAppCachePath()
{
	
	return ""; //invalid
}

void LaunchEmail(string subject, string content)
{
}

void LaunchURL(string url)
{
	
}

string GetClipboardText()
{
	return "";
}

bool IsIPhone3GS()
{
	return false;
}

bool IsDesktop()
{
	if (GetEmulatedPlatformID() == PLATFORM_ID_LINUX) return false;
	return true;
}

ePlatformID GetPlatformID()
{
	return PLATFORM_ID_LINUX;
}

bool IsIphoneOriPad()
{
	return false;
}

bool IsIphone()
{
	return false;
}

bool IsIphone4()
{
	return false; 
}

bool IsIPAD()
{
	return false;
}

eDeviceMemoryClass GetDeviceMemoryClass()
{
	return C_DEVICE_MEMORY_CLASS_1;
}

int GetYOffset()
{
	return 0;
}

unsigned int GetSystemTimeTick()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_usec/1000 + tv.tv_sec*1000;
}

string GetDateAndTimeAsString()
{
	time_t ltime;
	time( &ltime );

	tm today = *localtime( &ltime );

	char stTemp[128];
	sprintf(stTemp, "%d/%d at %d:%d:%d", today.tm_mday, today.tm_mon+1, today.tm_hour, today.tm_min, today.tm_sec);
	return string(stTemp);
}

double GetSystemTimeAccurate()
{
	return double(GetSystemTimeTick());
	return 0;
}

unsigned int GetFreeMemory()
{
	return 0; //don't care on the PC
}

string g_string;

void SetLastStringInput( string s )
{
	g_string = s;
}

std::string GetLastStringInput()
{
	return g_string;
}

eNetworkType IsNetReachable( string url )
{
	return C_NETWORK_WIFI;
}

int GetSystemData()
{
	return C_PIRATED_NO;
}

void RemoveFile( string fileName, bool bAddSavePath)
{
	
	if (bAddSavePath)
	{
		fileName = GetSavePath()+fileName;
	}

	if (unlink(fileName.c_str()) == -1)
	{
		switch (errno)
		{
		case EACCES: 
			LogMsg("Warning: Unable to delete file %s, no access", fileName.c_str());
			break;
		case EBUSY: 
			LogError("Warning: Unable to delete file %s, file is being used", fileName.c_str());
			break;
		case EPERM: 
			LogMsg("Warning: Unable to delete file %s, may be a dir", fileName.c_str());
			break;
		case EROFS: 
			LogMsg("Warning: Unable to delete file %s, File system is read only", fileName.c_str());
			break;
		default:
			//LogMsg("Warning: Unable to delete file %s, unknown error", fileName.c_str());
			//file doesn't exist
			break;
		}
	}
}

string GetRegionString()
{
	return "us_en";
}

//month is 1-12 btw
int GetDaysSinceDate(int month,int day, int year)
{
	//LogMsg("GetDaysSinceDate url not done");
	assert(!"no!");
	return 0;
}

bool RTCreateDirectory(const std::string &dir_name)
{
#ifdef _DEBUG
	LogMsg("CreateDirectory: %s", dir_name.c_str());
#endif

	string temp = dir_name;
	CreateDirectoryRecursively("", temp);
	return true;
}

void CreateDirectoryRecursively(string basePath, string path)
{
#ifdef _DEBUG
	//LogMsg("CreateDirectoryRecursively: %s, path is %s", basePath.c_str(), path.c_str());
#endif
	assert(0);
}


vector<string> GetDirectoriesAtPath(string path)
{
	vector<string> v;
	
#ifdef _DEBUG
	//LogMsg("GetDirectoriesAtPath: %s", path.c_str());
#endif

	dirent * buf, * ent;
	DIR *dp;

	dp = opendir(path.c_str());
	if (!dp)
	{
		LogError("GetDirectoriesAtPath: opendir failed");
		return v;
	}

	buf = (dirent*) malloc(sizeof(dirent)+512);
	while (readdir_r(dp, buf, &ent) == 0 && ent)
	{
		if (ent->d_name[0] == '.' && ent->d_name[1] == 0) continue;
		if (ent->d_name[0] == '.' && ent->d_name[1] == '.' && ent->d_name[2] == 0) continue;

		//LogMsg("Got %s. type %d", ent->d_name, int(ent->d_type));
		
		if (ent->d_type == DT_DIR)
		{
			v.push_back(ent->d_name);
		}
	}

	free (buf);
	closedir(dp);
	return v;
}

vector<string> GetFilesAtPath(string path)
{
#ifdef _DEBUG
	//LogMsg("GetFilesAtPath: %s", path.c_str());
#endif

	vector<string> v;
	dirent * buf, * ent;
	DIR *dp;

	dp = opendir(path.c_str());
	if (!dp)
	{
		LogError("GetDirectoriesAtPath: opendir failed");
		return v;
	}

	buf = (dirent*) malloc(sizeof(dirent)+512);
	while (readdir_r(dp, buf, &ent) == 0 && ent)
	{
		if (ent->d_name[0] == '.' && ent->d_name[1] == 0) continue;
		if (ent->d_name[0] == '.' && ent->d_name[1] == '.' && ent->d_name[2] == 0) continue;

		//LogMsg("Got %s. type %d", ent->d_name, int(ent->d_type));
		if (ent->d_type == DT_REG) //regular file
		{
			v.push_back(ent->d_name);
		}
	}

	free (buf);
	closedir(dp);
	return v;
}

bool RemoveDirectoryRecursively(string path)
{
	//LogMsg(" RemoveDirectoryRecursively: %s", path.c_str());
	
	dirent * buf, * ent;
	DIR *dp;

	dp = opendir(path.c_str());
	if (!dp)
	{
		LogError("RemoveDirectoryRecursively: opendir failed");
		return false;
	}

	buf = (dirent*) malloc(sizeof(dirent)+512);
	while (readdir_r(dp, buf, &ent) == 0 && ent)
	{
		
		if (ent->d_name[0] == '.' && ent->d_name[1] == 0) continue;
		if (ent->d_name[0] == '.' && ent->d_name[1] == '.' && ent->d_name[2] == 0) continue;

		//LogMsg("Got %s. type %d", ent->d_name, int(ent->d_type));
		if (ent->d_type == DT_REG) //regular file
		{
			string fName = path+string("/")+ent->d_name;
			//LogMsg("Deleting %s", fName.c_str());
			unlink( fName.c_str());
		}

		if (ent->d_type == DT_DIR) //regular file
		{
			string fName = path+string("/")+ent->d_name;
			//LogMsg("Entering DIR %s",fName.c_str());
			if (!RemoveDirectoryRecursively(fName.c_str()))
			{
				LogError("Error removing dir %s", fName.c_str());
				break;
			}
		}
	}

	free (buf);
	closedir(dp);

	//delete the final dir as well
	rmdir( path.c_str());
	return true; //success
}

bool CheckIfOtherAudioIsPlaying()
{
	return false;
}

void CreateAppCacheDirIfNeeded()
{
  //only applicable to iOS
}

void NotifyOSOfOrientationPreference(eOrientationMode orientation)
{
}

bool HasVibration()
{
	return true;
}
