
#include "PlatformPrecomp.h"
#include "App.h"
#include "GUI/MainMenu.h"
#include "Renderer/LinearParticle.h"
#include "Entity/EntityUtils.h"//create the classes that our globally library expects to exist somewhere.
#include "Renderer/SoftSurface.h"
#include "Entity/ArcadeInputComponent.h" 
#include "GUI/GameMenu.h"

SurfaceAnim g_surf;
 
MessageManager g_messageManager;
MessageManager * GetMessageManager() {return &g_messageManager;}

FileManager g_fileManager;
FileManager * GetFileManager() {return &g_fileManager;}

#ifdef __APPLE__

#if TARGET_OS_IPHONE == 1
	//it's an iPhone or iPad
	#include "Audio/AudioManagerOS.h"
	AudioManagerOS g_audioManager;
#else
	//it's being compiled as a native OSX app
   #include "Audio/AudioManagerFMOD.h"

   AudioManagerFMOD g_audioManager; //dummy with no sound
#endif
	
#else
#include "Audio/AudioManagerSDL.h"
#include "Audio/AudioManagerAndroid.h"

#if defined RT_WEBOS || defined RT_USE_SDL_AUDIO
AudioManagerSDL g_audioManager; //sound in windows and WebOS
//AudioManager g_audioManager; //to disable sound
#elif defined ANDROID_NDK
AudioManagerAndroid g_audioManager; //sound for android
#else
//in windows

#include "Audio/AudioManagerAudiere.h"
//#include "Audio/AudioManagerFMOD.h"

AudioManagerAudiere g_audioManager;  //Use Audiere for audio
//AudioManagerFMOD g_audioManager; //if we wanted FMOD sound in windows
//AudioManager g_audioManager; //to disable sound

#endif
#endif

AudioManager * GetAudioManager(){return &g_audioManager;}

App *g_pApp = NULL;
BaseApp * GetBaseApp() 
{
	if (!g_pApp)
	{
		#ifndef NDEBUG
		LogMsg("Creating app object");
		#endif
		g_pApp = new App;
	}

	return g_pApp;
}

App * GetApp() 
{
	return g_pApp;
}

App::App()
{
	m_bDidPostInit = false;
}

App::~App()
{
	L_ParticleSystem::deinit();
}

void App::OnExitApp(VariantList *pVarList)
{
	LogMsg("Exiting the app");

	OSMessage o;
	o.m_type = OSMessage::MESSAGE_FINISH_APP;
	GetBaseApp()->AddOSMessage(o);
}

bool App::Init()
{
	//SetDefaultAudioClickSound("audio/enter.wav");
	SetDefaultButtonStyle(Button2DComponent::BUTTON_STYLE_CLICK_ON_TOUCH_RELEASE);
	SetManualRotationMode(true);
	//SetFPSLimit(30);

	//we'll use a virtual screen size of this, and it will be scaled to any device
	int scaleToX = 1024;
	int scaleToY = 768;

	switch (GetEmulatedPlatformID())
	{
	case PLATFORM_ID_ANDROID:
	case PLATFORM_ID_OSX:
		//if we do this, everything will be stretched/zoomed to fit the screen
		SetLockedLandscape(false);  //because it's set in the app manifest, we don't have to rotate ourselves
		SetupFakePrimaryScreenSize(scaleToX,scaleToY); //game will think it's this size, and will be scaled up
		break;

	case PLATFORM_ID_BBX:
		//if we do this, everything will be stretched/zoomed to fit the screen
		SetLockedLandscape(false);  //because it's set in the app manifest, we don't have to rotate ourselves
		SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
		SetupFakePrimaryScreenSize(scaleToX,scaleToY); //game will think it's this size, and will be scaled up

		break;

	case PLATFORM_ID_WEBOS:
		//if we do this, everything will be stretched/zoomed to fit the screen
		if (IsIPADSize)
		{
			//doesn't need rotation
			SetLockedLandscape(false);  //because it's set in the app manifest, we don't have to rotate ourselves
			SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
			SetupFakePrimaryScreenSize(scaleToX,scaleToY); //game will think it's this size, and will be scaled up
		} else
		{
			//but the phones do
			SetLockedLandscape(true); //we don't allow portrait mode for this game
			SetupFakePrimaryScreenSize(scaleToY,scaleToX); //game will think it's this size, and will be scaled up
		}
		break;


	default:
		SetLockedLandscape(false); //we don't allow portrait mode for this game
		SetupFakePrimaryScreenSize(scaleToX,scaleToY); //game will think it's this size, and will be scaled up
	}


	if (GetPlatformID() == PLATFORM_ID_WEBOS && !IsIPADSize)
	{
		//non Touchpad webos devices are taller than they are higher.. fix that by rotating it
		LogMsg("Special handling for webos devices to switch to landscape mode..");
		//	SetLockedLandscape(true);
		//	SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_LANDSCAPE_LEFT);
	}

	L_ParticleSystem::init(2000);

	if (m_bInitted)	
	{
		return true;
	}
	
	if (!BaseApp::Init()) return false;

	LogMsg("Save path is %s", GetSavePath().c_str());

	if (!GetFont(FONT_SMALL)->Load("interface/font_trajan.rtfont")) 
	{
		LogMsg("Can't load font 1");
		return false;
	}
	if (!GetFont(FONT_LARGE)->Load("interface/font_trajan_big.rtfont"))
	{
		LogMsg("Can't load font 2");
		return false;
	}
	//GetFont(FONT_SMALL)->SetSmoothing(false); //if we wanted to disable bilinear filtering on the font

#ifdef _DEBUG
	GetBaseApp()->SetFPSVisible(true);
#endif
	
	bool bFileExisted;
	m_varDB.Load("save.dat", &bFileExisted);

	m_varDB.GetVarWithDefault("level", Variant(uint32(1)));
	//preload audio
	GetAudioManager()->Preload("audio/click.wav");
	return true;
}

void App::Kill()
{
	m_varDB.Save("save.dat");
	BaseApp::Kill();
	g_pApp = NULL;
}

void App::Update()
{
	BaseApp::Update();

	if (!m_bDidPostInit)
	{
		m_bDidPostInit = true;
		m_special = GetSystemData() != C_PIRATED_NO;

		//build a dummy entity called "GUI" to put our GUI menu entities under
		Entity *pGUIEnt = GetEntityRoot()->AddEntity(new Entity("GUI"));
	
#ifdef _DEBUG
		MainMenuCreate(pGUIEnt);
//		GameMenuCreate(pGUIEnt);
#else
		MainMenuCreate(pGUIEnt);
#endif
	}
}

void App::Draw()
{
	PrepareForGL();
//	glClearColor(0.6,0.6,0.6,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	BaseApp::Draw();
}

void App::OnScreenSizeChange()
{
	BaseApp::OnScreenSizeChange();
}

void App::GetServerInfo( string &server, uint32 &port )
{
#if defined (_DEBUG) && defined(WIN32)
	server = "localhost";
	port = 8080;

	//server = "www.rtsoft.com";
	//port = 80;
#else

	server = "rtsoft.com";
	port = 80;
#endif
}

int App::GetSpecial()
{
	return m_special; //1 means pirated copy
}

Variant * App::GetVar( const string &keyName )
{
	return GetShared()->GetVar(keyName);
}

std::string App::GetVersionString()
{
	return "V0.7";
}

float App::GetVersion()
{
	return 0.7f;
}

int App::GetBuild()
{
	return 1;
}

const char * GetAppName() {return "Looney Ladders by Seth A. Robinson (rtsoft.com)";}

//for palm webos and android
const char * GetBundlePrefix()
{
	const char * bundlePrefix = "com.rtsoft.";
	return bundlePrefix;
}

const char * GetBundleName()
{
	const char * bundleName = "RTLooneyLadders";
	return bundleName;
}


//below is a sort of hack that allows "release" builds on windows to override the settings of whatever the shared main.cpp is telling
//us for window sizes
#ifdef _WINDOWS_
#include "win/app/main.h"
#endif

bool App::OnPreInitVideo()
{
	if (!BaseApp::OnPreInitVideo()) return false;

#if !defined(_DEBUG) && defined(_WINDOWS_)

	SetEmulatedPlatformID(PLATFORM_ID_WINDOWS);
	g_winVideoScreenX = 768;
	g_winVideoScreenY = 1024;
	g_landScapeNoNeckHurtMode = true;
#endif
	return true;
}
