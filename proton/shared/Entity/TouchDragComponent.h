//  ***************************************************************
//  TouchDragComponent - Creation date: 02/04/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2011 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

//makes a strip or rectangle and returns an xy vector from 0 to 1 on each axis
//NOTE: Requires a TouchHandlerComponent to work

#ifndef TouchDragComponent_h__
#define TouchDragComponent_h__

#include "Component.h"
#include "Entity.h"
#include "Renderer/Surface.h"

class TouchDragComponent: public EntityComponent
{
public:
	TouchDragComponent();
	virtual ~TouchDragComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

	enum eVisualStyle
	{
		STYLE_NONE,
	};

private:

	void SetPosition(CL_Vec2f vInputPos);
	void OnInput( VariantList *pVList );
	uint32 *m_pDisabled;
	uint32 * m_pVisualStyle;
	CL_Vec2f *m_pPos2d;
	CL_Vec2f *m_pSize2d;
	CL_Vec2f *m_pMult; //multiplied against the final result, so you can make it go backwards or whatever
	uint32 *m_pSwapXAndY, *m_pReverseX, *m_pReverseY;
	CL_Vec2f m_lastPos;
	int m_lastFingerID;
	FunctionObject *m_pOnTouchDragUpdate;
};

#endif // TouchDragComponent_h__