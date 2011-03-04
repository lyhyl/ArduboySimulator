//  ***************************************************************
//  BaseApp - Creation date:  03/06/2009
//  -------------------------------------------------------------
//  Robinson Technologies - Check license.txt for license info.
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#pragma once

#ifndef BaseApp_h__
#define BaseApp_h__

#define C_MAX_TOUCHES_AT_ONCE 11


using namespace std;

#include "Manager/GameTimer.h"
#include "Manager/Console.h"
#include "Manager/ResourceManager.h"
#include "Renderer/RenderBatcher.h"

//these aren't really used by this class but makes it easy for me to share these with everyone by only including this header
#include "util/MiscUtils.h"
#include "util/RenderUtils.h"
#include "Manager/MessageManager.h"
#include "FileSystem/FileManager.h"
#include "util/ResourceUtils.h"
#include "util/GLESUtils.h"
#include "util/RenderUtils.h"
#include "GUI/RTFont.h"
#include "Entity/Entity.h"
#include "Audio/AudioManager.h"

#pragma message("Compiling BaseApp.h - this should happen just once per project.\n")

enum eFont
{
	FONT_SMALL,
	FONT_LARGE,
	FONT_FIXED,
	FONT_BASE_COUNT
};

//structure used to pass messages from Proton to the platform glue code on android, win, iOS, etc.  Parms are used differently
//for different messages so it's sort of confusing to use, just find an example and copy it of the message type you are trying
//to use.

struct OSMessage
{
	enum eMessageType
	{
		MESSAGE_NONE,
		MESSAGE_OPEN_TEXT_BOX,
		MESSAGE_CLOSE_TEXT_BOX,
		MESSAGE_CHECK_CONNECTION,
		MESSAGE_SET_FPS_LIMIT,
		MESSAGE_SET_ACCELEROMETER_UPDATE_HZ,
		MESSAGE_FINISH_APP, //only respected by windows and android right now.  webos and iphone don't really need it
		MESSAGE_SET_VIDEO_MODE,
		MESSAGE_USER = 1000
	};

	enum eParmKeyboardType
	{
		PARM_KEYBOARD_TYPE_ASCII,
		PARM_KEYBOARD_TYPE_NUMBERS,
		PARM_KEYBOARD_TYPE_URL,
		PARM_KEYBOARD_TYPE_ASCII_FULL,
		PARM_KEYBOARD_TYPE_EMAIL
	};

	eMessageType m_type;
	int m_parm1; //max length of input box
	float m_x, m_y; //location of text box, or screen size if using MESSAGE_SET_VIDEO_MODE
	float m_sizeX, m_sizeY;
	float m_fontSize; //aspect ratio if using MESSAGE_SET_VIDEO_MODE
	string m_string; //default text of text box
	uint32 m_parm2; //well, I use it to describe the input box type with the input stuff
	bool m_fullscreen; //used with MESSAGE_SET_VIDEO_MODE
};

enum eInputMode
{
	INPUT_MODE_NORMAL,
	INPUT_MODE_SEPARATE_MOVE_TOUCHES //move messages will only be on sig_input_move if this is set, useful because
	//on a 4 player game it's just too slow to send them to entire trees..
};

class TouchTrackInfo
{
public:
	TouchTrackInfo()
	{
		m_bHandled = false;
		m_bIsDown = false;
	}
	
	bool WasHandled() {return m_bHandled;}
	CL_Vec2f GetPos() {return m_vPos;}
	bool IsDown() {return m_bIsDown;}

	void SetIsDown(bool bNew)  {m_bIsDown = bNew;}
	void SetPos(const CL_Vec2f &vPos) {m_vPos = vPos;}
	void SetWasHandled(bool bNew) {m_bHandled = bNew;}
private:
	bool m_bHandled;
	bool m_bIsDown;
	CL_Vec2f m_vPos;
};


class BaseApp
{
public:
	
	//global errors can be set by anybody
	enum eErrorType
	{
		ERROR_NONE,
		ERROR_MEM
	};

	BaseApp();
	virtual ~BaseApp();

	virtual bool Init();
	virtual void Kill();
	virtual bool OnPreInitVideo();
	virtual void Draw();
	virtual void Update();
	virtual void OnEnterBackground(); //OS4 got a phonecall or changed apps, should save your junk
	virtual void OnEnterForeground(); //switched back to the app
	virtual void OnScreenSizeChange();
	virtual void OnFullscreenToggleRequest(); //Alt-Enter on Win, Ctrl-F on Mac - override if you want custom functionality
	void SetConsoleVisible(bool bNew);
	bool GetConsoleVisible() {return m_bConsoleVisible;}
	bool GetFPSVisible() {return m_bFPSVisible;}
	void SetFPSVisible(bool bNew) {m_bFPSVisible = bNew;}
	unsigned int GetGameTick() {return m_gameTimer.GetGameTick();}
	void SetGameTick(unsigned int tick) {m_gameTimer.SetGameTick(tick);}
	unsigned int GetTick() {return m_gameTimer.GetTick();}
	eTimingSystem GetActiveTimingSystem();
	unsigned int GetTickTimingSystem(eTimingSystem timingSystem);
	float GetDelta() {return m_gameTimer.GetDelta();}
	float GetGameDelta() {return m_gameTimer.GetGameDelta();}	
	int GetDeltaTick() {return m_gameTimer.GetDeltaTick();}
	int GetGameDeltaTick() {return m_gameTimer.GetDeltaGameTick();}
	Console * GetConsole() {return &m_console;}
	void SetGameTickPause(bool bNew) {m_gameTimer.SetGameTickPause(bNew);}
	bool GetGameTickPause() {return m_gameTimer.GetGameTickPause();}
	GameTimer * GetGameTimer() {return &m_gameTimer;}
	virtual void OnMessage(Message &m);
	RTFont * GetFont(eFont font) {return &m_fontArray[font];}
	void SetInputMode(eInputMode mode){m_inputMode = mode;}
	eInputMode GetInputMode() {return m_inputMode;}
	virtual void OnMemoryWarning();
	//FocusComponents connect to these, which will tricky down their hierarchy
	boost::signal<void (VariantList*)> m_sig_input; //taps, clicks, and basic keyboard input
	boost::signal<void (VariantList*)> m_sig_input_move; //"move" touch messages, if INPUT_MODE_SEPARATE_MOVE_TOUCHES was set, otherwise they go to m_sig_input
	boost::signal<void (VariantList*)> m_sig_os; //messages from the platform itself (do eMessageType mType = (eMessageType)(int)pVList->m_variant[0].GetFloat(); to get message)
	boost::signal<void (VariantList*)> m_sig_update; //called once per frame, usually.  For your game logic.
	boost::signal<void (VariantList*)> m_sig_render; //called once per frame. You should render but not do game logic.
	boost::signal<void (VariantList*)> m_sig_enterbackground; //game lost focus
	boost::signal<void (VariantList*)> m_sig_enterforeground; //game restored focus
	boost::signal<void (VariantList*)> m_sig_accel; //accelerometer data from iphone
	boost::signal<void (VariantList*)> m_sig_trackball; //used for android trackball move data
	boost::signal<void (VariantList*)> m_sig_arcade_input; //for arcade movement controls like left/right/up/down, if MovementInputComponent is used, trackball/wasd are converted to send through this as well
	boost::signal<void (VariantList*)> m_sig_raw_keyboard; //for raw data from keyboards that give pressed/released messages.  Generally you would convert them into arcade messages
	
	boost::signal<void (void)> m_sig_unloadSurfaces; //some phones may want you to release surfaces sometimes
	boost::signal<void (void)> m_sig_loadSurfaces; //some phones may want you to reload surfaces sometimes

	deque <OSMessage> * GetOSMessages() {return &m_OSMessages;}
	void AddOSMessage(OSMessage &m);
	void SetManualRotationMode(bool bRotation); //if true, we manually rotate our GL and coordinates for the screen.
	bool GetManualRotationMode() {return m_bManualRotation;}
	ResourceManager * GetResourceManager() {return &m_resourceManager;}

	void ModMemUsed(int mod) {m_memUsed += mod;}
	void ModTexUsed(int mod) {m_texMemUsed += mod;}

	int GetMemUsed() {return m_memUsed;}
	int GetTexMemUsed() {return m_texMemUsed;}
	
	eErrorType GetLastError() {return m_error;}
	void ClearError() {m_error = ERROR_NONE;}
	void SetLastError(eErrorType error) {m_error = error;}
	bool IsInBackground() {return m_bIsInBackground;} 
	
	void AddCommandLineParm(string parm);
	vector<string> GetCommandLineParms();
	void SetAccelerometerUpdateHz(float hz); //another way to think of hz is "how many times per second to update"
	void PrintGLString(const char *name, GLenum s);
	bool IsInitted() {return m_bInitted;}
	virtual void InitializeGLDefaults();
	CL_Mat4f * GetProjectionMatrix() {return &m_projectionMatrix;}
	Entity * GetEntityRoot() {return &m_entityRoot;} //an entity created by default, add children to become your entity hierarchy
	
	//not really used by framework, but useful if your app has cheat modes and you want an easy way to enable/disable them
	void SetCheatMode(bool bCheatMode) {m_bCheatMode = bCheatMode;}
	bool GetCheatMode() {return m_bCheatMode;}
	void SetVideoMode(int width, int height, bool bFullScreen, float aspectRatio = 0) /*aspectRatio should be 0 to ignore */;
	void KillOSMessagesByType(OSMessage::eMessageType type);
	void SetFPSLimit(float fps);
	TouchTrackInfo * GetTouch(int index);

protected:
	
	bool m_bConsoleVisible;
	bool m_bFPSVisible;
	bool m_bInitted;
	GameTimer m_gameTimer;
	Console m_console;
	RTFont m_fontArray[FONT_BASE_COUNT];
	deque <OSMessage> m_OSMessages; //simple way to send things to the OS, it will poll this
	bool m_bManualRotation;
	ResourceManager m_resourceManager; 
	eInputMode m_inputMode;
	int m_memUsed;
	int m_texMemUsed;
	eErrorType m_error;
	bool m_bIsInBackground;
	vector<string> m_commandLineParms;

	CL_Mat4f m_projectionMatrix;
	Entity m_entityRoot;
	bool m_bCheatMode;
	vector<TouchTrackInfo> m_touchTracker;
	
};

BaseApp * GetBaseApp(); //supply this yourself.  You create it on the first call if needed.
bool IsBaseAppInitted();
MessageManager * GetMessageManager(); //supply this yourself
FileManager * GetFileManager(); //supply this yourself
AudioManager * GetAudioManager();  //supply this yourself
Entity * GetEntityRoot(); //we supply this

ResourceManager * GetResourceManager();
unsigned int GetTick(eTimingSystem timingSystem = GetBaseApp()->GetActiveTimingSystem()); //faster to write
eTimingSystem GetTiming();
extern RenderBatcher g_globalBatcher; //can be used by anyone
extern bool g_isLoggerInitted;


#endif // BaseApp_h__