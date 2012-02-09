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

/*
Add this component and if it is clicked/touched (it knows the site because it checks pos2d, size2d, scale2d vars)
it causes a function called OnButtonSelected (on the parent entity) with two parms - index 0 is the vec2 position,
index 1 is the parent entity
*/

#include "Component.h"
#include "Entity.h"
#include "Renderer/Surface.h"

/**
 * A component that adds button behaviour to an \c Entity.
 *
 * The name of the component is initially set to "Button2D".
 *
 * This component works nicely together with \c TouchHandlerComponent which
 * calls appropriate functions on the parent \c Entity. This component listens
 * calls to those functions and behaves like a button.
 *
 * The following named variants are used inside the component itself:
 * - <b>"onClickAudioFile" (string):</b> the name of the audio file that is played
 *   when the button is clicked. By default is whatever is the current global
 *   default click sound. See SetDefaultAudioClickSound().
 * - <b>"disabled" (uint32):</b> used to disable the button. 0 (the default) means
 *   the button is enabled, any other values disables it.
 * - <b>"buttonStyle" (uint32):</b> determines the button's behaviour style. This
 *   needs to be one of the values of \link Button2DComponent::eButtonStyle \c eButtonStyle \endlink.

 * When the button is clicked the "OnButtonSelected" function of the parent \c Entity gets called.
 * This function gets a \c VariantList that contains three members:
 * - 0: the touch point of type \c CL_Vec2f
 * - 1: a pointer to the \c Entity that was touched
 * - 2: the id of the finger, type is uint32
 */
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

	/**
	 * Button behaviour styles.
	 */
	enum eButtonStyle
	{
		/// The button is clicked after it has been pressed and subsequently released.
		BUTTON_STYLE_CLICK_ON_TOUCH_RELEASE,
		/// The button is clicked immeditely when it's pressed.
		BUTTON_STYLE_CLICK_ON_TOUCH,
		/// No idea what this is. Seems to be exactly what BUTTON_STYLE_CLICK_ON_TOUCH is too.
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
/**
 * Sets a global default button style.
 *
 * This setting has effect only for buttons that are created after this call.
 * Initially this is set to \link Button2DComponent::BUTTON_STYLE_CLICK_ON_TOUCH BUTTON_STYLE_CLICK_ON_TOUCH \endlink.
 */
void SetDefaultButtonStyle(Button2DComponent::eButtonStyle style);
/**
 * Sets a global default audio file name for the click sound.
 *
 * This setting has effect only for buttons that are created after this call.
 * Initially this is set to "audio/click.wav".
 */
void SetDefaultAudioClickSound(string fileName);

#endif // Button2DComponent_h__
