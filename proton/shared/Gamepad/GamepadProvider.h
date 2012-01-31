//  ***************************************************************
//  GamepadProvider - Creation date: 01/27/2012
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2012 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef GamepadProvider_h__
#define GamepadProvider_h__

class GamepadProvider
{
public:
	GamepadProvider();
	virtual ~GamepadProvider();
	virtual string GetName()=0;
	virtual bool Init()=0;
	virtual void Kill()=0;
	virtual void Update()=0;

protected:
	

private:
};

#endif // GamepadProvider_h__