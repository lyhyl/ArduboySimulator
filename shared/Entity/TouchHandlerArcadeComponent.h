//  ***************************************************************
//  TouchHandlerArcadeComponent - Creation date: 10/24/2012
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2012 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef TouchHandlerArcadeComponent_h__
#define TouchHandlerArcadeComponent_h__

#include "Component.h"
#include "Entity.h"

//Like a normal touch HandlerArcade, but with better multi-touch support, and marks its buttons as "handled", better for
//say, arrow buttons for an arcade button 

class TouchHandlerArcadeComponent: public EntityComponent
{
public:
	TouchHandlerArcadeComponent();
	virtual ~TouchHandlerArcadeComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

	void OnInput(VariantList *pVList);

private:

	void HandleClickStart(CL_Vec2f &pt, uint32 fingerID);
	void HandleClickMove(CL_Vec2f &pt, uint32 fingerID);
	void HandleClickEnd(CL_Vec2f &pt, uint32 fingerID);
	
	Variant *m_pTouchOver;
	CL_Vec2f *m_pSize2d;
	CL_Vec2f *m_pPos2d;
	CL_Rectf *m_pTouchPadding;
	uint32 *m_pAlignment;
	uint32 *m_pIgnoreTouchesOutsideRect; //if this is set to 1, we will ignore "Touch end" messages that don't happen in our rect+padding

	void UpdateTouchArea(Variant *v);
	CL_Rectf m_touchArea;
	int m_activeFinger;
	

};

#endif // TouchHandlerArcadeComponent_h__
