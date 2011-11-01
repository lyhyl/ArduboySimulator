#include "AndroidUtils.h"
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#include <jni.h>
#include <android/log.h>
#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include "PlatformSetup.h"
#include "BaseApp.h"

#include "FileSystem/FileSystemZip.h"
extern JavaVM* g_pJavaVM;

const char * GetAppName();
const char * GetBundlePrefix();
const char * GetBundleName();

bool g_preferSDCardForUserStorage = false;

uint32 g_callAppResumeASAPTimer = 0;
bool g_pauseASAP = false;
string g_musicToPlay;
int g_musicPos = 0;

//this delay fixes a problem with restoring surfaces before android 1.6 devices are ready for it resulting
//in white textures -  update: unneeded
#define C_DELAY_BEFORE_RESTORING_SURFACES_MS 1

OSMessage g_lastOSMessage; //used to be a temporary holding place so android can access it because I'm too lazy to figure
//out how to make/pass java structs

//android has some concurrency issues that cause problems unless we cache input events - we don't want it calling ConvertCoordinatesIfRequired
//willy nilly

class AndroidMessageCache
{
public:
	float x,y;
	eMessageType type;
	int finger;

};

list<AndroidMessageCache> g_messageCache;

JavaVM* g_pJavaVM = NULL;

bool g_landScapeNoNeckHurtMode = false;

int g_winVideoScreenX = 0;
int g_winVideoScreenY = 0;


void StringReplace(const std::string& what, const std::string& with, std::string& in);
vector<string> StringTokenize (const  string  & theString,  const  string  & theDelimiter );



extern "C" 
{

	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) 
	{
		//LogMsg("JNI_OnLoad(): you are here");

		JNIEnv* env;
		if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) 
		{
			LogMsg("JNI_OnLoad(): GetEnv failed");
			return -1;
		}
		g_pJavaVM = vm; //save to use this for the rest of the app.  Not neccesarily safe to use the Env passed in though.
		return JNI_VERSION_1_4;
	}

}

int GetPrimaryGLX() 
{
	if (g_landScapeNoNeckHurtMode)
	{
		return g_winVideoScreenY;
	}

	return g_winVideoScreenX;
}
int GetPrimaryGLY() 
{
	if (g_landScapeNoNeckHurtMode)
	{
		return g_winVideoScreenX;
	}

	return g_winVideoScreenY;
}	

void LogMsg ( const char* traceStr, ... )
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	memset ( (void*)buffer, 0, logSize );

	va_start ( argsVA, traceStr );
	vsnprintf( buffer, logSize, traceStr, argsVA );
	va_end( argsVA );
	
	
	__android_log_write(ANDROID_LOG_INFO,GetAppName(), buffer);

	if (g_isLoggerInitted)
	{
		GetBaseApp()->GetConsole()->AddLine(buffer);
	}
}


string GetBaseAppPath()
{
	return ""; //we mount the assets as zip, there really isn't a base path
}

JNIEnv * GetJavaEnv()
{
	assert(g_pJavaVM);

	JNIEnv *env = NULL;
	g_pJavaVM->GetEnv((void **)&env, JNI_VERSION_1_4);
	if (!env)
	{
		LogError("Env is null, something is terrible wrong");
		return NULL;
	}
	return env;
};

char * GetAndroidMainClassName()
{
	static char name[128]="";
	static bool bFirstTime = true;
	if (bFirstTime)
	{
		bFirstTime = false;
		string package = string(GetBundlePrefix())+string(GetBundleName())+"/Main";
		StringReplace(".", "/", package);
		sprintf(name, package.c_str());
	}

	return name;

};

//this isn't really defined anywhere yet, if you need it, make your own header. Better to just use GetAppCachePath()
//directly.
void SetPreferSDCardForStorage(bool bNew)
{
	g_preferSDCardForUserStorage = bNew;
}

string GetSavePath()
{

	if (g_preferSDCardForUserStorage)
	{
		string storageDir = GetAppCachePath();
		if (!storageDir.empty()) return storageDir;
	}

	JNIEnv *env = GetJavaEnv();
	if (!env) return "";

	jclass cls = env->FindClass(GetAndroidMainClassName());
	jmethodID mid = env->GetStaticMethodID(cls,
		"get_docdir",
		"()Ljava/lang/String;");
	jstring ret;
	ret = (jstring)env->CallStaticObjectMethod(cls, mid);
	static char r[512];
	const char * ss=env->GetStringUTFChars(ret,0);
	sprintf(r,"%s",ss);
	env->ReleaseStringUTFChars(ret, ss);
	string retString = string(r)+"/";
	//LogMsg("Normal dir is %s", string(retString).c_str());

	return retString;
}

string GetAPKFile()
{
	JNIEnv *env = GetJavaEnv();
	if (!env) return "";

	jclass cls = env->FindClass(GetAndroidMainClassName());
	jmethodID mid = env->GetStaticMethodID(cls,
		"get_apkFileName",
		"()Ljava/lang/String;");
	jstring ret;
	ret = (jstring)env->CallStaticObjectMethod(cls, mid);
	const char * ss=env->GetStringUTFChars(ret,0);
	string s = ss;
	env->ReleaseStringUTFChars(ret, ss);
	return string(s);
}

//returns the SD card user save path (will be deleted when app is uninstalled on 2.2+)
//returns "" if no SD card/external writable storage available

string GetAppCachePath()
{
	JNIEnv *env = GetJavaEnv();
	if (!env) return "";

		//first see if we can access an external storage method
		jclass cls = env->FindClass(GetAndroidMainClassName());
		jmethodID mid = env->GetStaticMethodID(cls,
			"get_externaldir",
			"()Ljava/lang/String;");
		jstring ret;
		ret = (jstring)env->CallStaticObjectMethod(cls, mid);
		static char r[512];
		const char * ss=env->GetStringUTFChars(ret,0);
		sprintf(r,"%s",ss);
		env->ReleaseStringUTFChars(ret, ss);

		string retString = string(r);

		if (!retString.empty())
		{
			retString += string("/Android/data/")+GetBundlePrefix()+GetBundleName()+"/files/";
			//LogMsg("External dir is %s", retString.c_str());

			//looks valid
			return retString;
		}
	
	return ""; //invalid
}

void LaunchEmail(string subject, string content)
{
}

void LaunchURL(string url)
{
	JNIEnv *env = GetJavaEnv();
	LogMsg("Launching %s", url.c_str());

	if (!env) return;
	jclass cls = env->FindClass(GetAndroidMainClassName());
	jmethodID mid = env->GetStaticMethodID(cls,
		"LaunchURL",
		"(Ljava/lang/String;)V");
	env->CallStaticVoidMethod(cls, mid, env->NewStringUTF(url.c_str()));
}

string GetClipboardText()
{
	JNIEnv *env = GetJavaEnv();
	if (!env) return "";

	jclass cls = env->FindClass(GetAndroidMainClassName());
	jmethodID mid = env->GetStaticMethodID(cls,
		"get_clipboard",
		"()Ljava/lang/String;");
	jstring ret;
	ret = (jstring)env->CallStaticObjectMethod(cls, mid);
	const char * ss=env->GetStringUTFChars(ret,0);
	string s = ss;
	env->ReleaseStringUTFChars(ret, ss);
	return string(s);
}

bool IsIPhone3GS()
{
	return false;
}

bool IsDesktop()
{
	if (GetEmulatedPlatformID() == PLATFORM_ID_ANDROID) return false;
	return true;
}

ePlatformID GetPlatformID()
{
	return PLATFORM_ID_ANDROID;
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

string GetDeviceID()
{
	JNIEnv *env = GetJavaEnv();
	if (!env) return "";

	jclass cls = env->FindClass(GetAndroidMainClassName());
	jmethodID mid = env->GetStaticMethodID(cls,	
		"get_deviceID",
		"()Ljava/lang/String;");
	jstring ret;
	ret = (jstring)env->CallStaticObjectMethod(cls, mid);
	const char * ss=env->GetStringUTFChars(ret,0);
	string s = ss;
	env->ReleaseStringUTFChars(ret, ss);
	return string(s);
}

bool IsIPAD()
{
	return false;
}

eDeviceMemoryClass GetDeviceMemoryClass()
{
	return C_DEVICE_MEMORY_CLASS_2;
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
	JNIEnv *env = GetJavaEnv();
	if (!env) return "";

	jclass cls = env->FindClass(GetAndroidMainClassName());
	jmethodID mid = env->GetStaticMethodID(cls,
		"get_region",
		"()Ljava/lang/String;");
	jstring ret;
	ret = (jstring)env->CallStaticObjectMethod(cls, mid);
	const char * ss=env->GetStringUTFChars(ret,0);
	string s = ss;
	env->ReleaseStringUTFChars(ret, ss);
	return string(s);
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
	
	JNIEnv *env = GetJavaEnv();
	if (!env) return;

	jclass cls = env->FindClass(GetAndroidMainClassName());
	jmethodID mid = env->GetStaticMethodID(cls,
		"create_dir_recursively",
		"(Ljava/lang/String;Ljava/lang/String;)V");
	jstring ret;
	env->CallStaticVoidMethod(cls, mid, env->NewStringUTF(basePath.c_str()), env->NewStringUTF(path.c_str()));
	return;
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

void AppResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
	g_winVideoScreenX = w;
	g_winVideoScreenY = h;
#ifdef _DEBUG
	LogMsg("Resizing screen to %d %d", w, h);
#endif
	
	if (!GetBaseApp()->IsInitted())
	{
		SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
		LogMsg("Initializing BaseApp...");
		srand( (unsigned)time(NULL) );
		FileSystemZip *pFileSystem = new FileSystemZip();
		if (!pFileSystem->Init(GetAPKFile()))
		{
			LogMsg("Error finding APK file to load resources (%s", GetAPKFile().c_str());
		}

		/*
		vector<string> contents = pFileSystem->GetContents();
		for (int i=0; i < contents.size(); i++)
		{
		LogMsg("%s", contents[i].c_str());
		}
		*/

		pFileSystem->SetRootDirectory("assets");

		GetFileManager()->MountFileSystem(pFileSystem);
		LogMsg("Assets mounted");

		if (!GetBaseApp()->Init())
		{
			LogMsg("Unable to initalize BaseApp");
		}


		//let's also create our save directory on the sd card if needed, so we don't get errors when just assuming we can save
		//settings later in the app.

		CreateDirectoryRecursively("", GetAppCachePath());


	}

	GetBaseApp()->OnScreenSizeChange();
};

void AppRender(JNIEnv*  env)
{
	if (GetBaseApp()->IsInBackground() || g_pauseASAP) return;

	glViewport(0, 0, GetPrimaryGLX(), GetPrimaryGLY());
	GetBaseApp()->Draw();
	//appRender(0, sWindowWidth, sWindowHeight);
}

void AppUpdate(JNIEnv*  env)
{
	
	if (g_pauseASAP)
	{
		g_pauseASAP = false;	

		LogMsg("Pause");

		GetBaseApp()->m_sig_unloadSurfaces();
		GetBaseApp()->OnEnterBackground();

		GetAudioManager()->Kill();
		
		return;
	}

	if (g_callAppResumeASAPTimer != 0 && g_callAppResumeASAPTimer < GetSystemTimeTick())
	{
#ifdef _DEBUG
		LogMsg("Resuming at %u (timer was %u)", GetSystemTimeTick(), g_callAppResumeASAPTimer);
#endif
		g_callAppResumeASAPTimer = 0;
		GetBaseApp()->OnEnterForeground();
		GetAudioManager()->Init();

		if (!g_musicToPlay.empty())
		{
			GetAudioManager()->Play(g_musicToPlay, GetAudioManager()->GetLastMusicLooping(), true, false, true);
			GetAudioManager()->SetPos(GetAudioManager()->GetLastMusicID(), g_musicPos);
		}

	}

	if (GetBaseApp()->IsInBackground()) return;
	GetBaseApp()->Update();
}

void AppDone(JNIEnv*  env)
{
	LogMsg("Killing base app.");
	if (IsBaseAppInitted())
		GetBaseApp()->Kill();
}

void AppPause(JNIEnv*  env)
{
	
	//instead of pausing right now and dealing with thread problems, schedule it to happen
	//in the main update.  We will kill the sound now though.

	//Note: Thread update isn't going to happen until they COME back, which means we aren't killing
	//all used textures.  But this fixes problems with the engine trying to reload textures as it
	//notices they are killed because the pause is run concurrently with the game update/render
	//thread going.

	//We will kill/restore lost surfaces when the app is restored so we are still ok with v1.6

	//This also means we might not be saving the game until we return which is sort of weird, but
	//I don't think Android users really expect that to happen anyway

	if (g_pauseASAP)
	{
		LogMsg("Got android AppPause, ignoring as we've already triggered it");
		return;
	}
#ifdef _DEBUG
	LogMsg("Got android AppPause");
#endif

	g_pauseASAP = true;

	if (GetAudioManager()->IsPlaying(GetAudioManager()->GetLastMusicID()))
	{
		g_musicToPlay = GetAudioManager()->GetLastMusicFileName();
		g_musicPos = GetAudioManager()->GetPos(GetAudioManager()->GetLastMusicID());
	} else
	{
		g_musicToPlay.clear();
		g_musicPos = 0;

	}
	
	GetBaseApp()->m_sig_pre_enterbackground(NULL); //I needed this to kill audio in a custom way, but we still
	//shouldn't do anything else or we risk android conflicts

	GetAudioManager()->Kill();

}
void AppResume(JNIEnv*  env)
{

	g_callAppResumeASAPTimer = GetSystemTimeTick() + C_DELAY_BEFORE_RESTORING_SURFACES_MS;
#ifdef _DEBUG
	LogMsg("Queuing resume: %u (now is %u)", g_callAppResumeASAPTimer, GetTick(TIMER_SYSTEM));
#endif
}

void AppInit(JNIEnv*  env)
{
	//happens after the gl surface is initialized
	LogMsg("Initialized GL surfaces for game");
	GetBaseApp()->InitializeGLDefaults();
	LogMsg("gl defaults set");
	GetBaseApp()->OnScreenSizeChange();
	LogMsg("OnScreensizechange done");
	GetBaseApp()->m_sig_loadSurfaces(); 
	LogMsg("Surfaces loaded");

}



enum eAndroidActions
{
	ACTION_DOWN,
	ACTION_UP,
	ACTION_MOVE,
	ACTION_CANCEL,
	ACTION_OUTSIDE,
};

void AppOnTouch( JNIEnv*  env, jobject jobj,jint action, jfloat x, jfloat y, jint finger)
{
	//LogMsg("Got action %d, %.2f %.2f", action, x, y);

	
	eMessageType messageType = MESSAGE_TYPE_UNKNOWN;

	switch (action)
	{
	case ACTION_DOWN:
		messageType = MESSAGE_TYPE_GUI_CLICK_START;
		break;

	case ACTION_UP:
		messageType = MESSAGE_TYPE_GUI_CLICK_END;
		break;

	case ACTION_MOVE:
		messageType = MESSAGE_TYPE_GUI_CLICK_MOVE;
		break;

	default:
#ifndef NDEBUG
		LogMsg("Unhandled input message %d at %.2f:%.2f", action, x, y);
#endif
		break;

	}

	static AndroidMessageCache m;
	
	m.x = x;
	m.y = y;
	m.finger = finger;
	m.type = messageType;

	g_messageCache.push_back(m);
	//GetMessageManager()->SendGUI(messageType, x, y);
}


void AppOnSendGUIEx(JNIEnv*  env, jobject thiz,jint messageType, jint parm1, jint parm2, jint finger )
{
	GetMessageManager()->SendGUIEx((eMessageType)messageType, (float)parm1, (float)parm2, finger);  

}

void AppOnSendGUIStringEx(JNIEnv*  env, jobject thiz,jint messageType, jint parm1, jint parm2, jint finger, jstring s )
{
	const char * ss=env->GetStringUTFChars(s,0);
	string str = ss;
	env->ReleaseStringUTFChars(s, ss);

	GetMessageManager()->SendGUIStringEx((eMessageType)messageType, (float)parm1, (float)parm2, finger, str);  

}

void AppOnKey( JNIEnv*  env, jobject jobj, jint type, jint keycode, jint c)
{
	
#ifdef _DEBUG
	LogMsg("Native Got type %d, keycode %d, key %d (%c)", type, keycode, c, (char(c)));
#endif

	switch (keycode)
	{
		
		//case 4: e.v = KEY_BACK; break; // KEYCODE_BACK
		//case 82: e.v = KEY_MENU; break; // KEYCODE_MENU
		//case 84: e.v = KEY_SEARCH; break; // KEYCODE_SEARCH
	case 66: //enter
		c = 13;
		break;
	case 67: //back space
		c = 8;
		break;
	}
	
	if (keycode >= VIRTUAL_KEY_BACK)
	{
		
		if (GetIsUsingNativeUI())
		{
			//hitting back with the keyboard open?  Just pretend they closed the keyboard.
			SetIsUsingNativeUI(false);
			return;
		} else
		{
			c = keycode;
		}
		

		c = keycode;
	}

	switch (type)
	{
	case 1: //keydown
		GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR, (float)c, (float)1);  
		
		if (c < 128) c = toupper(c);
		GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR_RAW, (float)c, (float)1);  
		break;

	case 0: //key up
		if (c < 128) c = toupper(c);
		GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR_RAW, (float)c, 0.0f);  
		break;
	}
}


int AppOSMessageGet(JNIEnv* env)
{

	while (!g_messageCache.empty())
	{
		AndroidMessageCache *pM = &g_messageCache.front();
		
		ConvertCoordinatesIfRequired(pM->x, pM->y);

		GetMessageManager()->SendGUIEx(pM->type, pM->x, pM->y, pM->finger);
		g_messageCache.pop_front();
	}

	while (!GetBaseApp()->GetOSMessages()->empty())
	{
		//check for special messages that we don't want to pass on
		g_lastOSMessage = GetBaseApp()->GetOSMessages()->front();
		if (g_lastOSMessage.m_type == OSMessage::MESSAGE_CHECK_CONNECTION)
		{
				//pretend we did it
				GetMessageManager()->SendGUI(MESSAGE_TYPE_OS_CONNECTION_CHECKED, (float)RT_kCFStreamEventOpenCompleted, 0.0f);	
				GetBaseApp()->GetOSMessages()->pop_front();
				continue;
		}
		break;
	}

	if (!GetBaseApp()->GetOSMessages()->empty())
	{
		g_lastOSMessage = GetBaseApp()->GetOSMessages()->front();
		GetBaseApp()->GetOSMessages()->pop_front();
		//LogMsg("Preparing OS message %d, %s", g_lastOSMessage.m_type, g_lastOSMessage.m_string.c_str());
		return g_lastOSMessage.m_type;
	}

	return OSMessage::MESSAGE_NONE;
}

jstring AppGetLastOSMessageString(JNIEnv* env)
{
	 return(env->NewStringUTF(g_lastOSMessage.m_string.c_str()));
}


float AppGetLastOSMessageX(JNIEnv* env)
{
	return g_lastOSMessage.m_x;
}



void AppOnTrackball(JNIEnv* env, jobject jobj, jfloat x, jfloat y)
{
	//LogMsg("Got %.2f, %.2f", x, y);
	GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_TRACKBALL,  Variant(x, y, 0.0f));
}

void AppOnAccelerometerUpdate(JNIEnv* env, jobject jobj, jfloat x, jfloat y, jfloat z)
{
	//convert to about the same format as iPhone and webOS.  I used the nexus one to calibrate this.. are other phones different?

	x *= -(1.0f/8.3f);
	y *= -(1.0f/8.3f);
	z *= -(1.0f/8.3f);

	GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_ACCELEROMETER,  Variant(x, y, z));
}