/*
 *  App.cpp
 *  Created by Aki Koskinen on 2/4/12.
 *  For license info, check the license.txt file that should have come with this.
 */

#include "App.h"
#include "GUI/MainMenu.h"

MessageManager g_messageManager;
MessageManager * GetMessageManager() {return &g_messageManager;}

FileManager g_fileManager;
FileManager * GetFileManager() {return &g_fileManager;}

AudioManager g_audioManager;
AudioManager * GetAudioManager() {return &g_audioManager;}

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
}

void App::OnExitApp(VariantList *pVarList)
{
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_FINISH_APP;
	GetBaseApp()->AddOSMessage(o);
}

bool App::Init()
{
	SetManualRotationMode(true);

    bool bScaleScreenActive = true; //if true, we'll stretch every screen to the coords below
    int scaleToX = 480;
	int scaleToY = 320;
    
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
            if (bScaleScreenActive)
                SetupFakePrimaryScreenSize(scaleToX,scaleToY); //game will think it's this size, and will be scaled up
		} 
else
		{
			//but the phones do
			SetLockedLandscape(true); //we don't allow portrait mode for this game
            if (bScaleScreenActive)
                SetupFakePrimaryScreenSize(scaleToX,scaleToY); //game will think it's this size, and will be scaled up
		}
		
		break;

		case PLATFORM_ID_IOS:
			SetLockedLandscape(true); //we stay in portrait but manually rotate, gives better fps on older devices
            if (bScaleScreenActive)
                SetupFakePrimaryScreenSize(scaleToX,scaleToY); //game will think it's this size, and will be scaled up
			break;
			
	default:
		
		//Default settings for other platforms

		SetLockedLandscape(false); //we don't allow portrait mode for this game
		SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
            if (bScaleScreenActive)
                SetupFakePrimaryScreenSize(scaleToX,scaleToY); //game will think it's this size, and will be scaled up
	}

	if (m_bInitted)	
	{
		return true;
	}
	
	if (!BaseApp::Init()) return false;

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

	GetBaseApp()->SetFPSVisible(true);
	
	//preload audio
	GetAudioManager()->Preload("audio/click.wav");

	return true;
}

void App::Kill()
{
	BaseApp::Kill();
	g_pApp = NULL;
}

void App::Update()
{
	BaseApp::Update();

	if (!m_bDidPostInit)
	{
		m_bDidPostInit = true;

		//build a dummy entity called "GUI" to put our GUI menu entities under
		Entity *pGUIEnt = GetEntityRoot()->AddEntity(new Entity("GUI"));
		MainMenuCreate(pGUIEnt);
	}
}

void App::Draw()
{
	PrepareForGL();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	BaseApp::Draw();
}

const char * GetAppName() {return "SpriteSheets";}

//for palm webos and android
const char * GetBundlePrefix()
{
	const char * bundlePrefix = "com.rtsoft.";
	return bundlePrefix;
}

const char * GetBundleName()
{
	const char * bundleName = "rtspritesheets";
	return bundleName;
}
