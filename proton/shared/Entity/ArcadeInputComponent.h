//  ***************************************************************
//  ArcadeInputComponent - Creation date: 11/8/2010
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2010 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

//This component reads the android trackball data, and keyboard data, and converts them to direction up/down key events.

//The way it does it with the trackball is it detects a direction, then keeps the key "down" until no movement is detected after 100
//ms or so.

#ifndef ArcadeInputComponent_h__
#define ArcadeInputComponent_h__

#include "Entity/Component.h"
#include "Entity/Entity.h"
//#include "Renderer/SurfaceAnim.h"

enum eMoveButtonDir
{
	MOVE_BUTTON_DIR_LEFT = 0,
	MOVE_BUTTON_DIR_RIGHT,
	MOVE_BUTTON_DIR_UP,
	MOVE_BUTTON_DIR_DOWN,
	
	MOVE_BUTTON_DIR_COUNT

};


class MoveButtonState
{
public:

	MoveButtonState()
	{
		m_bIsDown = false;
		m_releaseTimer = 0;
	}

	void SetKeyType(eVirtualKeys type)
	{
		m_keyType = type;
	}

	void OnPress(int releaseTime);

	void Update();
	bool m_bIsDown;
	unsigned int m_releaseTimer;
	eVirtualKeys m_keyType;
};


class ArcadeInputComponent: public EntityComponent
{

	

public:
	ArcadeInputComponent();
	virtual ~ArcadeInputComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

private:

	void OnUpdate(VariantList *pVList);
	void OnTrackball(VariantList *pVList);

	CL_Vec2f *m_pPos2d;
	
	/*
	CL_Vec2f *m_pSize2d;
	CL_Vec2f *m_pScale2d;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	float *m_pAlpha;
	uint32 *m_pAlignment;
	float *m_pRotation; //in degrees
	*/


	MoveButtonState m_buttons[MOVE_BUTTON_DIR_COUNT];
};

#endif // MovementInputComponent_h__