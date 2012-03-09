/*
 *  App.cpp
 *  Created by Seth Robinson on 3/6/09.
 *  For license info, check the license.txt file that should have come with this.
 *
 */
#include "PlatformPrecomp.h"
#include "App.h"

#include "Entity/CustomInputComponent.h" //used for the back button (android)
#include "Entity/FocusInputComponent.h" //needed to let the input component see input messages
#include "Entity/LogDisplayComponent.h"
#include "MenuStore.h"
#include "MenuMain.h"

MessageManager g_messageManager;
MessageManager * GetMessageManager() {return &g_messageManager;}

FileManager g_fileManager;
FileManager * GetFileManager() {return &g_fileManager;}

#include "Audio/AudioManager.h"
AudioManager g_audioManager; //to disable sound, this is a dummy

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
	assert(g_pApp && "GetBaseApp must be called used first");
	return g_pApp;
}

App::App()
{
	m_bDidPostInit = false;
}

App::~App()
{
}

bool App::Init()
{
	
	if (m_bInitted)	
	{
		return true;
	}

	SetManualRotationMode(true);
	int scaleToX = 1024;
	int scaleToY = 768;

	switch (GetEmulatedPlatformID())
	{
		//special handling for certain platforms to tweak the video settings

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

	case PLATFORM_ID_IOS:
		SetLockedLandscape(true); //we stay in portrait but manually rotate, gives better fps on older devices
		SetupFakePrimaryScreenSize(scaleToX,scaleToY); //game will think it's this size, and will be scaled up
		break;

	default:

		//Default settings for other platforms

		SetLockedLandscape(false); //we don't allow portrait mode for this game
		SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
		SetupFakePrimaryScreenSize(scaleToX,scaleToY); //game will think it's this size, and will be scaled up
	}


	if (!BaseApp::Init()) return false;
	
	LogMsg("Save path is %s", GetSavePath().c_str());
	LogMsg("Region string is %s", GetRegionString().c_str());

#ifndef C_NO_ZLIB
	//fonts need zlib to decompress.  When porting a new platform I define C_NO_ZLIB and add zlib support later sometimes
	if (!GetFont(FONT_SMALL)->Load("interface/font_trajan.rtfont")) return false;
#endif
	
	m_IAPManager.Init();
	
	m_adManager.Init();
	GetBaseApp()->SetFPSVisible(true);
	return true;
}

void App::Kill()
{
	BaseApp::Kill();
}

void App::OnExitApp(VariantList *pVarList)
{
	LogMsg("Exiting the app");
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_FINISH_APP;
	GetBaseApp()->AddOSMessage(o);
}

void OnGotTapPoints(VariantList *pVList)
{
	LogMsg("Our OnGotTapPoints callback sees %d points were awarded.", pVList->Get(0).GetINT32());

}

void App::Update()
{
	
	//game can think here.  The baseApp::Update() will run Update() on all entities, if any are added.  The only one
	//we use in this example is one that is watching for the Back (android) or Escape key to quit that we setup earlier.
	
	BaseApp::Update();

	m_IAPManager.Update();
	m_adManager.Update();
	
	if (!m_bDidPostInit)
	{
		//stuff I want loaded during the first "Update"
		m_bDidPostInit = true;
		
		//for android, so the back key (or escape on windows) will quit out of the game
		Entity *pEnt = GetEntityRoot()->AddEntity(new Entity);
		EntityComponent *pComp = pEnt->AddComponent(new CustomInputComponent);
		//tell the component which key has to be hit for it to be activated
		pComp->GetVar("keycode")->Set(uint32(VIRTUAL_KEY_BACK));
		//attach our function so it is called when the back key is hit
		pComp->GetFunction("OnActivated")->sig_function.connect(1, boost::bind(&App::OnExitApp, this, _1));
		//nothing will happen unless we give it input focus
		pEnt->AddComponent(new FocusInputComponent);

		SetConsole(true, true);
		//adjust the console so it's full screen
		Entity *pConsole = GetEntityRoot()->GetEntityByName("ConsoleEnt");
		
		pConsole->GetVar("pos2d")->Set(CL_Vec2f(0,GetScreenSizeYf()*.7f));
		pConsole->GetVar("size2d")->Set(CL_Vec2f(GetScreenSizeXf(), GetScreenSizeYf()- (GetScreenSizeYf()*.7f)));
		//pConsole->GetComponentByName("LogDisplay")->GetVar("fontScale")->Set(0.6f);
		MenuMainCreate(GetEntityRoot());

		//let's get notified whenever they get tap points awarded from tapjoy
		m_adManager.m_sig_tappoints_awarded.connect(&OnGotTapPoints);
	}

	//game is thinking.  
}

void App::Draw()
{
	//Use this to prepare for raw GL calls
	PrepareForGL();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//the base handles actually drawing the GUI stuff over everything else, if applicable, which in this case it isn't.
	
	BaseApp::Draw();
	m_adManager.OnRender();

}

void App::OnScreenSizeChange()
{
	BaseApp::OnScreenSizeChange();
}

void App::OnEnterBackground()
{
	//save your game stuff here, as on some devices (Android <cough>) we never get another notification of quitting.
	LogMsg("Entered background");
	BaseApp::OnEnterBackground();
}

void App::OnEnterForeground()
{
	LogMsg("Entered foreground");
	BaseApp::OnEnterForeground();
}

void App::OnMessage( Message &m )
{
	m_IAPManager.OnMessage(m);
	m_adManager.OnMessage(m);
	BaseApp::OnMessage(m);
}

const char * GetAppName() {return "AdTest";}

//the stuff below is for android/webos builds.  Your app needs to be named like this.

//note: these are put into vars like this to be compatible with my command-line parsing stuff that grabs the vars

const char * GetBundlePrefix()
{
	char * bundlePrefix = "com.rtsoft.";
	return bundlePrefix;
}

const char * GetBundleName()
{
	char * bundleName = "rtadtest";
	return bundleName;
}


