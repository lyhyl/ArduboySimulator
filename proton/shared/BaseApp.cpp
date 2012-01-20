#include "PlatformPrecomp.h"
#include "BaseApp.h"
#include "Renderer/RTGLESExt.h"

#if defined( WIN32)&& defined(_DEBUG)
	//useful for debugging texture unloading, but we don't really need to do it for Windows builds.
	//If defined, losing focus (clicking outside the window) will completely unload all textures, and clicking
	//back will reload them.  If you're doing custom texture stuff, you'll probably want to hook into the
	//m_sig_unloadSurfaces and m_sig_loadSurfaces signals to know when to do it.

	//#define C_SURFACE_UNLOAD_TEXTURES
#endif

#ifdef _IRR_STATIC_LIB_
#include "Irrlicht/IrrlichtManager.h"
#endif

Entity * GetEntityRoot() 
{
	assert(IsBaseAppInitted() && "Base app should be initted before calling this");
	return GetBaseApp()->GetEntityRoot();
}

RenderBatcher g_globalBatcher;
bool g_isLoggerInitted = false;
bool IsBaseAppInitted()
{
	return g_isLoggerInitted;
}

BaseApp::BaseApp()
{
		m_bCheatMode = false;
		m_memUsed = m_texMemUsed = 0;	
		g_isLoggerInitted = true;	
		m_bInitted = false;
		m_bConsoleVisible = false;
		m_bManualRotation = false;
		SetFPSVisible(false);
		m_bIsInBackground = false;
		SetInputMode(INPUT_MODE_NORMAL);
		
		m_touchTracker.resize(C_MAX_TOUCHES_AT_ONCE);
		ClearError();
}

BaseApp::~BaseApp()
{
	g_isLoggerInitted = false;
}

void BaseApp::Kill()
{
	delete this;
}

eTimingSystem GetTiming()
{
	return GetBaseApp()->GetActiveTimingSystem();
}

void BaseApp::PrintGLString(const char *name, GLenum s)
{
	const char *v = (const char *) glGetString(s);
	LogMsg("GL %s = %s\n", name, v);
}

void BaseApp::InitializeGLDefaults()
{
	glMatrixMode(GL_MODELVIEW);
	glDepthMask(GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glDisable( GL_BLEND );
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
	glEnableClientState(GL_VERTEX_ARRAY);	
	glDisableClientState(GL_COLOR_ARRAY);	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable( GL_LIGHTING );
	glDepthFunc(GL_LEQUAL);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4x(1 << 16, 1 << 16, 1 << 16, 1 << 16);
	glClearColor(0,0,0,255);
}

bool BaseApp::Init()
{
	
	m_gameTimer.Reset();
	GetEntityRoot()->SetName("root");

	if (m_bInitted)	
	{
		LogMsg("Why are we initting more than once?");
		return true;
	}

	m_bInitted = true;

	CHECK_GL_ERROR();

	PrintGLString("Version", GL_VERSION);
	PrintGLString("Vendor", GL_VENDOR);
	PrintGLString("Renderer", GL_RENDERER);
	PrintGLString("Extensions", GL_EXTENSIONS);

	InitializeGLDefaults();
	
	GLint depthbits;
	glGetIntegerv(GL_DEPTH_BITS, &depthbits);
	LogMsg("GL depth buffer: %d bit", depthbits);

	CHECK_GL_ERROR();

	if (GetAudioManager())
	{
		GetAudioManager()->Init();
	}
	return true;
}

void DrawConsole()
{
	//not implemented
}

void BaseApp::Draw()
{
#ifdef _DEBUG
//LogMsg("**********FRAME START");
#endif
    VariantList vList(Variant(0,0));
    
	m_sig_render(&vList);

	if (GetFPSVisible())
	{
		char stTemp[256];
		
#ifdef _IRR_STATIC_LIB_
		
		int prims = 0;
		if (GetIrrlichtManager()->GetDriver())
		{
			prims = GetIrrlichtManager()->GetDriver()->getPrimitiveCountDrawn();
		}

		sprintf(stTemp, "FPS: %d - Prims: %d M: %.2f, T: %.2f A: %.2f F: %.2f", m_gameTimer.GetFPS(), 
			prims,  (float(m_memUsed)/1024)/1024, (float(m_texMemUsed)/1024)/1024,  float(GetAudioManager()->GetMemoryUsed()/1024)/ 1024, float(GetFreeMemory()/1024)/ 1024);
#else
		sprintf(stTemp, "fps: %d - M: %.2f, T: %.2f A: %.2f F: %.2f", m_gameTimer.GetFPS(), (float(m_memUsed)/1024)/1024, (float(m_texMemUsed)/1024)/1024,  float(GetAudioManager()->GetMemoryUsed()/1024)/ 1024, float(GetFreeMemory()/1024)/ 1024);
#endif		
		if (GetFont(FONT_SMALL)->IsLoaded())
		{
			GetFont(FONT_SMALL)->DrawScaled(2,2, stTemp, 0.7f);
		}
	}

	//draw the console messages?
	if (GetConsoleVisible())
	{
		DrawConsole();
	}

	switch (GetLastError())
	{
	case ERROR_MEM:
		GetFont(FONT_SMALL)->DrawScaled(2,14, "LOW MEM!", 0.7f);
		break;

	case ERROR_SPACE:
		GetFont(FONT_SMALL)->DrawScaled(2,14, "LOW STORAGE SPACE!", 0.7f);
		break;
	}

	SetupOrtho();
	g_globalBatcher.Flush();
}

void BaseApp::Update()
{
	m_gameTimer.Update();
	if (GetMessageManager()) GetMessageManager()->Update();
	if (GetAudioManager()) GetAudioManager()->Update();
	m_sig_update(NULL);
}

void BaseApp::OnScreenSizeChange()
{
	
#ifdef _DEBUG
	//LogMsg("Changing screen-size to %d, %d, %d", GetScreenSizeX(), GetScreenSizeY(), GetOrientation());
#endif
	
	GenerateSetPerspectiveFOV(C_APP_FOV, GetScreenSizeXf()/ GetScreenSizeYf(),0.1f,500.0f);
}

void BaseApp::SetConsoleVisible( bool bNew )
{
	m_bConsoleVisible = bNew;
}

void BaseApp::OnMessage(Message &m)
{
	static VariantList v;
	
	v.Reset();
	
	switch (m.GetClass())
	{
	case MESSAGE_CLASS_GUI:
			switch (m.GetType())
			{
			case MESSAGE_TYPE_GUI_CLICK_START:
			case MESSAGE_TYPE_GUI_CLICK_END:
				{
					v.Get(0).Set((float)m.GetType());
					v.Get(1).Set(float(m.GetParm1()), float(m.GetParm2()) );
					v.Get(2).Set(uint32(m.GetParm3()));
			
					if (m.GetType() == MESSAGE_TYPE_GUI_CLICK_START)
					{
						m_touchTracker[m.GetParm3()].SetIsDown(true);
						m_touchTracker[m.GetParm3()].SetPos(v.Get(1).GetVector2());
						m_touchTracker[m.GetParm3()].SetWasHandled(false);
					} else
					{
						m_touchTracker[m.GetParm3()].SetIsDown(false);
					}


					m_sig_input(&v);
					break;
				}
			
			case MESSAGE_TYPE_GUI_CLICK_MOVE:
			case MESSAGE_TYPE_GUI_CLICK_MOVE_RAW:
				{
				
					if (!m_touchTracker[m.GetParm3()].IsDown())
					{
						//ignore this, we don't want a move message from something that isn't fricken' down.  At least
						//one known HP device is known to do this...
						break;
					}

					if (m_inputMode == INPUT_MODE_NORMAL)
					{
						v.Get(0).Set((float)m.GetType());
						v.Get(1).Set(float(m.GetParm1()), float(m.GetParm2()) );
						v.Get(2).Set(uint32(m.GetParm3()));
						if (m.GetType() == MESSAGE_TYPE_GUI_CLICK_MOVE)
						m_touchTracker[m.GetParm3()].SetPos(v.Get(1).GetVector2());
						m_sig_input(&v);
					} else
					{
						v.Get(0).Set((float)m.GetType());
						v.Get(1).Set(float(m.GetParm1()), float(m.GetParm2()) );
						v.Get(2).Set(uint32(m.GetParm3()));
						if (m.GetType() == MESSAGE_TYPE_GUI_CLICK_MOVE)
						m_touchTracker[m.GetParm3()].SetPos(v.Get(1).GetVector2());
						m_sig_input_move(&v);
					}

					break;
				}
			
			case MESSAGE_TYPE_GUI_ACCELEROMETER:
				{
					v.Get(0).Set((float)m.GetType());
					v.Get(1).Set(m.Get().GetVector3());
					m_sig_accel(&v);				
					break;
				}
			//like MESSAGE_TYPE_GUI_CHAR, but handles up AND down events, and ignores things like key-repeat, better for
			//arcade action
			case MESSAGE_TYPE_GUI_CHAR_RAW:
				{
					v.Get(0).Set(uint32(m.GetParm1()));
					v.Get(1).Set(uint32(m.GetParm2()));
					v.Get(2).Set(uint32(m.GetParm3()));
					m_sig_raw_keyboard(&v);
					break;
				}
			
			//usually used for text input
			case MESSAGE_TYPE_GUI_CHAR:
				{
#ifdef _DEBUG
					//LogMsg("Got char: %c (%d)", m.GetParm1(), int(m.GetParm1()));
#endif
					v.Get(0).Set((float)m.GetType());
					v.Get(1).Set(0,0);
					v.Get(2).Set(uint32(m.GetParm1()));
					m_sig_input(&v);
					break;
				}
			
			case MESSAGE_TYPE_GUI_PASTE:
				{
					v.Get(0).Set((float)m.GetType());
					v.Get(1).Set(0,0);
					v.Get(2).Set(m.Get());
					m_sig_input(&v);
					break;
				}
	
			case MESSAGE_TYPE_GUI_TRACKBALL:
				{
					v.Get(0).Set((float)m.GetType());
					v.Get(1).Set(m.Get().GetVector3());
					m_sig_trackball(&v);
					break;
				}

			case MESSAGE_TYPE_OS_CONNECTION_CHECKED:
				{
					v.Get(0).Set((float)m.GetType());
					v.Get(1).Set(float(m.GetParm1()), float(m.GetParm2()) );
					m_sig_os(&v);
				}
				break;
			
			case MESSAGE_TYPE_GUI_TOGGLE_FULLSCREEN:
				{
					OnFullscreenToggleRequest();
					break;
				}
				break;
			}
	break;
		
	case MESSAGE_CLASS_GAME:

		switch (m.GetType())
		{
		case MESSAGE_TYPE_PLAY_SOUND:
			if (GetAudioManager())
			{
				GetAudioManager()->Play(m.GetVarName());
			}
			break;
		case MESSAGE_TYPE_SET_SOUND_ENABLED:
			GetAudioManager()->SetSoundEnabled(m.Get().GetUINT32() != 0);
			break;
		case MESSAGE_TYPE_PRELOAD_SOUND:
			if (GetAudioManager())
			{
				GetAudioManager()->Preload(ReplaceMP3(m.GetVarName()));
			}
			break;

		case MESSAGE_TYPE_PLAY_MUSIC:
			if (GetAudioManager())
			{
				GetAudioManager()->Play(ReplaceMP3(m.GetVarName()), true, true);
			}
			break;

		case MESSAGE_TYPE_VIBRATE:

			if (GetAudioManager())
			{
				GetAudioManager()->Vibrate(m.Get().GetUINT32());
			}
			break;
		}
		break;
	}
}

void BaseApp::AddOSMessage( OSMessage &m )
{
	m_OSMessages.push_back(m);
}

void BaseApp::KillOSMessagesByType(OSMessage::eMessageType type)
{
	//It's a deque, making it tricky to delete stuff from the middle.  I'll do it this way, speed isn't important
	//as this is used rarely.

	deque <OSMessage>::iterator itor = m_OSMessages.begin();

	deque <OSMessage> temp;

	for (;itor != m_OSMessages.end(); itor++)
	{
		if (itor->m_type != type)
		{
			temp.push_back(*itor);
		}
	}

	m_OSMessages = temp;
}

eTimingSystem BaseApp::GetActiveTimingSystem()
{
	if (GetGameTickPause()) return TIMER_SYSTEM;
	return TIMER_GAME;
}

unsigned int BaseApp::GetTickTimingSystem( eTimingSystem timingSystem )
{
	if (timingSystem == TIMER_SYSTEM) return m_gameTimer.GetTick();

	assert(timingSystem == TIMER_GAME);
	return m_gameTimer.GetGameTick();
}

void LogError ( const char* traceStr, ... )
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	memset ( (void*)buffer, 0, logSize );

	va_start ( argsVA, traceStr );
#ifdef WIN32
	vsnprintf_s( buffer, logSize, logSize, traceStr, argsVA );
#else
	vsnprintf( buffer, logSize, traceStr, argsVA );
#endif
	va_end( argsVA );
	LogMsg("ERROR: %s", buffer);

	GetBaseApp()->SetConsoleVisible(true); //make sure we see the error
	//assert(!"Got an error, your majesty...");
}

unsigned int GetTick( eTimingSystem timingSystem )
{
	return GetBaseApp()->GetTickTimingSystem(timingSystem);
}

ResourceManager * GetResourceManager()
{
	return GetBaseApp()->GetResourceManager();
}

void BaseApp::SetManualRotationMode( bool bRotation )
{
	//if (GetPlatformID() == PLATFORM_ID_BBX) bRotation = false; //on BBX we never have to do that
	LogMsg("AppManualRotation set to %d", int(bRotation));
	m_bManualRotation = bRotation;
}

void BaseApp::OnMemoryWarning()
{
	LogMsg("Got memory warning");
}

void BaseApp::OnEnterBackground()
{
	if (!m_bIsInBackground)
	{
		m_bIsInBackground = true;
#ifdef _DEBUG	
		LogMsg("Entering background");
#endif
	 
	#ifdef C_SURFACE_UNLOAD_TEXTURES
		m_sig_unloadSurfaces();	
	#endif
	
		
	if (GetEmulatedPlatformID() != PLATFORM_ID_ANDROID)
	{
		//android will do it elsewhere, but for other platforms we fake this message here
		m_sig_pre_enterbackground(NULL); 
	}

		m_sig_enterbackground(NULL);
	}

	GetAudioManager()->Suspend();
}

void BaseApp::OnEnterForeground()
{
	GetAudioManager()->Resume();

	if (m_bIsInBackground)
	{
		m_bIsInBackground = false;
#ifdef _DEBUG
		LogMsg("Entering foreground");
#endif
	#ifdef C_SURFACE_UNLOAD_TEXTURES
		
		if (GetEmulatedPlatformID() != PLATFORM_ID_ANDROID)
		{
			//emulate the android way a bit better, helps when fixing bugs under windows
			InitializeGLDefaults();
			LogMsg("gl defaults set");
			OnScreenSizeChange();
			LogMsg("OnScreensizechange done");
		}
		
		GetBaseApp()->m_sig_loadSurfaces(); //for anyone who cares
	#endif
		m_sig_enterforeground(NULL);
	}
}

void BaseApp::AddCommandLineParm( string parm )
{
	m_commandLineParms.push_back(parm);
}

vector<string> BaseApp::GetCommandLineParms()
{
	return m_commandLineParms;
}

void BaseApp::SetAccelerometerUpdateHz(float hz) //another way to think of hz is "how many times per second to update"
{
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_SET_ACCELEROMETER_UPDATE_HZ;
	o.m_x = hz;
	GetBaseApp()->AddOSMessage(o);
}

void BaseApp::SetFPSLimit(float fps) 
{
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_SET_FPS_LIMIT;
	o.m_x = fps;
	GetBaseApp()->AddOSMessage(o);
}

void BaseApp::SetVideoMode(int width, int height, bool bFullScreen, float aspectRatio) //aspectRatio should be 0 to ignore
{
	//this message is only going to be processed by platforms that can change size during runtime and have such a thing as fullscreen
	
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_SET_VIDEO_MODE;
	o.m_x =(float) width;
	o.m_y = (float) height;
	o.m_fullscreen = bFullScreen;	
	o.m_fontSize = aspectRatio;
	GetBaseApp()->AddOSMessage(o);
}



#ifdef _WINDOWS_

//yes, hacky.  Will cleanup when I add the OSX support for this
extern bool g_bIsFullScreen;
#endif

bool BaseApp::OnPreInitVideo()
{
	//only called for desktop systems
	//override in App.* if you want to do something here.  You'd have to
	//extern these vars from main.cpp to change them...
	
	//SetEmulatedPlatformID(PLATFORM_ID_WINDOWS);
	//g_winVideoScreenX = 768;
	//g_winVideoScreenY = 1024;

	
	return true; //no error
}


void BaseApp::OnFullscreenToggleRequest()
{
#ifdef _WINDOWS_
	
	static int savex =0;
	static int savey =0;

	if (g_bIsFullScreen)
	{
		GetBaseApp()->SetVideoMode(savex, savey, false);

	} else
	{
		savex = GetPrimaryGLX();
		savey = GetPrimaryGLY();

		GetBaseApp()->SetVideoMode(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), true);

	}
#endif
}

TouchTrackInfo * BaseApp::GetTouch( int index )
{
	if (this == 0) return NULL;
	if (index >= C_MAX_TOUCHES_AT_ONCE)
	{
		assert(!"Uh no");
		return &m_touchTracker[C_MAX_TOUCHES_AT_ONCE-1];
	}

	return &m_touchTracker[index];
}
