//  ***************************************************************
//  SliderComponent - Creation date: 8/25/2010
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2010 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef SliderComponent_h__
#define SliderComponent_h__

#include "Entity/Component.h"
#include "Entity/Entity.h"
//#include "Renderer/SurfaceAnim.h"

class SliderComponent: public EntityComponent
{
public:
	SliderComponent();
	virtual ~SliderComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

private:

	void OnRender(VariantList *pVList);
	void OnUpdate(VariantList *pVList);
	void OnProgressChanged(Variant *pDataObject);
	void OnSliderButtonChanged(Variant *pDataObject);
	void SetSliderPosition();
	void OnTouchStart(VariantList *pVList);
	void OnTouchEnd(VariantList *pVList);
	void OnInput( VariantList *pVList );
	void UpdatePositionByTouch(CL_Vec2f pt);
	void OnOverEnd(VariantList *pVList);
	CL_Vec2f *m_pPos2d;

	
	CL_Vec2f *m_pSize2d;
	/*CL_Vec2f *m_pScale2d;
	uint32 *m_pAlignment;
	float *m_pRotation; //in degrees
	*/
	
	float *m_pAlpha;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	uint32 *m_pBorderColor;
	float *m_pProgress;
	Entity * m_pSliderButton;
	CL_Vec2f m_pClickStartPos;
	bool	m_sliderButtonSelected;

};

#endif // SliderComponent_h__