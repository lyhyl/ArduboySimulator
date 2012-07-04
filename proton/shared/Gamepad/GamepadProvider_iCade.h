//  ***************************************************************
//  GamepadProvider_iCade - Creation date: 07/1/2012
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2012 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************


/*
 Provides an interface for the iCade controller.  Basically we just rewrote the keystrokes to look like a joystick.
 */

#ifndef GamepadProvider_iCade_h__
#define GamepadProvider_iCade_h__

#include "GamepadProvider.h"

class Gamepad_iCade;

class GamepadProvider_iCade: public GamepadProvider
{
public:
	GamepadProvider_iCade();
	virtual ~GamepadProvider_iCade();

	virtual string GetName() {return "iCade";}
	virtual bool Init();
	virtual void Kill();
	virtual void Update();
 
protected:
	
private:

    Gamepad_iCade *m_pPad;
};

#endif // GamepadProvider_iCade_h__
