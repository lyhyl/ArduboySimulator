#include "PlatformPrecomp.h"
#include "GamepadIOS_60Beat.h"
#include "GamepadProviderIOS_60BeatNative.h"


GamepadIOS_60Beat::GamepadIOS_60Beat()
{
	m_pPadProvider = NULL;
	
}

GamepadIOS_60Beat::~GamepadIOS_60Beat()
{
	Kill();
}

bool GamepadIOS_60Beat::Init()
{
    m_axisUsedCount = 4;
    SetRightStickAxis(2,3);
    m_name = "60beat";
    
    //improve button mappings
    
    m_buttons[SBEAT_BUTTON_1].m_virtualKey = VIRTUAL_DPAD_BUTTON_UP;
    m_buttons[SBEAT_BUTTON_2].m_virtualKey = VIRTUAL_DPAD_BUTTON_RIGHT;
    m_buttons[SBEAT_BUTTON_3].m_virtualKey = VIRTUAL_DPAD_BUTTON_DOWN;
    m_buttons[SBEAT_BUTTON_4].m_virtualKey = VIRTUAL_DPAD_BUTTON_LEFT;
  
    m_buttons[SBEAT_BUTTON_L1].m_virtualKey = VIRTUAL_DPAD_LBUTTON;
    m_buttons[SBEAT_BUTTON_L2].m_virtualKey = VIRTUAL_DPAD_LTRIGGER;

    m_buttons[SBEAT_BUTTON_R1].m_virtualKey = VIRTUAL_DPAD_RBUTTON;
    m_buttons[SBEAT_BUTTON_R2].m_virtualKey = VIRTUAL_DPAD_RTRIGGER;
 
    m_buttons[SBEAT_BUTTON_UP].m_virtualKey = VIRTUAL_DPAD_HAT_UP;
    m_buttons[SBEAT_BUTTON_DOWN].m_virtualKey = VIRTUAL_DPAD_HAT_DOWN;
    m_buttons[SBEAT_BUTTON_LEFT].m_virtualKey = VIRTUAL_DPAD_HAT_LEFT;
    m_buttons[SBEAT_BUTTON_RIGHT].m_virtualKey = VIRTUAL_DPAD_HAT_RIGHT;

    m_buttons[SBEAT_BUTTON_START].m_virtualKey = VIRTUAL_DPAD_SELECT;
    m_buttons[SBEAT_BUTTON_SELECT].m_virtualKey = VIRTUAL_DPAD_START;
    
	return true;
}


void GamepadIOS_60Beat::Kill()
{
	
}

void GamepadIOS_60Beat::Update()
{

	GamepadProviderIOS_60Beat *pProv = (GamepadProviderIOS_60Beat*)m_pPadProvider;

    CL_Vec2f vLeft = pProv->GetLeftStickPos();
    SetAxis(0, vLeft.x);
    SetAxis(1, vLeft.y);

    
    CL_Vec2f vRight = pProv->GetRightStickPos();
    SetAxis(2, vRight.x);
    SetAxis(3, vRight.y);

    Gamepad::Update();
}