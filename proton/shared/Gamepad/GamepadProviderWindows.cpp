#include "PlatformPrecomp.h"
#include "GamepadProviderWindows.h"
#include "GamepadWindows.h"
#include "GamepadManager.h"

HWND hwndMain;

BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam)
{
	HINSTANCE hinst=(HINSTANCE)GetModuleHandle(NULL);

	if((HINSTANCE)GetWindowLongPtr(hwnd, GWL_HINSTANCE)==hinst &&
		IsWindowVisible(hwnd))
	{
		hwndMain=hwnd;
		return FALSE;
	}
	else
		return TRUE;
}


GamepadProviderWindows::GamepadProviderWindows()
{
	
}

GamepadProviderWindows::~GamepadProviderWindows()
{
}

bool GamepadProviderWindows::Init()
{
	LogMsg("Initting windows gamepad provider");
	
	hwndMain=NULL;
	EnumWindows(EnumWindowProc, 0);

	HRESULT result = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID *) &directinput, 0);
	if (FAILED(result))
	{
		LogError("debug", "Unable to initialize direct input");
		return false;
	}


	//enumerate each device
	result = directinput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		&GamepadProviderWindows::enum_devices_callback,
		this,
		DIEDFL_ATTACHEDONLY);
	if (FAILED(result))
	{
		LogError("Unable to enumerate direct input devices");
	}

	return true; //success
}

void GamepadProviderWindows::Kill()
{
	SAFE_RELEASE(directinput);
}


BOOL GamepadProviderWindows::enum_devices_callback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	GamepadProviderWindows *self = (GamepadProviderWindows *) pvRef;

	
	GamepadWindows *pPad = new GamepadWindows;
	pPad->SetProvider(self);
	pPad->SetDeviceInstance(*lpddi);
	GetGamepadManager()->AddGamepad(pPad);

	//	CL_InputDevice device(new CL_InputDevice_DirectInput(self, lpddi));
	//	self->input_context.add_joystick(device);
	
	return TRUE;
}


void GamepadProviderWindows::Update()
{
	
}

HWND GamepadProviderWindows::GetHWND()
{
	return hwndMain;
}
