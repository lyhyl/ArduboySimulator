//  ***************************************************************
//  Gamepad_iCade - Creation date: 07/1/2012
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2012 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef Gamepad_iCade_h__
#define Gamepad_iCade_h__

#include "Gamepad.h"
#include "GamepadProvider_iCade.h"

class ArcadeKeyboardControl
{
public:
	ArcadeKeyboardControl()
	{
		m_bPushed = false;
	}


	void Setup(Gamepad_iCade *pPad, char keyDown, char keyUp, int buttonID, eVirtualKeys vKeyToSend);
	char m_keyDown, m_keyUp;
	eVirtualKeys m_keyToSend;
	bool m_bPushed;

};

enum eArcadeKeyboardControls
{
	//I've set these up to match the ICG07 rather than the old
	//one.. so the button/trigger mappings are really just the
	//"Other four buttons" on the old icade

	KEY_ARCADE_BUTTON_LEFT,
	KEY_ARCADE_BUTTON_UP,
	KEY_ARCADE_BUTTON_DOWN,
	KEY_ARCADE_BUTTON_RIGHT,

	KEY_ARCADE_BUTTON_L,
	KEY_ARCADE_BUTTON_R,
	KEY_ARCADE_TRIGGER_L,
	KEY_ARCADE_TRIGGER_R,
	KEY_ARCADE_BUTTON_COUNT,

	//well, these aren't buttons, but we'll keep track this way
	KEY_ARCADE_DIR_LEFT,
	KEY_ARCADE_DIR_RIGHT,
	KEY_ARCADE_DIR_UP,
	KEY_ARCADE_DIR_DOWN,
	KEY_ARCADE_TOTAL_COUNT

};
class Gamepad_iCade: public Gamepad
{
public:
	Gamepad_iCade();
	virtual ~Gamepad_iCade();
	virtual bool Init();
	virtual void Kill();
	virtual void Update();

protected:

	void OnRawKeyboardInput( VariantList *pVList );


private:


	ArcadeKeyboardControl m_keys[KEY_ARCADE_TOTAL_COUNT];
};

#endif // Gamepad_iCade_h__
