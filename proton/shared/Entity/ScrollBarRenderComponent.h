//  ***************************************************************
//  ScrollBarRenderComponent - Creation date: 07/08/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef ScrollBarRenderComponent_h__
#define ScrollBarRenderComponent_h__

#include "Component.h"
#include "Entity.h"
#include "Renderer/SurfaceAnim.h"

class ScrollBarRenderComponent: public EntityComponent
{
public:
	ScrollBarRenderComponent();
	virtual ~ScrollBarRenderComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

private:

	void OnUpdate(VariantList *pVList);
	void OnRender(VariantList *pVList);
	void OnTargetOverStart(VariantList *pVList);
	void OnTargetOverEnd(VariantList *pVList);
	void OnBoundsChanged(Variant *pVariant);
	void OnFileNameChanged(Variant *pDataObject);
	CL_Vec2f *m_pPos2d;
	CL_Vec2f *m_pSize2d;
	CL_Vec2f *m_pScale2d;
	float *m_pAlpha;
	CL_Rectf *m_pBoundsRect;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	CL_Vec2f *m_pProgress2d;
	SurfaceAnim *m_pSurf;
	string *m_pFileName;
	
};

#endif // ScrollBarRenderComponent_h__