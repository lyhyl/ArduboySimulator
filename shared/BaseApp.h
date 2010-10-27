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
	float m_x, m_y; //location of text box
	float m_sizeX, m_sizeY;
	float m_fontSize;
	string m_string; //default text of text box
	uint32 m_parm2; //well, I use it to describe the input box type with the input stuff

};

enum eInputMode
{
	INPUT_MODE_NORMAL,
	INPUT_MODE_SEPARATE_MOVE_TOUCHES //move messages will only be on sig_input_move if this is set, useful because
	//on a 4 player game it's just too slow to send them to entire trees..
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
	virtual void Draw();
	virtual void Update();
	virtual void OnEnterBackground(); //OS4 got a phonecall or changed apps, should save your junk
	virtual void OnEnterForeground(); //switched back to the app
	virtual void OnScreenSizeChange();
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
	virtual void OnMessage(Message &m);
	RTFont * GetFont(eFont font) {return &m_fontArray[font];}
	void SetInputMode(eInputMode mode){m_inputMode = mode;}
	eInputMode GetInputMode() {return m_inputMode;}
	virtual void OnMemoryWarning();
	//FocusComponents connect to these, which will tricky down their hierarchy
	boost::signal<void (VariantList*)> m_sig_input; //input specific messages from Mac
	boost::signal<void (VariantList*)> m_sig_input_move; //"move" touch messages from the mac, if INPUT_MODE_SEPARATE_MOVE_TOUCHES was set
	boost::signal<void (VariantList*)> m_sig_os; //messages from mac
	boost::signal<void (VariantList*)> m_sig_render;
	boost::signal<void (VariantList*)> m_sig_update;
	boost::signal<void (VariantList*)> m_sig_enterbackground;
	boost::signal<void (VariantList*)> m_sig_enterforeground;
	boost::signal<void (VariantList*)> m_sig_accel; //accelerometer data from iphone
	boost::signal<void (void)> m_sig_unloadSurfaces; //some phones may want you to release surfaces sometimes
	boost::signal<void (void)> m_sig_loadSurfaces; //some phones may want you to release surfaces sometimes

	deque <OSMessage> * GetOSMessages() {return &m_OSMessages;}
	void AddOSMessage(OSMessage &m);
	void SetManualRotationMode(bool bRotation); //if true, we manually rotate our GL and coordinates for the screen.  Only set by the Mac side..
	bool GetManualRotationMode() {return m_bManualRotation;}
	ResourceManager * GetResourceManager() {return &m_resourceManager;}

	void ModMemUsed(int mod) {m_memUsed += mod;}
	void ModTexUsed(int mod) {m_texMemUsed += mod;}

	int GetMemUsed() {return m_memUsed;}
	int GetTexMemUsed() {return m_texMemUsed;}
	
	eErrorType GetLastError() {return m_error;}
	void ClearError() {m_error = ERROR_NONE;}
	void SetLastError(eErrorType error) {m_error = error;}
	bool IsInBackground() {return m_bIsInBackground;} //useful for iOS multitasking
	
	void AddCommandLineParm(string parm);
	vector<string> GetCommandLineParms();
	void SetAccelerometerUpdateHz(float hz) /*another way to think of hz is "how many times per second to update" */;
	void PrintGLString(const char *name, GLenum s);
	bool IsInitted() {return m_bInitted;}
	virtual void InitializeGLDefaults();
	CL_Mat4f &GetProjectionMatrix() {return m_projectionMatrix;}
	Entity * GetEntityRoot() {return &m_entityRoot;}
	
	
	void SetCheatMode(bool bCheatMode) {m_bCheatMode = bCheatMode;}
	bool GetCheatMode() {return m_bCheatMode;}

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

#endif // BaseApp_h__