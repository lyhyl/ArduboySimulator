/*
 *  App.cpp
 *  Created by Seth Robinson on 3/6/09.
 *  For license info, check the license.txt file that should have come with this.
 *
 */
#include "PlatformPrecomp.h"
#include "App.h"
#include "GUI/MainMenu.h"
#include "Renderer/LinearParticle.h"
#include "Entity/EntityUtils.h"//create the classes that our globally library expects to exist somewhere.
#include "Irrlicht/IrrlichtManager.h"
 
MessageManager g_messageManager;
MessageManager * GetMessageManager() {return &g_messageManager;}

FileManager g_fileManager;
FileManager * GetFileManager() {return &g_fileManager;}


#ifdef __APPLE__
#include "Audio/AudioManagerOS.h"
AudioManagerOS g_audioManager;
#else
#include "Audio/AudioManagerFMOD.h"
#include "Audio/AudioManagerSDL.h"
#include "Audio/AudioManagerAndroid.h"

#ifdef RT_WEBOS
AudioManagerSDL g_audioManager; //sound in windows and WebOS
//AudioManager g_audioManager; //to disable sound
#elif defined ANDROID_NDK
AudioManagerAndroid g_audioManager; //sound for android build

#else
AudioManagerFMOD g_audioManager; //sound in windows
//AudioManager g_audioManager; //to disable sound

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

	L_ParticleSystem::deinit();
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

	L_ParticleSystem::init(2000);

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

const char * GetAppName() {return "RT3DApp";};
//for palm webos and android.  Dunno why my palm os versions have .app in them, but it's already in the story so can't change now.
const char * GetBundlePrefix()
{

	char * bundlePrefix = "com.rtsoft.app.";
	return bundlePrefix;
}

//applicable to Palm WebOS builds only
const char * GetBundleName()
{
	char * bundleName = "RT3DApp";
	return bundleName;
}


