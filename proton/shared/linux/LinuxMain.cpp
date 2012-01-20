#include "BaseApp.h"
#include "LinuxUtils.h"
#include <SDL.h>

using namespace std;

bool g_bAppFinished = false;

SDL_Surface *g_screen = NULL;
SDL_Joystick *g_pSDLJoystick = NULL;
CL_Vec3f g_accelHold = CL_Vec3f(0,0,0);

bool g_isInForeground = true;

int g_winVideoScreenX = 0;
int g_winVideoScreenY = 0;

int GetPrimaryGLX() 
{
	return g_winVideoScreenX;
}

int GetPrimaryGLY() 
{
	return g_winVideoScreenY;
}

int initSDL_GLES()
{
	// used to get the result value of SDL operations
	int result;

	// init SDL. This function is all it takes to init both
	// the audio and video.
	result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);
	if (result < 0)
	{
		return result;
	}

#if SDL_VERSION_ATLEAST(1, 3, 0)
	// we need to set up OGL to be using the version of OGL we want. This
	// example uses OGL 1.0. 2.0 is a completely different animal. If you fail
	// to speficy what OGL you want to use, it will be pure luck which one you
	// get. Don't leave that up to chance. 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);	// Force this to be version 1
#endif
	
	Uint32 videoFlags = SDL_OPENGL;
	g_screen = SDL_SetVideoMode(GetPrimaryGLX() , GetPrimaryGLY(), 0, videoFlags);

	if (g_screen == NULL)
	{
		// couldn't make that screen
		LogMsg("error making screen");
		return 1;
	}

	LogMsg("Setting up GLES to %d by %d.", g_screen->w, g_screen->h);
	LogMsg((char*)glGetString(GL_VERSION));

	// This sets the "caption" for whatever the window is. On windows, it's the window
	// title. On the palm, this functionality does not exist and the function is ignored.
	SDL_WM_SetCaption(GetAppName(), NULL);

	// Like SDL, we will have the standard of always returning 0 for
	// success, and nonzero for failure. If we're here, it means success!
	return 0;
}

bool InitSDL()
{
	LogMsg("initting SDL");
	int result = initSDL_GLES(); 
	if (result != 0)
	{
		LogMsg("Error initting SDL: %s", SDL_GetError());
		return false;
	}

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	return true;
}

bool g_leftMouseButtonDown = false; //to help emulate how an iphone works

void SDLEventLoop()
{
	// we'll be using this for event polling
	SDL_Event ev;

	// this is how you poll for events. You can't just
	// ignore this function and go your own way. SDL does
	// critical tasks during SDL_PollEvent. You have to call
	// it, and call it frequently. If you have some very large
	// task to perform, like loading hundreds of images, you'll
	// have to break it up in to bite sized pieces. Don't 
	// starve SDL. 
	while (SDL_PollEvent(&ev)) 
	{
		switch (ev.type) 
		{
		case SDL_QUIT:
			{
				g_bAppFinished = true;
			}
			break; 

		case SDL_JOYAXISMOTION:
			{
				int accelerometer = ev.jaxis.axis;
				float val = float(ev.jaxis.value) / 32768.0f;
				//updateAccelerometer(accelerometer, val);
				
				CL_Vec3f v = CL_Vec3f(0,0,0);

				switch (ev.jaxis.axis)
				{
				case 0: //x
					g_accelHold.x = val;
					break;
				case 1: //y
					g_accelHold.y = val;
					break;
				case 2: //z
					g_accelHold.z = val;
					break;
				}
				GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_ACCELEROMETER, Variant(g_accelHold));

				//LogMsg("Got %s",PrintVector3(v).c_str());
			}
			break;
	
		case SDL_ACTIVEEVENT:
#ifndef RT_RUNS_IN_BACKGROUND
			if (ev.active.gain == 0)
			{
				g_isInForeground = false;
				LogMsg("In background");
				GetBaseApp()->OnEnterBackground();
			} else if (ev.active.gain == 1)
			{
				g_isInForeground = true;
				GetBaseApp()->OnEnterForeground();
				LogMsg("In foreground");
			}
#endif
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			{
				//ev.motion.which should hold which finger id

				g_leftMouseButtonDown = true;
				float xPos = ev.motion.x;
				float yPos = ev.motion.y;
				ConvertCoordinatesIfRequired(xPos, yPos);
				
				GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_START, xPos, yPos, ev.motion.which);
			}
			break;
		
		case SDL_MOUSEBUTTONUP:
			{
				//ev.motion.which should hold which finger id
				g_leftMouseButtonDown = false;
				float xPos = ev.motion.x;
				float yPos = ev.motion.y;
				ConvertCoordinatesIfRequired(xPos, yPos);
				GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_END, xPos, yPos, ev.motion.which);
			}
			break;
	
		case SDL_MOUSEMOTION:     
			{
				float xPos = ev.motion.x;
				float yPos = ev.motion.y;
				
				//ev.motion.which should hold which finger id
				ConvertCoordinatesIfRequired(xPos, yPos);

				if (g_leftMouseButtonDown || GetPlatformID() == PLATFORM_ID_WEBOS)
				{
					GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_MOVE, xPos, yPos, ev.motion.which);
					break;
				}
			}

			break;

		case SDL_KEYDOWN:
			switch (ev.key.keysym.sym)
			{
				//touchpad doesn't seem to send these capslock events at all.  Uh.. why?

				case SDLK_CAPSLOCK:
					//LogMsg("Got Capslock: %d, mod of %d, scan of %d, unicode of %d", ev.key.keysym.sym, ev.key.keysym.mod, ev.key.keysym.scancode, ev.key.keysym.unicode);
					break;
					
				case SDLK_NUMLOCK:
					//LogMsg("Got numlock: %d, mod of %d, scan of %d, unicode of %d", ev.key.keysym.sym, ev.key.keysym.mod, ev.key.keysym.scancode, ev.key.keysym.unicode);
					break;

				case SDLK_ESCAPE:
					// Escape forces us to quit the app
					// this is also sent when the user makes a back gesture
					g_bAppFinished = true;
					break;
				
				default:
					{
						if (ev.key.keysym.sym >= 32 && ev.key.keysym.sym <= 128 || ev.key.keysym.sym == 8 || ev.key.keysym.sym == 13)
						{
							signed char key = ev.key.keysym.sym;

							if (ev.key.keysym.mod & KMOD_LSHIFT || ev.key.keysym.mod & KMOD_RSHIFT || ev.key.keysym.mod & KMOD_CAPS)
							{
								key = toupper(key);
							}

							GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR, (float)key, 0.0f); 

						} else
						{
							//LogMsg("Got unknown key: %d", ev.key.keysym.sym);
						}
					}
					break;
			}
		break;
		}
	}
}

int main(int argc, char *argv[])
{
	srand( (unsigned)time(NULL) );
	RemoveFile("log.txt", false);

//	InitVideoSize();
	// Simplified InitVideoSize() from win version to this:
	g_winVideoScreenX = 320;
	g_winVideoScreenY = 480;
	SetEmulatedPlatformID(PLATFORM_ID_LINUX);
	SetForcedOrientation(ORIENTATION_DONT_CARE);
	GetBaseApp()->OnPreInitVideo(); //gives the app level code a chance to override any of these parms if it wants to

	if (!InitSDL())
	{
		LogMsg("Couldn't init SDL: %s", SDL_GetError());
		goto cleanup;
	}

	SDL_JoystickEventState(SDL_IGNORE);
	
	SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
	
	if (!GetBaseApp()->Init())
	{
		LogError("Couldn't initialize game. Yeah.\n\nDid everything unzip right?");
		goto cleanup;
	}

	static unsigned int gameTimer = 0;
	static unsigned int fpsTimerLoopMS = 0;
	GetBaseApp()->OnScreenSizeChange();

	while(1)
	{
		if (g_bAppFinished) break;
		SDLEventLoop();

		if (fpsTimerLoopMS != 0)
		{
			while (gameTimer > SDL_GetTicks())
			{
				SDL_Delay(1); 
			}
			gameTimer = SDL_GetTicks()+fpsTimerLoopMS;
		}

		if (g_isInForeground) 
		{
			GetBaseApp()->Update();
		}

		GetBaseApp()->Draw();

		while (!GetBaseApp()->GetOSMessages()->empty())
		{
			OSMessage m = GetBaseApp()->GetOSMessages()->front();
			GetBaseApp()->GetOSMessages()->pop_front();
			//LogMsg("Got OS message %d, %s", m.m_type, m.m_string.c_str());

			switch (m.m_type)
			{
				case OSMessage::MESSAGE_CHECK_CONNECTION:
					//pretend we did it
					GetMessageManager()->SendGUI(MESSAGE_TYPE_OS_CONNECTION_CHECKED, (float)RT_kCFStreamEventOpenCompleted, 0.0f);	
					break;
			
				case OSMessage::MESSAGE_OPEN_TEXT_BOX:
					break;
				
				case OSMessage::MESSAGE_CLOSE_TEXT_BOX:
					break;
					
				case OSMessage::MESSAGE_SET_FPS_LIMIT:
					fpsTimerLoopMS = (int) (1000.0f/m.m_x);
					break;

				case OSMessage::MESSAGE_SET_ACCELEROMETER_UPDATE_HZ:
					if( SDL_NumJoysticks() > 0 )
					{
						if (m.m_x == 0)
						{
							//disable it if needed
							if (g_pSDLJoystick)
							{
								SDL_JoystickClose(g_pSDLJoystick);
								g_pSDLJoystick = NULL;
								SDL_JoystickEventState(SDL_IGNORE);
							}
						} else
						{
							if (!g_pSDLJoystick)
							{
								g_pSDLJoystick = SDL_JoystickOpen(0);
								SDL_JoystickEventState(SDL_ENABLE);
							}
						}
					}
					break;
			}
		}

		// You should always have a little delay (recommend 10ms)
		// at the end of your event loop. This keeps SDL from
		// hogging all the CPU time. 
		glFlush();
		glFinish();
		SDL_GL_SwapBuffers();
		SDL_Delay(10); 
	}
	
	GetBaseApp()->Kill();

cleanup:
	if (g_pSDLJoystick)
	{
		SDL_JoystickClose(g_pSDLJoystick);
		g_pSDLJoystick = NULL;
	}

	SDL_Quit();

	return 0;
}
