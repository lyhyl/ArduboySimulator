#include "BaseApp.h"
#include "HTML5Utils.h"
#include <SDL.h>
//#include <GL/glfw.h>
#include <emscripten/html5.h>
#include <emscripten/emscripten.h>

using namespace std;

bool g_bAppFinished = false;

SDL_Surface *g_screen = NULL;
SDL_Joystick *g_pSDLJoystick = NULL;
CL_Vec3f g_accelHold = CL_Vec3f(0,0,0);

bool g_isInForeground = true;

int g_winVideoScreenX = 0;
int g_winVideoScreenY = 0;
Uint32 g_nextFrameTick = 0;
Uint32 g_frameDelayMS = 0;

int GetPrimaryGLX() 
{
	return g_winVideoScreenX;
}

int GetPrimaryGLY() 
{
	return g_winVideoScreenY;
}




EM_BOOL on_canvassize_changed(int eventType, const void *reserved, void *userData)
{
	int w, h, fs;
	emscripten_get_canvas_size(&w, &h, &fs);
	double cssW, cssH;
	emscripten_get_element_css_size(0, &cssW, &cssH);
	LogMsg("Resized: RTT: %dx%d, CSS : %02gx%02g\n", w, h, cssW, cssH);
	return 0;
}

void requestFullscreen(int scaleMode, int canvasResolutionScaleMode, int filteringMode)
{
	EmscriptenFullscreenStrategy s;
	memset(&s, 0, sizeof(s));
	s.scaleMode = scaleMode;
	s.canvasResolutionScaleMode = canvasResolutionScaleMode;
	s.filteringMode = filteringMode;
	s.canvasResizedCallback = on_canvassize_changed;
	int ret = emscripten_request_fullscreen_strategy(0, 1, &s);

}


void enterSoftFullscreen(int scaleMode, int canvasResolutionScaleMode, int filteringMode)
{
	EmscriptenFullscreenStrategy s;
	memset(&s, 0, sizeof(s));
	s.scaleMode = scaleMode;
	s.canvasResolutionScaleMode = canvasResolutionScaleMode;
	s.filteringMode = filteringMode;
	s.canvasResizedCallback = on_canvassize_changed;
	int ret = emscripten_enter_soft_fullscreen(0, &s);
	
}

bool InitSDLScreen()
{
	//Uint32 videoFlags = SDL_HWSURFACE;
	//SDL_OPENGL;
	//Uint32 videoFlags = SDL_SWSURFACE;
	Uint32 videoFlags = SDL_OPENGL;

	LogMsg("Setting video mode");
	g_screen = SDL_SetVideoMode(GetPrimaryGLX() , GetPrimaryGLY(), 32, videoFlags);

	if (g_screen == NULL)
	{
		// couldn't make that screen
		LogMsg("error making screen");
		return false;
	}

	LogMsg("Setting up GLES to %d by %d.", g_screen->w, g_screen->h);
	return true;
}
int initSDL_GLES()
{
	// used to get the result value of SDL operations
	int result;

	// init SDL. This function is all it takes to init both
	// the audio and video.
	LogMsg("initting SDL video");
	
	//requestFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT);
    
	
	result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);

	atexit(SDL_Quit);

	if (result != 0)
	{
		LogMsg("Error initting SDL");
		return result;
	}

#if SDL_VERSION_ATLEAST(1, 3, 0)
	// we need to set up OGL to be using the version of OGL we want. This
	// example uses OGL 1.0. 2.0 is a completely different animal. If you fail
	// to speficy what OGL you want to use, it will be pure luck which one you
	// get. Don't leave that up to chance. 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);	// Force this to be version 1
#endif


/*
	EM_ASM(
		SDL.defaults.copyOnLock = false;
	SDL.defaults.discardOnLock = true;
	SDL.defaults.opaqueFrontBuffer = false;
	);
	*/

	InitSDLScreen();
	enterSoftFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE, EMSCRIPTEN_FULLSCREEN_FILTERING_NEAREST);


	//LogMsg((const char*)glGetString(GL_VERSION));

	
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
	

	/*
		if (glfwInit() != GL_TRUE) {
			printf("glfwInit() failed\n");
			return GL_FALSE;
		}

		if (glfwOpenWindow(GetPrimaryGLX(), GetPrimaryGLY(), 8, 8, 8, 8, 16, 0, GLFW_WINDOW) != GL_TRUE) {
			printf("glfwOpenWindow() failed\n");
			return false;
		}
*/
		
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	return true;
}

void ChangeEmulationOrientationIfPossible(int desiredX, int desiredY, eOrientationMode desiredOrienation)
{
	if (GetForcedOrientation() != ORIENTATION_DONT_CARE) {
		LogMsg("Can't change orientation because SetForcedOrientation() is set.");
	} else {
		SetupScreenInfo(desiredX, desiredY, desiredOrienation);
	}
}

int ConvertSDLKeycodeToProtonVirtualKey(SDLKey sdlkey)
{
	int keycode = sdlkey;
	
	switch (sdlkey)
	{
	case SDLK_LEFT: keycode = VIRTUAL_KEY_DIR_LEFT; break;
	case SDLK_RIGHT: keycode = VIRTUAL_KEY_DIR_RIGHT; break;
	case SDLK_UP: keycode = VIRTUAL_KEY_DIR_UP; break;
	case SDLK_DOWN: keycode = VIRTUAL_KEY_DIR_DOWN; break;
	
	case SDLK_RSHIFT:
	case SDLK_LSHIFT: keycode = VIRTUAL_KEY_SHIFT; break;
	case SDLK_RCTRL:
	case SDLK_LCTRL: keycode = VIRTUAL_KEY_CONTROL; break;
	
	case SDLK_ESCAPE: keycode = VIRTUAL_KEY_BACK; break;

	default:
		if (sdlkey >= SDLK_F1 && sdlkey <= SDLK_F15)
		{
				keycode = VIRTUAL_KEY_F1 + (sdlkey - SDLK_F1);
		}
	}

	return keycode;
}

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
		
			float xPos = ev.button.x;
			float yPos = ev.button.y;
			ConvertCoordinatesIfRequired(xPos, yPos);
			GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_START, xPos, yPos, ev.button.button);
			break;
		}
		
		case SDL_MOUSEBUTTONUP:
		{
			float xPos = ev.button.x;
			float yPos = ev.button.y;
			ConvertCoordinatesIfRequired(xPos, yPos);
			GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_END, xPos, yPos, ev.button.button);
			break;
		}
	
		case SDL_MOUSEMOTION:
		{
			float xPos = ev.motion.x;
			float yPos = ev.motion.y;
			ConvertCoordinatesIfRequired(xPos, yPos);

			// Always pass the same pointer id here since it's not possible to track multiple pointing devices with SDL_MOUSEMOTION
			GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_MOVE, xPos, yPos, SDL_BUTTON_LEFT);
			break;
		}

		case SDL_KEYDOWN:
		{
			if (ev.key.keysym.mod & KMOD_CTRL)
			{
				/*
				switch (ev.key.keysym.sym)
				{
					case SDLK_l: // Left landscape mode
						ChangeEmulationOrientationIfPossible(GetPrimaryGLY(), GetPrimaryGLX(), ORIENTATION_LANDSCAPE_LEFT);
						break;

					case SDLK_r: // Right landscape mode
						ChangeEmulationOrientationIfPossible(GetPrimaryGLY(), GetPrimaryGLX(), ORIENTATION_LANDSCAPE_RIGHT);
						break;

					case SDLK_p: // Portrait mode
						ChangeEmulationOrientationIfPossible(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
						break;

					case SDLK_u: // Upside down portrait mode
						ChangeEmulationOrientationIfPossible(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT_UPSIDE_DOWN);
						break;
				}
				*/

			}

			switch (ev.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				// Escape forces us to quit the app
				// this is also sent when the user makes a back gesture
				g_bAppFinished = true;
				break;
			}

			int vKey = ConvertSDLKeycodeToProtonVirtualKey(ev.key.keysym.sym);
			GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR_RAW, (float)vKey, (float)VIRTUAL_KEY_PRESS);

			if (vKey >= SDLK_SPACE && vKey <= SDLK_DELETE || vKey == SDLK_BACKSPACE || vKey == SDLK_RETURN)
			{
				signed char key = vKey;

				if (ev.key.keysym.mod & KMOD_SHIFT || ev.key.keysym.mod & KMOD_CAPS)
				{
					key = toupper(key);
				}

				GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR, (float)key, (float)VIRTUAL_KEY_PRESS);
			}
		}
		break;
			
		case SDL_KEYUP:
		{
			int vKey = ConvertSDLKeycodeToProtonVirtualKey(ev.key.keysym.sym);
			GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR_RAW, (float)vKey, (float)VIRTUAL_KEY_RELEASE);
		}
		break;
		}
	}
}


void MainEventLoop()
{

	static int oldWidth = GetScreenSizeX();
	static int oldHeight = GetScreenSizeY();

	int width, height, isfs;
	emscripten_get_canvas_size(&width, &height, &isfs);
	
	if (oldWidth != width || oldHeight != height)
	{
		LogMsg("Canvas: %d, %d, %d", width, height, isfs);
		oldWidth = width;
		oldHeight = height;

		g_winVideoScreenX = width;
		g_winVideoScreenY = height;
		//InitSDLScreen();
	//	emscripten_set_canvas_size(width, height);
	}

	if (g_bAppFinished)
	{
		//break; //uhhh...
	}

	SDLEventLoop();

	
//	SDL_LockSurface(g_screen);
	
	GetBaseApp()->Update();
	GetBaseApp()->Draw();

	while (!GetBaseApp()->GetOSMessages()->empty())
	{
		OSMessage m = GetBaseApp()->GetOSMessages()->front();
		GetBaseApp()->GetOSMessages()->pop_front();

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
			if (m.m_x == 0.0f)
			{
				g_frameDelayMS = 0;
			} else
			{
				g_frameDelayMS = (int)(1000.0f / m.m_x);
			}
			break;

		case OSMessage::MESSAGE_SET_VIDEO_MODE:
			{

			
			LogMsg("Got set video mode message");
			GetBaseApp()->KillOSMessagesByType(OSMessage::MESSAGE_SET_VIDEO_MODE);
			//GetBaseApp()->SetVideoMode(Width, Height, false, 0);
			//int isInFullscreen = EM_ASM_INT_V(return !!(document.fullscreenElement || document.mozFullScreenElement || document.webkitFullscreenElement || document.msFullscreenElement));
			int width, height, isFullscreen;
			emscripten_get_canvas_size(&width, &height, &isFullscreen);
			LogMsg("Is full screen is %d - Width: %d Height: %d", isFullscreen, width, height);
//			enterSoftFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT);

			static bool bIsSoftFullscreen = false;

			if (bIsSoftFullscreen)
			{
				LogMsg("Leaving fullscreen mode");
				emscripten_exit_soft_fullscreen();
				bIsSoftFullscreen = false;
			} else
			{
				enterSoftFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF, EMSCRIPTEN_FULLSCREEN_FILTERING_NEAREST);
				
				//emscripten_run_script("Module[\"requestFullScreen\"](false,false);"); for security reasons browsers won't let you do this from a non-button
				bIsSoftFullscreen = true;
			}

			/*
#if SDL_VERSION_ATLEAST(2,0,0)
			SDL_SetWindowFullscreen(g_screen, SDL_WINDOW_FULLSCREEN);
#else
			SDL_WM_ToggleFullScreen(g_screen);
#endif
			*/
		


			
			LogMsg("Post setting: Is full screen is %d - Width: %d Height: %d", isFullscreen, width, height);

			}
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

	//glFlush();
	//glFinish();
	SDL_GL_SwapBuffers();

	// Tell SDL to update the whole screen
//	SDL_UpdateRect(g_screen, 0, 0, GetPrimaryGLX(), GetPrimaryGLY());    
	
	//SDL_UnlockSurface(g_screen);

	//SDL_Flip(g_screen);

	/*
	// Control FPS and give some time for other processes too
	Uint32 sleepTime = 1;

	if (g_frameDelayMS != 0)
	{
		Uint32 ticksNow = SDL_GetTicks();

		if (g_nextFrameTick != 0 && g_nextFrameTick > ticksNow)
		{
			sleepTime = g_nextFrameTick - ticksNow;
		}
	}

	SDL_Delay(sleepTime);
	g_nextFrameTick = SDL_GetTicks() + g_frameDelayMS;
	*/

}

int main(int argc, char *argv[])
{
	if (argc > 1) 
	{
	}
	
	srand( (unsigned)time(NULL) );
	RemoveFile("log.txt", false);
	g_winVideoScreenX = 480;
	g_winVideoScreenY = 320;
	
	GetBaseApp()->OnPreInitVideo(); //gives the app level code a chance to override any of these parms if it wants to

	//SetEmulatedPlatformID(PLATFORM_ID_LINUX);
		SetForcedOrientation(ORIENTATION_DONT_CARE);
	
	GetBaseApp()->OnPreInitVideo(); //gives the app level code a chance to override any of these parms if it wants to

	if (!InitSDL())
	{
		LogMsg("Couldn't init SDL: %s", SDL_GetError());
		goto cleanup;
	}

	LogMsg("Setting up joystick");

	SDL_JoystickEventState(SDL_IGNORE);
	
	LogMsg("Setting up screen");
	SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
	
	LogMsg("Initting BaseApp");
	if (!GetBaseApp()->Init())
	{
		LogError("Couldn't initialize game. Yeah.\n\nDid everything unzip right?");
		goto cleanup;
	}

	GetBaseApp()->OnScreenSizeChange();

	int w, h, fs;
	emscripten_get_canvas_size(&w, &h, &fs);
	double cssW, cssH;
	emscripten_get_element_css_size(0, &cssW, &cssH);
	LogMsg("Init: RTT size: %dx%d, CS: %02gx%02g\n", w, h, cssW, cssH);


	LogMsg("Starting main loop");
	emscripten_set_main_loop(MainEventLoop, 0, 1);
	

	LogMsg("Finished running");
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
