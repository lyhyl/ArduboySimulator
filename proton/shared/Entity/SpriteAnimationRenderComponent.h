//  ***************************************************************
//  SpriteAnimationRenderComponent - Creation date: 04/04/2012
//  -------------------------------------------------------------
//
//  ***************************************************************
//  Programmer(s):  Aki Koskinen
//  ***************************************************************

#ifndef SpriteAnimationRenderComponent_h__
#define SpriteAnimationRenderComponent_h__

#include "Component.h"

class SpriteSheetSurface;
class SpriteAnimation;
class SpriteAnimationSet;

class SpriteAnimationRenderComponent: public EntityComponent
{
public:
	SpriteAnimationRenderComponent();
	virtual ~SpriteAnimationRenderComponent();

	virtual void OnAdd(Entity *pEnt);
	
private:
	void UpdateSizeVars();
	
	void OnRender(VariantList *pVList);
	void OnFileNameChanged(Variant *pFileName);
	void OnScaleChanged(Variant *pDataObject);

	void OnAnimationNameChanged(Variant *pAnimationName);

	CL_Vec2f *m_pPos2d;
	CL_Vec2f *m_pSize2d;
	CL_Vec2f *m_pScale2d;
	CL_Vec2f *m_pFrameSize;
	float *m_pRotation;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	float *m_pAlpha;
	SpriteAnimationSet *m_pSpriteAnimationSet;
	SpriteSheetSurface *m_pSpriteSheet;
	string *m_pFileName;
	string *m_pAnimationName;
	float *m_pPhase;
	uint32 *m_pVisible;
	uint32 *m_pFlipX, *m_pFlipY;

	const SpriteAnimation *m_pCurrentAnimation;
	CL_Rectf m_pCurrentAnimBB;
};

#endif // SpriteAnimationRenderComponent_h__
