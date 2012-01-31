#include "PlatformPrecomp.h"

#include "InputTextRenderComponent.h"
#include "BaseApp.h"

InputTextRenderComponent::InputTextRenderComponent()
{
	SetName("InputTextRender");
}

InputTextRenderComponent::~InputTextRenderComponent()
{
	//BigB's change
	if (GetEntityWithNativeUIFocus() == GetParent())
	{
		GetFunction("CloseKeyboard")->sig_function(NULL);
	} else
	{
		if (GetIsUsingNativeUI())
		{
			LogMsg("Keyboard is active, but proton isn't closing it because it doesn't look like this InputTextRenderComponent has focus.");
		}
	}
}

void InputTextRenderComponent::OnVisibilityChanged(Variant *pDataObject)
{
	if (pDataObject->GetUINT32() == 0)
	{
		//we're no longer visible.  If we have focus and the keyboard is up, we should really put it down
		CloseKeyboard(NULL);
	}
	
}


void InputTextRenderComponent::OnTextChanged(Variant *pDataObject)
{
	rtRectf rt;
	GetBaseApp()->GetFont(eFont(*m_pFontID))->MeasureText(&rt, *m_pText, m_pScale2d->x);
	*m_pTextSize2d = CL_Vec2f(rt.GetWidth(), rt.GetHeight());

	if (m_bEditActive)
	{
		SetLastStringInput(*m_pText);
	}

	float fontHeight = GetBaseApp()->GetFont(eFont(*m_pFontID))->GetLineHeight(m_pScale2d->x);
//	float sizeX = 0;
	float sizeY = 0;
//	if (sizeX == 0) sizeX = fontHeight*10;
	if (sizeY == 0) sizeY = fontHeight+6;

	*m_pSize2d = CL_Vec2f(m_pSize2d->x, sizeY);

}


void InputTextRenderComponent::OnScaleChanged(Variant *pDataObject)
{
	//OPTIMIZE: this is lame, recomputing everything, but whatever
	OnTextChanged(NULL);
}


void InputTextRenderComponent::OnFontChanged(Variant *pDataObject)
{
	//OPTIMIZE: this is lame, recomputing everything, but whatever
	OnTextChanged(NULL);
}


void InputTextRenderComponent::ActivateKeyboard(VariantList *pVList)
{
	if (GetIsUsingNativeUI())
	{
		if (GetEntityWithNativeUIFocus() == GetParent())
		{
			return;
		} else
		{
			SetIsUsingNativeUI(false); //prepare to accept focus here and unfocus whatever had it
			GetMessageManager()->CallComponentFunction(this, 1, "ActivateKeyboard");
		}
	}
	
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_OPEN_TEXT_BOX;
	o.m_string = *m_pText;
	SetLastStringInput(*m_pText);
	o.m_x = -1000;
	o.m_y = -1000; 
	o.m_parm1 = *m_pInputLengthMax; //length of input allowed
	o.m_fontSize = 30.0f;
	o.m_sizeX = 217;
	o.m_sizeY = 40;

	switch (*m_pInputType)
	{
	case INPUT_TYPE_NUMBERS:
		o.m_parm2 = OSMessage::PARM_KEYBOARD_TYPE_NUMBERS;
		break;

	case INPUT_TYPE_ASCII:
		o.m_parm2 = OSMessage::PARM_KEYBOARD_TYPE_ASCII;
		break;

	case INPUT_TYPE_URL:
		o.m_parm2 = OSMessage::PARM_KEYBOARD_TYPE_URL;
		break;

	case INPUT_TYPE_ASCII_FULL:
		o.m_parm2 = OSMessage::PARM_KEYBOARD_TYPE_ASCII_FULL;
		break;
	
	case INPUT_TYPE_EMAIL:
		o.m_parm2 = OSMessage::PARM_KEYBOARD_TYPE_EMAIL;
		break;
	}

	GetBaseApp()->AddOSMessage(o);
	m_bEditActive = true;
	SetIsUsingNativeUI(true);
	SetEntityWithNativeUIFocus(GetParent());

}
void InputTextRenderComponent::OnLosingNativeGUIFocus(VariantList *pVList)
{
    VariantList vList(this);
	GetFunction("CloseKeyboard")->sig_function(&vList);
}

void InputTextRenderComponent::OnEnterForeground(VariantList *pVList)
{
	if (GetEntityWithNativeUIFocus() == GetParent())
	{

		//added for android
#ifdef _DEBUG
		LogMsg("Reinitting keyboard focus");
#endif
		//re init our keyboard just in case, android needs this
		GetFunction("ActivateKeyboard")->sig_function(NULL);
	}
	
}

void InputTextRenderComponent::OnEnterBackground(VariantList *pVList)
{
	
	//added for Android
	if (GetEntityWithNativeUIFocus() == GetParent())
	{

#ifdef _DEBUG
		LogMsg("closing keyboard focus");
#endif
        VariantList vList(this);
		GetFunction("CloseKeyboard")->sig_function(&vList);
	}

}


void InputTextRenderComponent::CloseKeyboard( VariantList *pVList )
{
	
	if (GetEntityWithNativeUIFocus() == GetParent())
	{
		SetEntityWithNativeUIFocus(NULL);
	}

	if (!m_bEditActive) return;

	LogMsg("Attempting to close onscreen keyboard");
	m_bEditActive = false;
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_CLOSE_TEXT_BOX;
	GetBaseApp()->AddOSMessage(o);
}

void InputTextRenderComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);

	//shared with the rest of the entity
	m_bEditActive = false;
	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	m_pSize2d = &GetParent()->GetVar("size2d")->GetVector2();
	m_pScale2d = &GetParent()->GetShared()->GetVarWithDefault("scale2d", Variant(1.0f, 1.0f))->GetVector2();
	m_pAlignment = &GetParent()->GetVar("alignment")->GetUINT32();
	m_pColor = &GetParent()->GetShared()->GetVarWithDefault("color", Variant(MAKE_RGBA(255,255,255,255)))->GetUINT32();
	m_pColorMod = &GetParent()->GetShared()->GetVarWithDefault("colorMod", Variant(MAKE_RGBA(255,255,255,255)))->GetUINT32();
	m_pAlpha = &GetParent()->GetShared()->GetVarWithDefault("alpha", Variant(1.0f))->GetFloat();
	GetParent()->GetFunction("OnTouchEnd")->sig_function.connect(1, boost::bind(&InputTextRenderComponent::OnTouchEnd, this, _1));
	GetParent()->GetFunction("OnInput")->sig_function.connect(1, boost::bind(&InputTextRenderComponent::OnInput, this, _1)); //used for keyboard keys on Win
	GetParent()->GetFunction("OnLosingNativeGUIFocus")->sig_function.connect(1, boost::bind(&InputTextRenderComponent::OnLosingNativeGUIFocus, this, _1)); 
	GetFunction("ActivateKeyboard")->sig_function.connect(1, boost::bind(&InputTextRenderComponent::ActivateKeyboard, this, _1)); 
	GetFunction("CloseKeyboard")->sig_function.connect(1, boost::bind(&InputTextRenderComponent::CloseKeyboard, this, _1)); 
	m_pVisible = &GetParent()->GetVarWithDefault("visible", uint32(1))->GetUINT32();
	m_pGetFocusOnEnter = &GetVarWithDefault("getFocusOnEnter", uint32(0))->GetUINT32();
	GetParent()->GetVar("visible")->GetSigOnChanged()->connect(1, boost::bind(&InputTextRenderComponent::OnVisibilityChanged, this, _1));

	//our own stuff
	m_pDisabled = &GetVarWithDefault("disabled", uint32(0))->GetUINT32();
	m_pStyle = &GetVarWithDefault("style", Variant(uint32(STYLE_NORMAL)))->GetUINT32();
	m_pTextSize2d = &GetVar("textSize2d")->GetVector2();
	m_pTextOffsetPos2d = &GetVarWithDefault("textOffsetPos2d", Variant(3,3))->GetVector2();
	m_pCursorColor = &GetVarWithDefault("cursorColor", Variant(MAKE_RGBA(209,181,137,255)))->GetUINT32();
	m_pInputLengthMax = &GetVarWithDefault("inputLengthMax", Variant(uint32(10)))->GetUINT32();
	m_pBorderColor = &GetVarWithDefault("borderColor", Variant(MAKE_RGBA(255,255,255,255)))->GetUINT32();
	m_pInputType = &GetVarWithDefault("inputType", Variant(uint32(INPUT_TYPE_ASCII)))->GetUINT32();
	m_pFiltering = &GetVarWithDefault("filtering", Variant(uint32(FILTERING_STRICT)))->GetUINT32();

	m_pText = &GetVar("text")->GetString(); //local to us
	GetVar("text")->GetSigOnChanged()->connect(1, boost::bind(&InputTextRenderComponent::OnTextChanged, this, _1));

	m_pPlaceHolderText = &GetVar("placeHolderText")->GetString(); //local to us

	m_pFontID = &GetVarWithDefault("font", uint32(FONT_SMALL))->GetUINT32();
	GetVar("font")->GetSigOnChanged()->connect(1, boost::bind(&InputTextRenderComponent::OnFontChanged, this, _1));
	GetParent()->GetVar("scale2d")->GetSigOnChanged()->connect(1, boost::bind(&InputTextRenderComponent::OnScaleChanged, this, _1));

	//register ourselves to render if the parent does
	GetParent()->GetFunction("OnRender")->sig_function.connect(1, boost::bind(&InputTextRenderComponent::OnRender, this, _1));
	GetParent()->GetFunction("OnUpdate")->sig_function.connect(1, boost::bind(&InputTextRenderComponent::OnUpdate, this, _1));

	//on android we might need to reinit the soft keyboard
	
	GetBaseApp()->m_sig_enterforeground.connect(1, boost::bind(&InputTextRenderComponent::OnEnterForeground, this, _1));
	GetBaseApp()->m_sig_enterbackground.connect(1, boost::bind(&InputTextRenderComponent::OnEnterBackground, this, _1));

}


void InputTextRenderComponent::OnTouchEnd(VariantList *pVList)
{
	if (*m_pVisible == 0) return;

	if (*m_pDisabled == false)
	{
		ActivateKeyboard(NULL);
	}
}

void InputTextRenderComponent::OnRemove()
{
	/*
	if (GetEntityWithNativeUIFocus() == GetParent())
	{
		SetEntityWithNativeUIFocus(NULL);
	}
	//Don't do this, otherwise we won't kill the keyboard properly in the deconstructor
	*/

	EntityComponent::OnRemove();
}

void InputTextRenderComponent::OnUpdate(VariantList *pVList)
{

	if (m_bEditActive)
	{
		string curString = FilterToValidAscii(GetLastStringInput(), *m_pFiltering == FILTERING_STRICT);
				
		if (*m_pText != curString)
		{
			if (curString.size() > m_pText->size())
			{
				//well, a character was added.  send it in case anybody cars
                VariantList vList( this, uint32(curString[curString.size()-1]));
				GetFunction("OnChar")->sig_function(&vList);
			} else
			{
				if (curString.size() < m_pText->size())
				{
                    VariantList vList( this, uint32(8) );
                    
					GetFunction("OnChar")->sig_function(&vList); //send a delete char
				}
			}
		
			GetVar("text")->Set(curString);
		}

		if (!GetIsUsingNativeUI())
		{
			//the keyboard is closed, we're done
			m_bEditActive = false;
			GetFunction("CloseKeyboard")->sig_function(NULL);
		}
	}
}

void InputTextRenderComponent::OnRender(VariantList *pVList)
{
	if (*m_pAlpha <= 0) return;

	if (*m_pVisible == 0) return;

	CL_Vec2f vFinalPos = pVList->m_variant[0].GetVector2()+*m_pPos2d;
	//vFinalPos -= GetAlignmentOffset(*m_pSize2d, eAlignment(*m_pAlignment));

	if (vFinalPos.y < -m_pSize2d->y) return;
	if (vFinalPos.y > GetOrthoRenderSizeYf()) return;

	float alpha;

	if (*m_pDisabled)
	{
		alpha = rt_min(*m_pAlpha, 0.5f);
	} else
	{
		alpha = *m_pAlpha;
	}
	
	uint32 color = ColorCombine(*m_pColor, *m_pColorMod, alpha);

	string *pTextToDraw = m_pText;

	if (!m_bEditActive && !m_pPlaceHolderText->empty())
	{
		pTextToDraw = m_pPlaceHolderText;
	}
	switch(*m_pStyle)
	{
	case STYLE_NORMAL:
		
		
		GetBaseApp()->GetFont(eFont(*m_pFontID))->DrawScaled(vFinalPos.x+m_pTextOffsetPos2d->x* m_pScale2d->x, vFinalPos.y+m_pTextOffsetPos2d->y* m_pScale2d->y, *pTextToDraw, m_pScale2d->x, color);
		break;
	}

	uint32 borderCol = ColorCombine(*m_pBorderColor, MAKE_RGBA(255,255,255,255), alpha);
	if (GET_ALPHA(borderCol) > 0)
	{
		DrawRect(vFinalPos, *m_pSize2d, borderCol);
	}

	if (m_bEditActive)
	{
		//draw the blinking cursor too
		float height = GetBaseApp()->GetFont(eFont(*m_pFontID))->GetLineHeight(m_pScale2d->x);
				
		CL_Vec2f vCursorPos(vFinalPos+*m_pTextOffsetPos2d);
		vCursorPos.x += m_pTextSize2d->x;
		
		CL_Rectf r = CL_Rectf(vCursorPos.x, vCursorPos.y, vCursorPos.x+2, vCursorPos.y+height); 
		float blinkAlpha = ((SinPulseByMS(500)+1)/2);
		uint32 color = ColorCombine(*m_pCursorColor, *m_pColorMod, *m_pAlpha*blinkAlpha);
		
		if (GET_ALPHA(color) > 0)
		DrawFilledRect(r, color);
	}
}

void InputTextRenderComponent::OnInput( VariantList *pVList )
{
	if (*m_pVisible == 0) return;

	eMessageType messageType = eMessageType( int(pVList->Get(0).GetFloat()));

	if (!m_bEditActive)
	{
		//well, there is one exception to a keypress we'll process, if nobody else focus, and it's an enter, and we're
		//instructed to give ourselves focus if enter is hit, then ok.
		if (messageType == MESSAGE_TYPE_GUI_CHAR)
		{
			char c = (char)pVList->Get(2).GetUINT32();
			
			if (c == 13)
			{
				if ( *m_pGetFocusOnEnter && GetEntityWithNativeUIFocus() == NULL)
				{
					//they hit enter, give ourselves focus!
					GetFunction("ActivateKeyboard")->sig_function(NULL);
				}
			}
		}
		
		return; //don't process text if we're not active
	}

	//0 = message type, 1 = parent coordinate offset, 2 = char, 3 reserved for filtering control messages
	
	switch (messageType)
	{
	case MESSAGE_TYPE_GUI_PASTE:
		{
			string paste = pVList->Get(2).GetString();
			string input = *m_pText;

			input += paste;

			//but is it too long for our input box?
			TruncateString(input, *m_pInputLengthMax);
			input = FilterToValidAscii(input, *m_pFiltering == FILTERING_STRICT);
			if (m_bEditActive)
			{
				SetLastStringInput( input);
			} else
			{
				GetVar("text")->Set(input); //so everybody receives notifications that it has changed
			}
		}
		break;
	
	case MESSAGE_TYPE_GUI_CHAR:
#ifdef _DEBUG		
		LogMsg("Got key %c", (char)pVList->Get(2).GetUINT32());
#endif

		char c = (char)pVList->Get(2).GetUINT32();
		
		string input = GetLastStringInput();

		if (c == 13)
		{
			//enter
            VariantList vList(this);
			GetFunction("CloseKeyboard")->sig_function(&vList);
		} else
		if (c == 8)
		{
			//backspace
			if (input.length() > 0)
			{
				input.erase(input.length()-1, 1);
			}
		} else
		{
			if (input.size() <= *m_pInputLengthMax)
			{
				input += c;
			} 
		}

		SetLastStringInput( FilterToValidAscii(input, *m_pFiltering == FILTERING_STRICT));
		break;
	}	

}
