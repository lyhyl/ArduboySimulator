#include "PlatformPrecomp.h"

#include "Button2DComponent.h"
#include "BaseApp.h"
#include "Entity/EntityUtils.h"

string g_defaultButtonClickSound ="audio/click.wav";

Button2DComponent::eButtonStyle g_defaultButtonStyle = Button2DComponent::BUTTON_STYLE_CLICK_ON_TOUCH;

void SetDefaultButtonStyle(Button2DComponent::eButtonStyle style)
{
	g_defaultButtonStyle = style;
}

void SetDefaultAudioClickSound(string fileName)
{
	g_defaultButtonClickSound = fileName;
}
Button2DComponent::Button2DComponent()
{
	SetName("Button2D");
}

Button2DComponent::~Button2DComponent()
{
}

void Button2DComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);


	//when area is clicked, OnButtonSelected is called on the parent entity

	m_pOnClickAudioFile = &GetVarWithDefault("onClickAudioFile", Variant(g_defaultButtonClickSound))->GetString();
	m_pDisabled = &GetVarWithDefault("disabled", uint32(0))->GetUINT32();
	m_pRepeatDelayMS = &GetVarWithDefault("repeatDelayMS", uint32(250))->GetUINT32();
	m_pVisualStyle = &GetVarWithDefault("visualStyle", uint32(STYLE_FADE_ALPHA_ON_HOVER))->GetUINT32();
	m_pButtonStyle = &GetVarWithDefault("buttonStyle", uint32(g_defaultButtonStyle))->GetUINT32();
	m_pFileName = &GetVar("fileName")->GetString();
	m_pOverFileName = &GetVar("overFileName")->GetString();
	m_pTouchOver = &GetParent()->GetVar("touchOver")->GetUINT32();
	m_pAlpha = &GetParent()->GetShared()->GetVarWithDefault("alpha", Variant(1.0f))->GetFloat();

	m_repeatTimer = 0;

	//register to get updated every frame
	GetParent()->GetFunction("PerformClick")->sig_function.connect(1, boost::bind(&Button2DComponent::PerformClick, this, _1));

	GetParent()->GetFunction("OnOverStart")->sig_function.connect(1, boost::bind(&Button2DComponent::OnOverStart, this, _1));
	GetParent()->GetFunction("OnOverEnd")->sig_function.connect(1, boost::bind(&Button2DComponent::OnOverEnd, this, _1));
	GetParent()->GetFunction("OnTouchEnd")->sig_function.connect(1, boost::bind(&Button2DComponent::OnTouchEnd, this, _1));
	GetParent()->GetFunction("OnTouchStart")->sig_function.connect(1, boost::bind(&Button2DComponent::OnTouchStart, this, _1));
	GetVar("fileName")->GetSigOnChanged()->connect(boost::bind(&Button2DComponent::UpdateButtonVisuals, this, _1));
	GetVar("overFileName")->GetSigOnChanged()->connect(boost::bind(&Button2DComponent::UpdateButtonVisuals, this, _1));
	GetVar("visualStyle")->GetSigOnChanged()->connect(boost::bind(&Button2DComponent::OnVisualStyleChanged, this, _1));
}

void Button2DComponent::OnRemove()
{
	EntityComponent::OnRemove();
}

void Button2DComponent::OnVisualStyleChanged(Variant *pVariant)
{
	if (*m_pVisualStyle == STYLE_INVISIBLE_UNTIL_CLICKED)
	{
		m_alphaSave = *m_pAlpha;
		*m_pAlpha = 0;
	}
}

void Button2DComponent::UpdateButtonVisuals(Variant *pVariant)
{
	if (m_pFileName->empty() && m_pOverFileName->empty()) return;
	
	EntityComponent *pComp = GetParent()->GetComponentByName("OverlayRender");
	
	if (*m_pTouchOver == 0)
	{
		pComp->GetVar("fileName")->Set(*m_pFileName);
	} else
	{
		pComp->GetVar("fileName")->Set(*m_pOverFileName);
	}
}

void Button2DComponent::OnOverStart(VariantList *pVList)
{
	//LogMsg("Now over button");

	switch (*m_pVisualStyle)
	{
	case STYLE_FADE_ALPHA_ON_HOVER:
		m_alphaSave = *m_pAlpha;
		break;
	}
	
	UpdateButtonVisuals(NULL);

	switch (*m_pVisualStyle)
	{
	case STYLE_FADE_ALPHA_ON_HOVER:
		if (*m_pDisabled == 0 && m_repeatTimer < GetBaseApp()->GetTick())
		{	
			GetParent()->GetVar("alpha")->Set(m_alphaSave*0.5f);
		}
		break;
	}
}

void Button2DComponent::OnOverEnd(VariantList *pVList)
{
	//LogMsg("Now off button");
	
	UpdateButtonVisuals(NULL);

	switch (*m_pVisualStyle)
	{
	case STYLE_FADE_ALPHA_ON_HOVER:
		GetParent()->GetVar("alpha")->Set(m_alphaSave);
		break;
	}
}

void Button2DComponent::OnTouchStart(VariantList *pVList)
{
	TouchTrackInfo *pTouch = GetBaseApp()->GetTouch(pVList->Get(2).GetUINT32());
	if (pTouch->WasHandled()) return;
	pTouch->SetWasHandled(true);
	
	if (*m_pButtonStyle == BUTTON_STYLE_CLICK_ON_TOUCH || *m_pButtonStyle == BUTTON_STYLE_CLICK_ON_TOUCH_IGNORE_DRAGGING)
	{
		if (*m_pVisualStyle == STYLE_FADE_ALPHA_ON_HOVER)
		{
			m_alphaSave = *m_pAlpha;
		}

		PerformClick(pVList);
		//tell the original touch handler that we're not over it anymore, because we may want to load a menu now, then return, and the unclick will be missed.
		return;
	}

	//LogMsg("Clicked button");
}

void Button2DComponent::PerformClick(VariantList *pVList)
{
	assert(pVList && "Need to send in coords so it knows where they clicked on the image");

	//why are we setting the parent here?  Well, I must have had a good reason.. someone must want this as output.
	//Don't change to be safe.  -Seth
	pVList->m_variant[1].Set(GetParent());

	switch (*m_pVisualStyle)
	{
		case STYLE_FADE_ALPHA_ON_HOVER:
			GetParent()->GetVar("alpha")->Set(m_alphaSave);
			break;

		case STYLE_INVISIBLE_UNTIL_CLICKED:
			GetParent()->GetVar("alpha")->Set(m_alphaSave);
			//schedule the alpha to change back after a while
			GetMessageManager()->SetEntityVariable(GetParent(), *m_pRepeatDelayMS, "alpha", 0.0f);
			break;
	}

	if (*m_pDisabled == 0 && m_repeatTimer < GetBaseApp()->GetTick())
	{
		m_repeatTimer = GetBaseApp()->GetTick() + (*m_pRepeatDelayMS);
		if (!m_pOnClickAudioFile->empty())
		{
			GetAudioManager()->Play(*m_pOnClickAudioFile);
		}
		GetMessageManager()->CallEntityFunction(GetParent(), 1, "OnButtonSelected", pVList);  //sends a vec2 with position and this entity
		if (*m_pButtonStyle == BUTTON_STYLE_CLICK_ON_TOUCH && *m_pRepeatDelayMS != 0 )
		{
			SendFakeInputMessageToEntity(GetParent(), MESSAGE_TYPE_GUI_CLICK_END, pVList->m_variant[0].GetVector2()); //otherwise the menu may never get the touch release message
			return;
		}
		
	} else
	{
#ifdef _DEBUG
		LogMsg("Ignoring click to %s, button is disabled",GetParent()->GetName().c_str());
#endif
	}
}
void Button2DComponent::OnTouchEnd(VariantList *pVList)
{
	//LogMsg("Released button while highlighted");
	if (*m_pButtonStyle != BUTTON_STYLE_CLICK_ON_TOUCH && *m_pButtonStyle != BUTTON_STYLE_CLICK_ON_TOUCH_IGNORE_DRAGGING)
	{
		PerformClick(pVList);
		return;
	}
}
