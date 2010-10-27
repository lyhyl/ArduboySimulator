//  ***************************************************************
//  OverlayRenderComponent - Creation date: 04/11/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef OverlayRenderComponent_h__
#define OverlayRenderComponent_h__

#include "Component.h"
#include "Entity.h"
#include "Renderer/SurfaceAnim.h"

class OverlayRenderComponent: public EntityComponent
{
public:
	OverlayRenderComponent();
	virtual ~OverlayRenderComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();
	
	//normally you would never access this component directly through class functions but for this specific thing I needed it as it bypassed
	//the texture caching system completely

	void SetSurface(SurfaceAnim *pSurf, bool bDeleteSurface);

private:

	void OnRender(VariantList *pVList);
	void OnFileNameChanged(Variant *pDataObject);
	void OnScaleChanged(Variant *pDataObject);
	void SetupAnim(VariantList *pVList);

	CL_Vec2f *m_pPos2d;
	CL_Vec2f *m_pSize2d;
	CL_Vec2f *m_pScale2d;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	float *m_pAlpha;
	SurfaceAnim *m_pTex;
	uint32 *m_pAlignment;
	string *m_pFileName;
	uint32 *m_pFrameX, *m_pFrameY;
	float *m_pRotation; //in degrees
	bool m_bDeleteSurface;
};

#endif // OverlayRenderComponent_h__