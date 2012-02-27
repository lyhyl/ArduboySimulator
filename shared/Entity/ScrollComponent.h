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

/**
 * A component that adds scrolling behaviour to an \c Entity.
 *
 * The name of the component is initially set to "Scroll".
 *
 * This component monitors the touch events for it's parent \c Entity and
 * makes the children entities of the parent move according to the events' positions.
 * This causes a "pannable area" effect to the contents of the parent \c Entity.
 *
 * In order to get the touch events correctly there should be a \c TouchHandlerComponent
 * in the parent \c Entity.
 *
 * The following named variants are used inside the component itself:
 * - <b>"progress2d" (Vector2):</b> the current relative position of the scroll area.
 *   The x coordinate of the returned value tells the horizontal and y coordinate
 *   tells the vertical position of the scroll area. The values are between 0 and 1.
 *   0 means that the scroll area is at its most left/top and 1 means that the scroll
 *   area is at its most right/bottom. This is a read-only variant, setting it has no
 *   effect. Use the "SetProgress" function object for setting this.
 *
 * There is one function object that this component offers:
 * - <b>"SetProgress"</b>: sets the relative position of the scroll area. It requires
 *   a Vector2 as an argument (first member of the VariantList) whose coordinate values tell the
 *   relative positions in the horizontal and vertical directions. The values must be
 *   from 0 to 1. Setting the value to (0, 0) sets the scroll area to the top left
 *   position. Setting the value to (1, 1) sets the scroll area to the bottom right position.
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
	uint32 *m_pScrollStyle;
	CL_Vec2f m_vecDisplacement;
	float * m_pFriction, *m_pMaxScrollSpeed, *m_pPowerMod;
	CL_Vec2f m_vecChildPos;
	Variant * m_progressVar; //we write this out
	int m_activeFinger;
	uint32 *m_pEnforceFingerTracking;
};

#endif // ScrollComponent_h__
