//  ***************************************************************
//  TextBoxRenderComponent - Creation date: 07/02/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef TextBoxRenderComponent_h__
#define TextBoxRenderComponent_h__

#include "Component.h"
#include "Entity.h"
#include "Renderer/Surface.h"

class TextBoxRenderComponent: public EntityComponent
{
public:
	TextBoxRenderComponent();
	virtual ~TextBoxRenderComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();
	
	enum eVisualStyle
	{
		STYLE_NORMAL,
		STYLE_TYPE
	};

private:

	//our stuff
	void OnRender(VariantList *pVList);
	void OnTextChanged(Variant *pDataObject);
	void OnScaleChanged(Variant *pDataObject);
	void OnFontChanged(Variant *pDataObject);
	void DrawTextNormal(CL_Vec2f vPos);
	void DrawTextType(CL_Vec2f vPos);

	CL_Vec2f *m_pPos2d;
	CL_Vec2f *m_pSize2d;
	CL_Vec2f *m_pEnclosedSize2d;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	float *m_pAlpha;
	string *m_pText;
	CL_Vec2f *m_pScale2d;
	uint32 *m_pAlignment;
	uint32 *m_pFontID;
	uint32 *m_pStyle;
	float *m_pEffectPower;
	deque<string> m_lines;
	float *m_pFontScale; //so we can set it independently of our main size, although that will be applied to the font too
	uint32 m_typeTimer;
	uint32 *m_pTimingSystem;
	uint32 m_lastLineRendered;
	uint32 m_lastCharRendered;

};

#endif // TextBoxRenderComponent_h__