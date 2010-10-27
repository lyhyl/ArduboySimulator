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
#include "Renderer/SoftSurface.h"

SurfaceAnim g_surf;
 
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
AudioManagerAndroid g_audioManager; //sound for android
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
		#ifndef NDEBUG
		LogMsg("Creating app object")	;
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


//First, our function we're going to call with a function object in a second..
void SaySomething(VariantList *pVlist)
{
	LogMsg( ("I will say:" + pVlist->Get(0).GetString()).c_str() );
}



bool App::Init()
{
	//SetDefaultAudioClickSound("audio/enter.wav");
	SetDefaultButtonStyle(Button2DComponent::BUTTON_STYLE_CLICK_ON_TOUCH_RELEASE);
	SetLockedLandscape(true); //we don't allow portrait mode for this game
	SetManualRotationMode(true);

	
	//if we do this, everything will be stretched/zoomed to fit the screen
	SetupFakePrimaryScreenSize(320,480); //game will think its this size, and will be scaled up
	
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
	//GetFont(FONT_SMALL)->SetSmoothing(false);

	GetBaseApp()->SetFPSVisible(true);
	
	bool bFileExisted;
	m_varDB.Load("save.dat", &bFileExisted);
 
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

		//build a GUI node

		Entity *pGUIEnt = GetEntityRoot()->AddEntity(new Entity("GUI"));
		MainMenuCreate(pGUIEnt);
	
	}
}

void App::Draw()
{
	//turn normal GL back on
	//GetBaseApp()->OnScreenSizeChange();
	PrepareForGL();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//draw our game stuff
	//DrawFilledRect(0,10,GetScreenSizeX(),GetScreenSizeY()/2, MAKE_RGBA(255,255,255,255));
	PrepareForGL();

	//RenderSpinningTriangle();
	g_lastBound = 0;
	//SetupOrtho();
	
	/*
	if (!g_surf.IsLoaded())
	{
		SoftSurface surf;
		surf.LoadFile("game/test.bmp", SoftSurface::COLOR_KEY_WHITE);
		
		g_surf.InitBlankSurface(surf.GetWidth(),surf.GetHeight());

		SoftSurface s;
		s.Init(surf.GetWidth(),surf.GetHeight(), SoftSurface::SURFACE_RGBA);
		s.FillColor(glColorBytes(0,0,0,0));
		s.Blit(0,0, &surf);
		s.FlipY();

		//put it on the GL surface
		g_surf.UpdateSurfaceRect(rtRect(0,0, s.GetWidth(), s.GetHeight()), s.GetPixelData());
		g_surf.SetUsesAlpha(surf.GetUsesAlpha());
		if (s.GetHasPremultipliedAlpha()) 
		{
			g_surf.SetBlendingMode(Surface::BLENDING_PREMULTIPLIED_ALPHA);
		}
	}
		 
	g_surf.BlitScaled(0,0, CL_Vec2f(3,3), ALIGNMENT_UPPER_LEFT);
	*/

	//DrawFilledRect(0,0,512,512, MAKE_RGBA(0,255,0,255));
	//GetFont(FONT_SMALL)->Draw(0,0, "`4test");
	//g_surf.CopyFromScreen();
	
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//g_surf.Blit(0,0, MAKE_RGBA(255,255,255,255));
	SetupOrtho();
	//GetFont(FONT_SMALL)->Draw(0,50, " white `2Green `3Cyan `4Red `5Purp ");
	//GetFont(FONT_SMALL)->DrawScaled(0,50, "Default - `2Green`` - default",1+SinGamePulseByMS(3000));
	//the base handles actually drawing the GUI stuff over everything else
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

const char * GetAppName() {return "SimpleApp";}

//for palm webos and android.  Dunno why my palm os versions have .app in them, but it's already in the story so can't change now.
const char * GetBundlePrefix()
{

	char * bundlePrefix = "com.rtsoft.";
	return bundlePrefix;
}

const char * GetBundleName()
{
	char * bundleName = "rtsimpleapp";
	return bundleName;
}
