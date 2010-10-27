//  ***************************************************************
//  Button2DComponent - Creation date: 04/13/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef Button2DComponent_h__
#define Button2DComponent_h__

#include "Component.h"
#include "Entity.h"
#include "Renderer/Surface.h"

class Button2DComponent: public EntityComponent
{
public:
	Button2DComponent();
	virtual ~Button2DComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

	enum eVisualStyle
	{
		STYLE_NONE,
		STYLE_FADE_ALPHA_ON_HOVER,
		STYLE_INVISIBLE_UNTIL_CLICKED //like above, but invisible unless clicked/hovered
	};

	enum eButtonStyle
	{
		BUTTON_STYLE_CLICK_ON_TOUCH_RELEASE,
		BUTTON_STYLE_CLICK_ON_TOUCH,
		BUTTON_STYLE_CLICK_ON_TOUCH_IGNORE_DRAGGING

	};

private:

	void OnOverStart(VariantList *pVList);
	void OnOverEnd(VariantList *pVList);
	void OnTouchStart(VariantList *pVList);
	void OnTouchEnd(VariantList *pVList);
	void UpdateButtonVisuals(Variant *pVariant);
	void PerformClick(VariantList *pVList);
	void OnVisualStyleChanged(Variant *pVariant);

	float m_alphaSave;
	string *m_pOnClickAudioFile;
	uint32 *m_pDisabled;
	unsigned int m_repeatTimer; //so we don't let them hit the button too fast
	uint32 * m_pRepeatDelayMS;
	uint32 * m_pVisualStyle;
	uint32 * m_pButtonStyle;
	string *m_pFileName;
	string *m_pOverFileName;
	uint32 *m_pTouchOver;
	float *m_pAlpha;
};


//All my buttons in blip arcade are 'click on touch' but mind wall's buttons are click on release, calling this
//lets me set the global default so I don't have to set it everywhere
void SetDefaultButtonStyle(Button2DComponent::eButtonStyle style);
void SetDefaultAudioClickSound(string fileName);

#endif // Button2DComponent_h__