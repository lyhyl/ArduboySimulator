//  ***************************************************************
//  GamepadProviderIOS_60Beat - Creation date: 01/27/2012
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2012 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************


/*
 
 Provides the GamepadManager with support for the 60beat gamepad.
 
 Just adding the three files and a GetGameManager->AddProvider(new GamepadProviderIOS_60Beat); is enough to make it work,
 but you should also define RT_IOS_60BEAT_GAMEPAD_SUPPORT so the Denshion audio
 manager will also call it when audio is suspended.
 
 (This pad uses the mic jack.. I have no idea what will happen if you try it with the
 iOS fmod audio system..)
 
 
 
 */

#ifndef GamepadProviderIOS_60Beat_h__
#define GamepadProviderIOS_60Beat_h__

#include "GamepadProvider.h"


class GamepadIOS_60Beat;

class GamepadProviderIOS_60Beat: public GamepadProvider
{
public:
	GamepadProviderIOS_60Beat();
	virtual ~GamepadProviderIOS_60Beat();

	virtual string GetName() {return "IOS_60Beat";}
	virtual bool Init();
	virtual void Kill();
	virtual void Update();

	//Used by our custom GamepadIOS_60Beat class
	
    CL_Vec2f  GetLeftStickPos();
    CL_Vec2f  GetRightStickPos();
    
protected:
	

private:


    GamepadIOS_60Beat *m_pPad;
};

#endif // GamepadProviderIOS_60Beat_h__