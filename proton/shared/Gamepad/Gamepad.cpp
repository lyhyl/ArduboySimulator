#include "PlatformPrecomp.h"
#include "Gamepad.h"
#include "Entity/ArcadeInputComponent.h"


#ifdef _DEBUG
	//#define SHOW_GAMEPAD_DEBUG_STUFF
#endif


Gamepad::Gamepad()
{
	m_pPadProvider = NULL;
	m_name = "Unknown";
	m_buttonsUsedCount = 0;
	m_axisUsedCount = 0;
	m_bIsUsed = false;

	SetRightStickAxis(7, 2);

	for (int i=0; i < GAMEPAD_MAX_BUTTONS; i++)
	{
		m_buttons[i].m_virtualKey = (eVirtualKeys)(VIRTUAL_DPAD_BUTTON_LEFT+i); //defaults
	}
}

Gamepad::~Gamepad()
{
	LogMsg("Removing gamepad %s", m_name.c_str());
}

void Gamepad::SetAxis( int axis, float val )
{
	m_axis[axis].m_axis = val;

#ifdef SHOW_GAMEPAD_DEBUG_STUFF
	//LogMsg("Got axis %d: %.2f", axis, val);
#endif

}

void Gamepad::OnButton( bool bDown, int buttonID )
{
	m_buttons[buttonID].OnPress(bDown);
	
#ifdef SHOW_GAMEPAD_DEBUG_STUFF
	LogMsg("Button %d is %s", buttonID, bDown ? "Down" : "Up");
#endif
	
	VariantList v;
	v.Get(0).Set(uint32(m_buttons[buttonID].m_virtualKey));
	v.Get(2).Set(uint32(m_id));
	if (bDown)
	{
		v.Get(1).Set(uint32(VIRTUAL_KEY_PRESS)); 
	} else
	{
		v.Get(1).Set(uint32(VIRTUAL_KEY_RELEASE)); 
	}

	m_sig_gamepad_buttons(&v);
}

void Gamepad::OnHat( int index, float val )
{
	//ignoring this for now.. add as extra buttons later?
#ifdef SHOW_GAMEPAD_DEBUG_STUFF
	LogMsg("Hat index %d is %.2f", index, val);
#endif
}


void Gamepad::SetRightStickAxis( int axisX, int axisY )
{
	m_rightStickAxisX = axisX;
	m_rightStickAxisY = axisY;

}

void Gamepad::ConnectToArcadeComponent( ArcadeInputComponent *pComp, bool bSendButtonEvents, bool bSendPadEventsAsFourDirections )
{
	
	if (bSendButtonEvents)
	{
		m_sig_gamepad_buttons.connect(1, boost::bind(&ArcadeInputComponent::OnRawKeyboard, pComp, _1));
	}

	if (bSendPadEventsAsFourDirections)
	{
		m_sig_four_directions.connect(1, boost::bind(&ArcadeInputComponent::OnRawKeyboard, pComp, _1));
	}
}

GamepadButton * Gamepad::GetButton( int buttonID )
{
	assert(buttonID < GAMEPAD_MAX_BUTTONS && "No button this high");
	return &m_buttons[buttonID];
}

CL_Vec2f Gamepad::GetLeftStick()
{
	return CL_Vec2f(m_axis[0].m_axis, m_axis[1].m_axis);
}

CL_Vec2f Gamepad::GetRightStick()
{
	return CL_Vec2f(m_axis[m_rightStickAxisX].m_axis, m_axis[m_rightStickAxisY].m_axis);
}

void Gamepad::Update()
{
	if (GetLeftStick() != m_vLastSentLeftStick)
	{
		VariantList v(GetLeftStick(), (int32)m_id, (int32)0);
		m_sig_left_stick(&v);
		m_vLastSentLeftStick = GetLeftStick();
	}

	if (GetRightStick() != m_vLastSentRightStick)
	{
		VariantList v(GetRightStick(), (int32)m_id, (int32)1);
		m_sig_right_stick(&v);
		m_vLastSentRightStick = GetRightStick();
	}
}

void GamepadButton::OnPress( bool bDown )
{
	m_bDown = bDown;
	//LogMsg(")
}