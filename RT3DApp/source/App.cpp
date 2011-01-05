/*
 *  App.cpp
 *  Created by Seth Robinson on 3/6/09.
 *  For license info, check the license.txt file that should have come with this.
 *
 */
#include "PlatformPrecomp.h"
#include "App.h"
#include "GUI/MainMenu.h"
#include "Entity/EntityUtils.h"
#include "Irrlicht/IrrlichtManager.h"
 
//create the classes that our globally library expects to exist somewhere.

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

#ifdef RT_WEBOS
AudioManagerSDL g_audioManager; //sound in windows and WebOS
//AudioManager g_audioManager; //to disable sound
#elif defined ANDROID_NDK
AudioManagerAndroid g_audioManager; //sound for android
#else
//in windows
//AudioManager g_audioManager; //to disable sound

#include "Audio/AudioManagerAudiere.h"
AudioManagerAudiere g_audioManager;  //Use Audiere for audio

//#include "Audio/AudioManagerFMOD.h"
//AudioManagerFMOD g_audioManager; //if we wanted FMOD sound in windows

#endif
#endif

AudioManager * GetAudioManager(){return &g_audioManager;}

App *g_pApp = NULL;
BaseApp * GetBaseApp() 
{
	if (!g_pApp)
	{
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

	//L_ParticleSystem::deinit(); //if we wanted to use the 2d particle system
}

bool App::Init()
{
	
	SetDefaultButtonStyle(Button2DComponent::BUTTON_STYLE_CLICK_ON_TOUCH_RELEASE);
	
	if (GetEmulatedPlatformID() != PLATFORM_ID_ANDROID)
	{
		SetLockedLandscape(true); //we don't allow portrait mode for this game.  Android doesn't count
		//because its landscape mode is addressed like portrait mode when set in the manifest.

	}
	
	//SetupFakePrimaryScreenSize(320,480); //game will think its this size, and will be scaled up

	//L_ParticleSystem::init(2000); //if we wanted to use the 2d particle system

	if (m_bInitted)	
	{
		return true;
	}
	
	if (!BaseApp::Init()) return false;

	SetManualRotationMode(true);

	LogMsg("Save path is %s", GetSavePath().c_str());

	if (!GetFont(FONT_SMALL)->Load("interface/font_trajan.rtfont")) return false;
	if (!GetFont(FONT_LARGE)->Load("interface/font_trajan_big.rtfont")) return false;
	//GetFont(FONT_SMALL)->SetSmoothing(false);

	GetBaseApp()->SetFPSVisible(true);
	
	bool bFileExisted;
	m_varDB.Load("save.dat", &bFileExisted);
 
	//preload audio
	GetAudioManager()->Preload("audio/click.wav");
	if (!GetIrrlichtManager()->Init(0)) return false;
	return true;
}

void App::Kill()
{
	GetIrrlichtManager()->Kill();
	
	BaseApp::Kill();
}

void App::Update()
{
	BaseApp::Update();

	if (!m_bDidPostInit)
	{
		m_bDidPostInit = true;
		m_special = GetSystemData() != C_PIRATED_NO;
		
		//build a GUI node
		Entity *pGUIEnt = GetEntityRoot()->AddEntity(new Entity("GUI"));
		MainMenuCreate(pGUIEnt);
		PreloadKeyboard();
	}
}

void App::Draw()
{
	//turn normal GL back on
	//GetBaseApp()->OnScreenSizeChange();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw our game stuff
	//DrawFilledRect(0,80,GetScreenSizeX(),GetScreenSizeY()/2, MAKE_RGBA(200,0,0,100));
	//RenderSpinningTriangle();
	
	GetIrrlichtManager()->IsRunning(); //let it do its own update tick
	GetIrrlichtManager()->BeginScene(); //turn on irrlicht's 3d mode renderstates
	GetIrrlichtManager()->Render(); //render its scenegraph
	GetIrrlichtManager()->EndScene(); //tell irrlicht to go into 2d mode renderstates
	
	g_lastBound = 0;
	SetupOrtho();
	//GetFont(FONT_SMALL)->Draw(0,50, " white `2Green `3Cyan `4Red `5Purp ");

   //GetFont(FONT_SMALL)->DrawScaled(0,200, "Default - `2Green`` - default",1+SinPulseByMS(3000), MAKE_RGBA(255,255,255,50));
	//the base handles actually drawing the GUI stuff over everything else
	BaseApp::Draw();
	PrepareForGL();
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

void App::SaveStuff()
{
	m_varDB.Save("save.dat");
}
void App::OnEnterBackground()
{
	SaveStuff();
	BaseApp::OnEnterBackground();
}

void App::OnEnterForeground()
{
	BaseApp::OnEnterForeground();
}


void App::OnExitApp(VariantList *pVarList)
{
	LogMsg("Exiting the app");

	OSMessage o;
	o.m_type = OSMessage::MESSAGE_FINISH_APP;
	GetBaseApp()->AddOSMessage(o);
}

const char * GetAppName() {return "RT3DApp";};
//for palm webos, android, App Store
const char * GetBundlePrefix()
{
	char * bundlePrefix = "com.rtsoft.";
	return bundlePrefix;
}

//applicable to Palm WebOS builds only
const char * GetBundleName()
{
	char * bundleName = "3dapp";
	return bundleName;
}


