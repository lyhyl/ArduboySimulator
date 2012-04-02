/*
 *  App.cpp
 *  Created by Aki Koskinen on 2/4/12.
 *  For license info, check the license.txt file that should have come with this.
 *
 */

//#include "PlatformPrecomp.h"
#include "App.h"
#include "GUI/MainMenu.h"
//#include "Renderer/LinearParticle.h"
//#include "Entity/EntityUtils.h"
//#include "Renderer/SoftSurface.h"

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
//	SetDefaultButtonStyle(Button2DComponent::BUTTON_STYLE_CLICK_ON_TOUCH_RELEASE);
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


//	LogMsg("Save path is %s", GetSavePath().c_str());

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

	GetBaseApp()->SetFPSVisible(true);
	
//	bool bFileExisted;
//	m_varDB.Load("save.dat", &bFileExisted);
 
	//preload audio
	GetAudioManager()->Preload("audio/click.wav");

	return true;
}

//void App::SaveOurStuff()
//{
//	LogMsg("Saving our stuff");
//	m_varDB.Save("save.dat");
//}

void App::Kill()
{
//	SaveOurStuff();
	BaseApp::Kill();
	g_pApp = NULL;
}

void App::Update()
{
	BaseApp::Update();

	if (!m_bDidPostInit)
	{
		m_bDidPostInit = true;
//		m_special = GetSystemData() != C_PIRATED_NO;

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

void App::OnEnterBackground()
{
	BaseApp::OnEnterBackground();
//	SaveOurStuff();

}
void App::OnScreenSizeChange()
{
	BaseApp::OnScreenSizeChange();
}

//void App::GetServerInfo( string &server, uint32 &port )
//{
//#if defined (_DEBUG) && defined(WIN32)
//	server = "localhost";
//	port = 8080;

//	//server = "www.rtsoft.com";
//	//port = 80;
//#else

//	server = "rtsoft.com";
//	port = 80;
//#endif
//}

//int App::GetSpecial()
//{
//	return m_special; //1 means pirated copy
//}

//Variant * App::GetVar( const string &keyName )
//{
//	return GetShared()->GetVar(keyName);
//}

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
