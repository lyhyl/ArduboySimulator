//  ***************************************************************
//  GamepadIOS_60Beat - Creation date: 01/27/2012
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2012 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

//The DirectInput code was mostly stolen from Clanlib, thanks guys!

#ifndef GamepadIOS_60Beat_h__
#define GamepadIOS_60Beat_h__

#include "Gamepad.h"
#include "GamepadProviderIOS_60Beat.h"

class GamepadIOS_60Beat: public Gamepad
{
public:
	GamepadIOS_60Beat();
	virtual ~GamepadIOS_60Beat();
	virtual bool Init();
	virtual void Kill();
	virtual void Update();

	void SetProvider(GamepadProviderIOS_60Beat *pProvider) {m_pPadProvider = pProvider;}


protected:
	

private:

	GamepadProviderIOS_60Beat *m_pPadProvider;

	

};

#endif // GamepadIOS_60Beat_h__