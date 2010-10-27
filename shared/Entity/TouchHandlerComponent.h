//  ***************************************************************
//  TouchHandlerComponent - Creation date: 05/12/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef TouchHandlerComponent_h__
#define TouchHandlerComponent_h__

#include "Component.h"
#include "Entity.h"

class TouchHandlerComponent: public EntityComponent
{
public:
	TouchHandlerComponent();
	virtual ~TouchHandlerComponent();
	
	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

	void OnInput(VariantList *pVList);
	void OnUpdate(VariantList *pVList);
	
private:

	void HandleClickStart(CL_Vec2f &pt);
	void HandleClickMove(CL_Vec2f &pt);
	void HandleClickEnd(CL_Vec2f &pt);
	Variant *m_pTouchOver;
	CL_Vec2f *m_pSize2d;
	CL_Vec2f *m_pPos2d;
	CL_Rectf *m_pTouchPadding;
	uint32 *m_pAlignment;
	uint32 *m_pIgnoreTouchesOutsideRect; //if this is set to 1, we will ignore "Touch end" messages that don't happen in our rect+padding
	
};

#endif // TouchHandlerComponent_h__