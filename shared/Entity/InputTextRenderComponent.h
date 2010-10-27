//  ***************************************************************
//  InputTextRenderComponent - Creation date: 07/21/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef InputTextRenderComponent_h__
#define InputTextRenderComponent_h__

#include "Component.h"
#include "Entity.h"
#include "Renderer/Surface.h"

class InputTextRenderComponent: public EntityComponent
{
public:
	InputTextRenderComponent();
	virtual ~InputTextRenderComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

	enum eVisualStyle
	{
		STYLE_NORMAL,
	};

	enum eInputType
	{
		INPUT_TYPE_ASCII,
		INPUT_TYPE_NUMBERS,
		INPUT_TYPE_URL,
		INPUT_TYPE_ASCII_FULL, //untested
		INPUT_TYPE_EMAIL //untested
	};

	enum eInputFiltering
	{
		FILTERING_STRICT, //no spaces or weird symbols allowed
		FILTERING_LOOSE //allows everything that could be an say, a URL
	};
private:

	void OnRender(VariantList *pVList);
	void OnTextChanged(Variant *pDataObject);
	void OnScaleChanged(Variant *pDataObject);
	void OnFontChanged(Variant *pDataObject);
	void ActivateKeyboard(VariantList *pVList);
	void CloseKeyboard(VariantList *pVList);
	void OnTouchEnd(VariantList *pVList);
	void OnUpdate(VariantList *pVList);
	void OnInput( VariantList *pVList );
	void OnLosingNativeGUIFocus(VariantList *pVList);
	void OnEnterForeground(VariantList *pVList);
	void OnEnterBackground(VariantList *pVList);
	CL_Vec2f *m_pPos2d;
	CL_Vec2f *m_pTextOffsetPos2d;
	CL_Vec2f *m_pSize2d;
	CL_Vec2f *m_pTextSize2d;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	float *m_pAlpha;
	string *m_pText;
	CL_Vec2f *m_pScale2d;
	uint32 *m_pAlignment;
	uint32 *m_pFontID;
	uint32 *m_pStyle;
	uint32 *m_pCursorColor;
	bool m_bEditActive;
	uint32 *m_pInputLengthMax;
	uint32 *m_pBorderColor;
	uint32 *m_pDisabled;
	uint32 *m_pInputType;
	uint32 *m_pFiltering;

};

#endif // InputTextRenderComponent_h__