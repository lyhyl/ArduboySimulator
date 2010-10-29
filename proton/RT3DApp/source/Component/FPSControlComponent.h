//  ***************************************************************
//  FPSControlComponent - Creation date: ?/?/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef FPSControlComponent_h__
#define FPSControlComponent_h__

#include "Entity/Component.h"
#include "Entity/Entity.h"
//#include "Renderer/SurfaceAnim.h"
#include "Irrlicht/IrrlichtManager.h"

class FPSControlComponent: public EntityComponent
{
public:
	FPSControlComponent();
	virtual ~FPSControlComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

private:

	void OnRender(VariantList *pVList);
	void OnUpdate(VariantList *pVList);
	void OnStripUpdate(VariantList *pVList);
	void SendKey(irr::EKEY_CODE key, bool bIsDown);
	void OnOverStart(VariantList *pVList);
	void OnOverEnd(VariantList *pVList);
	void OnTurnOverStart(VariantList *pVList);
	void OnTurnOverEnd(VariantList *pVList);
	void OnTurnStripUpdate(VariantList *pVList);
	
	CL_Vec2f *m_pPos2d;

	/*
	CL_Vec2f *m_pSize2d;
	float *m_pScale;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	float *m_pAlpha;
	uint32 *m_pAlignment;
	float *m_pRotation; //in degrees
	*/
	Entity *m_pArrowEnt;
	Entity *m_pTurnArrowEnt;
	CL_Vec2f m_lastTouchDir;
	CL_Vec2f m_lastTurnDir;
	bool m_bTurningNow;

};

#endif // FPSControlComponent_h__