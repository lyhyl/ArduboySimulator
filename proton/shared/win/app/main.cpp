//THIS FILE IS SHARED BETWEEN ALL GAMES!  I don't always put it in the "shared" folder in the msvc project tree because
//I want quick access to it.

#include "PlatformPrecomp.h"
#include "main.h"
#include "WebOS/SDLMain.h"
#include "App.h"

vector<VideoModeEntry> g_videoModes;
void AddVideoMode(string name, int x, int y, ePlatformID platformID);
void SetVideoModeByName(string name);



void InitVideoSize()
{
#ifdef RT_WEBOS_ARM
	return;
#endif

	AddVideoMode("Windows", 768, 1024, PLATFORM_ID_WINDOWS); //used for Dink

	AddVideoMode("iPhone", 320, 480, PLATFORM_ID_IOS);
	AddVideoMode("iPad", 768, 1024, PLATFORM_ID_IOS);
	AddVideoMode("iPhone4", 640, 960, PLATFORM_ID_IOS);

	AddVideoMode("Palm Pre Plus", 320, 480, PLATFORM_ID_WEBOS);
	AddVideoMode("Pixi", 320, 400, PLATFORM_ID_WEBOS);
	
	AddVideoMode("G1", 320, 480, PLATFORM_ID_ANDROID);
	AddVideoMode("G1 Landscape", 480, 320, PLATFORM_ID_ANDROID);
	AddVideoMode("Nexus One", 480, 800, PLATFORM_ID_ANDROID);
	AddVideoMode("Droid", 480, 854, PLATFORM_ID_ANDROID);
	AddVideoMode("Droid Landscape", 854, 480, PLATFORM_ID_ANDROID);
	AddVideoMode("Nexus One Landscape", 800, 480, PLATFORM_ID_ANDROID); //g_landScapeNoNeckHurtMode should be false when testing

	string desiredVideoMode = "iPhone"; //name needs to match one of the ones defined below
    g_landScapeNoNeckHurtMode = true; //if true, will rotate the screen so we can play in landscape mode in windows without hurting ourselves

	#ifndef _DEBUG
		//desiredVideoMode = "Windows"; //so when I build Dink for windows I don't have to worry about this being set right
	#endif

	SetVideoModeByName(desiredVideoMode);
}

//***************************************************************************

bool g_landScapeNoNeckHurtMode = false;
int winVideoScreenX = 0;
int winVideoScreenY = 0;


void AddVideoMode(string name, int x, int y, ePlatformID platformID)
{
	g_videoModes.push_back(VideoModeEntry(name, x, y, platformID));
}

void SetVideoModeByName(string name)
{
	VideoModeEntry *v = NULL;

	for (unsigned int i=0; i < g_videoModes.size(); i++)
	{
		v = &g_videoModes[i];
		if (v->name == name)
		{
			winVideoScreenX = v->x;
			winVideoScreenY = v->y;
			SetEmulatedPlatformID(v->platformID);
			return;
		}
	}

	LogError("Don't have %s registered as a video mode.", name.c_str());
	assert(!"huh?");
}



#ifdef _IRR_STATIC_LIB_
#include "Irrlicht/IrrlichtManager.h"
using namespace irr;
#endif

#ifdef C_GL_MODE

#define eglGetError glGetError
#define EGLint GLint
#define EGL_SUCCESS GL_NO_ERROR
#endif


int GetPrimaryGLX() 
{
	if (g_landScapeNoNeckHurtMode)
	{
		return winVideoScreenY;
	}

	return winVideoScreenX;
}
int GetPrimaryGLY() 
{
	if (g_landScapeNoNeckHurtMode)
	{
		return winVideoScreenX;
	}

	return winVideoScreenY;
}	

bool g_bHasFocus = true;
int mousePosX = 0;
int mousePosY = 0;
bool g_bAppFinished = false;

#ifndef RT_WEBOS

#define	WINDOW_CLASS _T("AppClass")

#include <windows.h>

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif
bool g_leftMouseButtonDown = false; //to help emulate how an iphone works



int ConvertWindowsKeycodeToProtonVirtualKey(int keycode)
{
	switch (keycode)
	{
	case 37: keycode = VIRTUAL_KEY_DIR_LEFT; break;
	case 39: keycode = VIRTUAL_KEY_DIR_RIGHT; break;
	case 38: keycode = VIRTUAL_KEY_DIR_UP; break;
	case 40: keycode = VIRTUAL_KEY_DIR_DOWN; break;
	case VK_SHIFT: keycode = VIRTUAL_KEY_SHIFT; break;
	case VK_CONTROL: keycode = VIRTUAL_KEY_CONTROL; break;
	}

	return keycode;
}

// Variable set in the message handler to finish the demo

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		// Handles the close message when a user clicks the quit icon of the window
	case WM_CLOSE:
		g_bAppFinished = true;
		
		//PostQuitMessage(0);
		return 1;

	case WM_LBUTTONDOWN:
		{
			if (!g_bHasFocus) break;
			g_leftMouseButtonDown = true;
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam) + GetYOffset();
			ConvertCoordinatesIfRequired(xPos, yPos);
			GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CLICK_START, xPos, yPos);
			break;
		}

	case WM_CHAR:
		{
			if (!g_bHasFocus) break;
			
			GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR, wParam, lParam);  //lParam holds a lot of random data about the press, look it up if
			//you actually want to access it
			return 0;
		}

		break;

	case WM_KILLFOCUS:
		if (g_bHasFocus && IsBaseAppInitted())
		{
			

			GetBaseApp()->OnEnterBackground();
		}

		g_bHasFocus = false;
		break;

	case WM_SETFOCUS:
		if (!g_bHasFocus && IsBaseAppInitted())
		{
			GetBaseApp()->OnEnterForeground();
		}
		g_bHasFocus = true;
		break;

	case WM_COMMAND:
		{
			if (LOWORD(wParam)==IDCANCEL)
			{
				/* ESC key has been pressed*/
				LogMsg("WM command escape");
			}
		}
		break;

	case WM_KEYDOWN:
		if (!g_bHasFocus) break;

		switch (wParam)
		{
			
		case VK_ESCAPE:
			GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR, VIRTUAL_KEY_BACK, VIRTUAL_KEY_PRESS); 
			break;

		case 'L': //left landscape mode
			if (!g_landScapeNoNeckHurtMode)
			{
				SetupScreenInfo(GetPrimaryGLY(), GetPrimaryGLX(), ORIENTATION_LANDSCAPE_LEFT);
			}
			break;

		case 'R': //right landscape mode
			if (!g_landScapeNoNeckHurtMode)
			{
				SetupScreenInfo(GetPrimaryGLY(), GetPrimaryGLX(), ORIENTATION_LANDSCAPE_RIGHT);
			}
			break;

		case 'P': //portrait mode
			if (!g_landScapeNoNeckHurtMode)
			{
				SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
			}
			break;
		case 'U': //Upside down portrait mode
			if (!g_landScapeNoNeckHurtMode)
			{
				SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT_UPSIDE_DOWN);
			}
			break;

		case 'C':

			if (GetKeyState(VK_CONTROL)& 0xfe)
			{
				//LogMsg("Copy");
				GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_COPY, 0, 0);  //lParam holds a lot of random data about the press, look it up if
			}
			break;

		
		case 'V':

			if (GetKeyState(VK_CONTROL)& 0xfe)
			{
				//LogMsg("Paste");
				string text = GetClipboardText();

				if (!text.empty())
				{
					GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_PASTE, Variant(text), 0);  //lParam holds a lot of random data about the press, look it up if
				}
				;
			}
		break;

		/*
		case VK_RETURN:
		case VK_SHIFT:
		case VK_CONTROL:

			GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR, wParam, lParam);  //lParam holds a lot of random data about the press, look it up if

			break;
			*/
		}
		
		{

		
		//send the raw key data as well
		VariantList v;
		const bool isBitSet = lParam & (1 << 30);

		if (!isBitSet)
		{
			GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR_RAW, ConvertWindowsKeycodeToProtonVirtualKey(wParam), 1);  
		}
		//LogMsg("Got key down %d", (int)wParam);
		break;
		}

	case WM_KEYUP:
		{
			//LogMsg("Got key up %d", (int)wParam);
		GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR_RAW, ConvertWindowsKeycodeToProtonVirtualKey(wParam), 0);  
		}
	break;


	case WM_LBUTTONUP:
		{
			if (!g_bHasFocus) break;
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam) + GetYOffset();
			ConvertCoordinatesIfRequired(xPos, yPos);
			GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CLICK_END, xPos, yPos);
			g_leftMouseButtonDown = false;
		}
		break;

	case WM_MOUSEMOVE:
		{
			if (!g_bHasFocus) break;
		
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam) + GetYOffset();
			ConvertCoordinatesIfRequired(xPos, yPos);

			GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CLICK_MOVE_RAW, xPos, yPos);

			if (g_leftMouseButtonDown)
			{
				GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CLICK_MOVE, xPos, yPos);
				break;
			} 
		}

		break;
	
	default:
		break;
	}

	// Calls the default window procedure for messages we did not handle
	return DefWindowProc(hWnd, message, wParam, lParam);
}


bool TestEGLError(HWND hWnd, char* pszLocation)
{

	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
		TCHAR pszStr[256];
		_stprintf_s(pszStr, _T("%s failed (%d).\n"), pszLocation, iErr);
		MessageBox(hWnd, pszStr, _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR *lpCmdLine, int nCmdShow)
{
	
	RemoveFile("log.txt", false);

	if (lpCmdLine[0])
	{
		vector<string> parms = StringTokenize(lpCmdLine, " ");
	
		for (unsigned int i=0; i < parms.size(); i++)
		{
			GetBaseApp()->AddCommandLineParm(parms[i]);
		}
	}
	
	InitVideoSize();

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Windows variables
	HWND				hWnd	= 0;
	HDC					hDC		= 0;

	// EGL variables
#ifndef C_GL_MODE
	EGLDisplay			eglDisplay	= 0;
	EGLConfig			eglConfig	= 0;
	EGLSurface			eglSurface	= 0;
	EGLContext			eglContext	= 0;
	NativeWindowType	eglWindow	= 0;
	EGLint				pi32ConfigAttribs[128];
	int				i;
#else
	int bits = 16;
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

#endif

	srand( (unsigned)GetTickCount() );


	// Register the windows class
	WNDCLASS sWC;
	sWC.style = CS_HREDRAW | CS_VREDRAW;
	sWC.lpfnWndProc = WndProc;
	sWC.cbClsExtra = 0;
	sWC.cbWndExtra = 0;
	sWC.hInstance = hInstance;
	sWC.hIcon = 0;
	sWC.hCursor = 0;
	sWC.lpszMenuName = 0;
	sWC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	sWC.lpszClassName = WINDOW_CLASS;
	ATOM registerClass = RegisterClass(&sWC);
	if (!registerClass)
	{
		MessageBox(0, _T("Failed to register the window class"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
	}

	// Create the eglWindow
	RECT	sRect;
	int offsety = 23;
	int offsetX = 3;

	SetRect(&sRect, 0, 0, GetPrimaryGLX()+offsetX, GetPrimaryGLY()+offsety);

	AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);

	hWnd = CreateWindow( WINDOW_CLASS, GetAppName(), WS_VISIBLE | WS_SYSMENU,
		(GetSystemMetrics(SM_CXSCREEN)-sRect.right)/2, (GetSystemMetrics(SM_CYSCREEN)-sRect.bottom)/2, sRect.right, sRect.bottom, NULL, NULL, hInstance, NULL);

#ifndef C_GL_MODE
	eglWindow = hWnd;
#endif

	// Get the associated device context
	hDC = GetDC(hWnd);
	if (!hDC)
	{
		MessageBox(0, _T("Failed to create the device context"), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		goto cleanup;
	}


#ifndef C_GL_MODE
	eglDisplay = eglGetDisplay((NativeDisplayType) hDC);

	if(eglDisplay == EGL_NO_DISPLAY)
		eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);

	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
	{

		MessageBox(0, _T("eglInitialize() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);

		goto cleanup;
	}

	i = 0;
	pi32ConfigAttribs[i++] = EGL_RED_SIZE;
	pi32ConfigAttribs[i++] = 5;
	pi32ConfigAttribs[i++] = EGL_GREEN_SIZE;
	pi32ConfigAttribs[i++] = 6;
	pi32ConfigAttribs[i++] = EGL_BLUE_SIZE;
	pi32ConfigAttribs[i++] = 5;
	pi32ConfigAttribs[i++] = EGL_ALPHA_SIZE;
	pi32ConfigAttribs[i++] = 0;
	pi32ConfigAttribs[i++] = EGL_SURFACE_TYPE;
	pi32ConfigAttribs[i++] = EGL_WINDOW_BIT;
	pi32ConfigAttribs[i++] = EGL_NONE;

	int iConfigs;
	if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		MessageBox(0, _T("eglChooseConfig() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		goto cleanup;
	}

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

	if(eglSurface == EGL_NO_SURFACE)
	{
		eglGetError(); // Clear error
		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}

	if (!TestEGLError(hWnd, "eglCreateWindowSurface"))
	{
		goto cleanup;
	}

	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
	if (!TestEGLError(hWnd, "eglCreateContext"))
	{
		goto cleanup;
	}


	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	if (!TestEGLError(hWnd, "eglMakeCurrent"))
	{
		goto cleanup;
	}
#else

	//NORMAL GL INIT

	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		goto cleanup;							// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		goto cleanup;										// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		goto cleanup;											// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		goto cleanup;										// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		goto cleanup;										// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
#endif

	SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);

	if (!GetBaseApp()->Init())
	{
		assert(!"Unable to init");
		MessageBox(NULL, "Error initializing the game.  Did you unzip everything right?", "Unable to load stuff", NULL);
		goto cleanup;
	}

	//our main loop

	while(1)
	{

		if (GetAsyncKeyState('Q') && GetAsyncKeyState(VK_MENU))
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		
		if (g_bAppFinished) break;

		if (g_bHasFocus)
		{
			GetBaseApp()->Update();
			GetBaseApp()->Draw();
		} else
		{
			Sleep(50);
		}

		while (!GetBaseApp()->GetOSMessages()->empty())
		{
			OSMessage m = GetBaseApp()->GetOSMessages()->front();
			GetBaseApp()->GetOSMessages()->pop_front();
			//LogMsg("Got OS message %d, %s", m.m_type, m.m_string.c_str());

			switch (m.m_type)
			{
			case OSMessage::MESSAGE_CHECK_CONNECTION:
				//pretend we did it
				GetMessageManager()->SendGUI(MESSAGE_TYPE_OS_CONNECTION_CHECKED, RT_kCFStreamEventOpenCompleted, 0);	
				break;
			case OSMessage::MESSAGE_OPEN_TEXT_BOX:
				break;
			case OSMessage::MESSAGE_CLOSE_TEXT_BOX:
				SetIsUsingNativeUI(false);
				break;
			
			case OSMessage::MESSAGE_FINISH_APP:
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			}
		}
#ifdef C_GL_MODE
		SwapBuffers(hDC);
#else
		eglSwapBuffers(eglDisplay, eglSurface);
		if (!TestEGLError(hWnd, "eglSwapBuffers"))
		{
			goto cleanup;
		}

#endif

		// Managing the window messages
		MSG msg;
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		}
		Sleep(0);
	}

cleanup:

	if (IsBaseAppInitted())
	{
		GetBaseApp()->OnEnterBackground();
		GetBaseApp()->Kill();
	}


#ifdef C_GL_MODE

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}
#else
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglTerminate(eglDisplay);
#endif

	// Release the device context
	if (hDC) ReleaseDC(hWnd, hDC);

	// Destroy the eglWindow
	if (hWnd) DestroyWindow(hWnd);

	WSACleanup(); 
	return 0;
}

void AddText(char *tex ,char *filename)
{
	FILE *          fp;
	if (strlen(tex) < 1) return;

	if (FileExists(filename) == false)
	{

		fp = fopen(filename, "wb");
		fwrite( tex, strlen(tex), 1, fp);       /* current player */
		fclose(fp);
		return;
	} else
	{
		fp = fopen(filename, "ab");
		fwrite( tex, strlen(tex), 1, fp);       /* current player */
		fclose(fp);
	}
}

extern bool g_isLoggerInitted;
void LogMsg ( const char* traceStr, ... )
{
	va_list argsVA;
	const int logSize = 1024*10;
	char buffer[logSize];
	memset ( (void*)buffer, 0, logSize );

	va_start ( argsVA, traceStr );
	vsnprintf_s( buffer, logSize, logSize, traceStr, argsVA );
	va_end( argsVA );
	

	OutputDebugString(buffer);
	OutputDebugString("\n");

	if (g_isLoggerInitted)
	{
		GetBaseApp()->GetConsole()->AddLine(buffer);
	}

	strcat(buffer, "\r\n");
	AddText(buffer, "log.txt");
}
#endif
