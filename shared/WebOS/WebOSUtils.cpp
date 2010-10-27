#ifdef RT_WEBOS
#include "SDL.h"
#include "WebOSUtils.h"
#include "PDL.h"
#include <errno.h>

#ifdef RT_WEBOS_ARM
#include "PlatformSetupWebOS.h"

#else
#include "PlatformSetupWin.h"
#endif

#include <cstdlib>
#include <cstdarg>
#include <cassert>

void StringReplace(const std::string& what, const std::string& with, std::string& in);
vector<string> StringTokenize (const  string  & theString,  const  string  & theDelimiter );

string GetBaseAppPath()
{
	return ""; //well, according to the docs PDL_GetCallingPath is deprecated
}



string GetSavePath()
{
#ifdef RT_WEBOS_ARM
	return "";  //well, according to the docs, PDL_GetDataFilePath is buggy and returns a dir that can't be written to and "" is better to use.

#else
	return "";

#endif
}

string GetAppCachePath()
{
	return "";
}

void LaunchEmail(string subject, string content)
{
	PDL_LaunchEmail(subject.c_str(), "");
}

void LaunchURL(string url)
{
	LogMsg("Launching %s", url.c_str());
	
	PDL_LaunchBrowser(url.c_str());

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
	if (GetEmulatedPlatformID() == PLATFORM_ID_WEBOS) return false;
	return true;
}

ePlatformID GetPlatformID()
{
	return PLATFORM_ID_WEBOS;
}


bool IsIphoneOriPad()
{
	return false;
}

bool IsIphone()
{
	//return false; //act like an iTouch, useful for knowing when not to show stuff about vibration and camera
	if (IsIPAD() || IsIphone4()) return false;

	return true;  //act like an iPhone
}

bool IsIphone4()
{
	//return false; //act like an iTouch, useful for knowing when not to show stuff about vibration and camera
	if (GetPrimaryGLX() == 960 || GetPrimaryGLY() == 960) return true;
	return false; 
}

bool IsIPAD()
{
	if (GetPrimaryGLX() == 1024 || GetPrimaryGLY() == 1024) return true;
	return false;
}

eDeviceMemoryClass GetDeviceMemoryClass()
{
#ifdef WIN32
	return C_DEVICE_MEMORY_CLASS_4;

#else
	return C_DEVICE_MEMORY_CLASS_2;
#endif
}


int GetYOffset()
{
	return 0;
}

unsigned int GetSystemTimeTick()
{
	return SDL_GetTicks();
}
double GetSystemTimeAccurate()
{
	//return double(GetTickCount());

	return SDL_GetTicks();
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

const char * GetBundleName();
const char * GetBundlePrefix();

int GetSystemData()
{
	/*
	//don't know if it's safe to use this, I read on the forum it can lockup devices who are on wifi only?
	int ret = PDL_isAppLicensedForDevice(  (  string(GetBundlePrefix())+string(GetBundleName()) ).c_str());
	if (ret == 0) return C_PIRATED_YES;
	
	*/
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
			LogMsg("Warning: Unable to delete file %s, file is being used", fileName.c_str());
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
	const int buffLen= 64;
	char buff[buffLen];
	PDL_GetLanguage(buff, buffLen);

	for (int i=0; buff[i]; i++)
	{
		buff[i] = tolower(buff[i]);
	}
	return buff;
	
	//return "en_jp";
}


//month is 1-12 btw
int GetDaysSinceDate(int month,int day, int year)
{
	
	LogMsg("GetDaysSinceDate url not done");
	assert(!"no!");
	return 0;
/*
	time_t ltime;
	time( &ltime );

	tm expire = { 0, 0, 0, day, month-1, year-1900, 0 };	 //Month is 0-11 btw
	tm today = *localtime( &ltime );

	long time_now = (long)today.tm_mday + (long)today.tm_mon * 30 + today.tm_year*365;
	long time_exp = (long)expire.tm_mday +(long)expire.tm_mon * 30 + expire.tm_year * 365;
	long time_passed = time_now - time_exp;

	//now let's convert it back to days
	if (time_passed == 0) return 0; //avoid devide by 0
	return time_passed;
	*/
}


bool RTCreateDirectory(const std::string &dir_name)
{

	LogMsg("RTCreateDirectory url not done");
	assert(!"no!");
	return 0;
/*
	if (dir_name.empty())
		return false;

	// this will be a full path
	std::string full_path; 	// calculate the full path

#ifdef WIN32
	DWORD buff_len = ::GetFullPathName(dir_name.c_str(), 0, 0, 0);

	if (buff_len == 0)
		// can't calculate, return bad status
		return false;
	else
	{
		char * buffer = new char[buff_len + 1];
		char * buffer_ptr_to_filename = 0;
		// Obtaining full path
		buff_len = ::GetFullPathName(dir_name.c_str(), buff_len, buffer, &buffer_ptr_to_filename);
		if (buff_len == 0)
		{
			delete[] buffer;			
			// can't obtain full path, return bad status
			return false;
		}

		// ok, save it
		full_path = buffer;
		delete[] buffer;			
	}
#else
	// TODO: add here Linux version of GetFullPathName
	full_path = dir_name;
#endif

#ifdef WIN32
	return ::CreateDirectory(full_path.c_str(), NULL) != 0;
#else
	return ::mkdir(full_path.c_str(), 0755) == 0;
#endif
	*/
}


void CreateDirectoryRecursively(string basePath, string path)
{
	LogMsg("CreateDirectoryRecursively url not done");
	assert(!"no!");
	return;
	
	/*StringReplace("\\", "/", path);
	StringReplace("\\", "/", basePath);

	vector<string> tok = StringTokenize(path, "/");

	if (!basePath.empty())
	{
		if (basePath[basePath.size()-1] != '/') basePath += "/";
	}
	path = "";
	for (unsigned int i=0; i < tok.size(); i++)
	{
		path += tok[i].c_str();
		//LogMsg("Creating %s%s", basePath.c_str(), path.c_str());
		RTCreateDirectory(basePath+path);
		path += "/";
	}
	*/
}



vector<string> GetDirectoriesAtPath(string path)
{
	vector<string> v;


	
	LogMsg(" GetDirectoriesAtPathurl not done");
	assert(!"no!");
	return v;

/*
	if (path.empty()) path += "."; //needed for relative paths

	HANDLE hFind;    // file handle
	WIN32_FIND_DATA FindFileData;

	hFind = FindFirstFile((path+"\\*").c_str(),&FindFileData);
	while (FindNextFile(hFind,&FindFileData))
	{
		if(IsDots(FindFileData.cFileName)) continue;

		if((FindFileData.dwFileAttributes &	FILE_ATTRIBUTE_DIRECTORY))
		{
			v.push_back(string(FindFileData.cFileName));
		}
	}

	FindClose(hFind);
	return v;
	*/
}



vector<string> GetFilesAtPath(string path)
{
	vector<string> v;

	LogMsg(" GetFilesAtPath not done");
	assert(!"no!");
return v;

/*

	if (path.empty()) path += "."; //needed for relative paths

	HANDLE hFind;    // file handle
	WIN32_FIND_DATA FindFileData;

	hFind = FindFirstFile((path+"\\*").c_str(),&FindFileData);
	while (FindNextFile(hFind,&FindFileData))
	{
		if(IsDots(FindFileData.cFileName)) continue;

		if( !(FindFileData.dwFileAttributes &	FILE_ATTRIBUTE_DIRECTORY))
		{
			v.push_back(string(FindFileData.cFileName));
		}
	}

	FindClose(hFind);
	return v;
	*/
}

//based on a snippet fromFeroz Zahid (http://www.codeguru.com/cpp/w-p/files/folderdirectorymaintenance/article.php/c8999/
bool RemoveDirectoryRecursively(string path)
{
	LogMsg(" RemoveDirectoryRecursively not done");
	assert(!"no!");
	return false;

	/*
	const TCHAR* sPath = path.c_str();
	HANDLE hFind;    // file handle
	WIN32_FIND_DATA FindFileData;

	TCHAR DirPath[MAX_PATH];
	TCHAR FileName[MAX_PATH];

	_tcscpy(DirPath,sPath);
	_tcscat(DirPath,"\\*");    // searching all files
	_tcscpy(FileName,sPath);
	_tcscat(FileName,"\\");

	// find the first file

	//SETH: This is actually wrong as it's ignoring the first file.  But since it's always "." which means the current dir it doesn't
	//matter, right?

	hFind = FindFirstFile(DirPath,&FindFileData);
	if(hFind == INVALID_HANDLE_VALUE) return FALSE;
	_tcscpy(DirPath,FileName);

	bool bSearch = true;
	while(bSearch) {    // until we find an entry
		if(FindNextFile(hFind,&FindFileData)) {
			if(IsDots(FindFileData.cFileName)) continue;
			_tcscat(FileName,FindFileData.cFileName);
			if((FindFileData.dwFileAttributes &
				FILE_ATTRIBUTE_DIRECTORY)) {

					// we have found a directory, recurse
					if(!RemoveDirectoryRecursively(FileName)) {
						FindClose(hFind);
						return FALSE;    // directory couldn't be deleted
					}
					// remove the empty directory
					RemoveDirectory(FileName);
					_tcscpy(FileName,DirPath);
			}
			else {
				if(FindFileData.dwFileAttributes &
					FILE_ATTRIBUTE_READONLY)
					// change read-only file mode
					_chmod(FileName, _S_IWRITE);
				if(!DeleteFile(FileName)) {    // delete the file
					FindClose(hFind);
					return FALSE;
				}
				_tcscpy(FileName,DirPath);
			}
		}
		else {
			// no more files there
			if(GetLastError() == ERROR_NO_MORE_FILES)
				bSearch = false;
			else {
				// some error occurred; close the handle and return FALSE
				FindClose(hFind);
				return FALSE;
			}

		}

	}
	FindClose(hFind);                  // close the file handle

	return RemoveDirectory(sPath) != 0;     // remove the empty directory
*/
}


bool CheckIfOtherAudioIsPlaying()
{
	return false;
}

void NotifyOSOfOrientationPreference(eOrientationMode orientation)
{
	PDL_Orientation o = PDL_ORIENTATION_0;

	//map to WebOS specific versions from the iPhone versions
	switch(orientation)
	{
	case ORIENTATION_PORTRAIT:
		o = PDL_ORIENTATION_0;
		break;

	case ORIENTATION_PORTRAIT_UPSIDE_DOWN:
		o = PDL_ORIENTATION_180;
		break;


	case ORIENTATION_LANDSCAPE_LEFT:
		o = PDL_ORIENTATION_270;
		break;

	case ORIENTATION_LANDSCAPE_RIGHT:

		break;
	} 

	PDL_Err err = PDL_SetOrientation(o);
	if (err == PDL_INVALIDINPUT) LogMsg("Invalid orientation passed");
	if (err == PDL_ECONNECTION) LogMsg("Unable to communicate with app card");
}

bool HasVibration()
{
	return false;
}


#endif