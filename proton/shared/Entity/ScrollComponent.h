//  ***************************************************************
//  ScrollComponent - Creation date: 07/07/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef ScrollComponent_h__
#define ScrollComponent_h__

#include "Component.h"
#include "Entity.h"
#include "Renderer/SurfaceAnim.h"

/*

  Allows finger/mouse scrolling of children content.

  If you do the following, it will enforce fingerid tracking:
  
  pScrollComp->GetVar("fingerTracking")->Set(uint32(1)); 

  This will do two things:

  1. Any click marked "handled" will be ignored
  2. It will mark its own click as "handled"

  If you activate this, you probably want to move this component to the "bottom" so it gets processed AFTER its content is.

  Example of its use is in RTSimpleApp example.
*/

class ScrollComponent: public EntityComponent
{
public:
	ScrollComponent();
	virtual ~ScrollComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

	enum eScrollStyle
	{
		STYLE_MOMENTUM,
		STYLE_EXACT
	};


private:

	void OnUpdate(VariantList *pVList);
	void OnOverStart(VariantList *pVList);
	void OnOverEnd(VariantList *pVList);
	void OnOverMove(VariantList *pVList);
	void SetPosition(CL_Vec2f vDisplacement, bool bForceUpdate);
	void OnBoundsChanged(Variant *pVariant);
	void SetProgress(VariantList *pVList);
	CL_Vec2f *m_pPos2d;
	CL_Vec2f *m_pSize2d;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	float *m_pAlpha;
	float *m_pScale;
	uint32 *m_pAlignment;
	CL_Vec2f m_lastTouchPos;
	CL_Rectf *m_pBoundsRect;
	Variant *m_pvPos;
	uint32 *m_pScrollStyle;
	CL_Vec2f m_vecDisplacement;
	float * m_pFriction, *m_pMaxScrollSpeed, *m_pPowerMod;
	CL_Vec2f m_vecChildPos;
	Variant * m_progressVar; //we write this out
	int m_activeFinger;
	uint32 *m_pEnforceFingerTracking;
	bool m_bIsAMove;
};

#endif // ScrollComponent_h__