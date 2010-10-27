#include "PlatformPrecomp.h"
#include "SliderComponent.h"
#include "util/GLESUtils.h"
#include "Entity/EntityUtils.h"
#include "BaseApp.h"

SliderComponent::SliderComponent()
{
	SetName("Slider");
}

SliderComponent::~SliderComponent()
{

}

void SliderComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);
	m_pClickStartPos = CL_Vec2f(0,0);
	m_sliderButtonSelected = false;
	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	m_pSize2d = &GetParent()->GetVar("size2d")->GetVector2();

	m_pColor = &GetParent()->GetShared()->GetVarWithDefault("color", Variant(MAKE_RGBA(255,255,255,255)))->GetUINT32();
	m_pColorMod = &GetParent()->GetShared()->GetVarWithDefault("colorMod", Variant(MAKE_RGBA(255,255,255,255)))->GetUINT32();
	m_pAlpha = &GetParent()->GetShared()->GetVarWithDefault("alpha", Variant(1.0f))->GetFloat();
	m_pSliderButton = GetParent()->GetShared()->GetVarWithDefault("sliderButton", (Entity*)NULL)->GetEntity();

	/*
	m_pScale2d = &GetParent()->GetShared()->GetVarWithDefault("scale2d", Variant(1.0f, 1.0f))->GetVector2();
	m_pRotation = &GetParent()->GetVar("rotation")->GetFloat();  //in degrees

	m_pAlignment = &GetParent()->GetVar("alignment")->GetUINT32();
	*/
	m_pProgress = &GetVarWithDefault("progress", Variant(0.0f))->GetFloat();

	//register ourselves to render if the parent does
	//GetParent()->GetFunction("OnRender")->sig_function.connect(1, boost::bind(&SliderComponent::OnRender, this, _1));
	//GetParent()->GetFunction("OnUpdate")->sig_function.connect(1, boost::bind(&SliderComponent::OnUpdate, this, _1));
	GetVar("progress")->GetSigOnChanged()->connect(boost::bind(&SliderComponent::OnProgressChanged, this, _1));
	GetVar("sliderButton")->GetSigOnChanged()->connect(boost::bind(&SliderComponent::OnSliderButtonChanged, this, _1));
	GetParent()->GetFunction("OnInput")->sig_function.connect(1, boost::bind(&SliderComponent::OnInput, this, _1));

}

void SliderComponent::OnRemove()
{
	EntityComponent::OnRemove();
}

void SliderComponent::OnRender(VariantList *pVList)
{
}

void SliderComponent::OnUpdate(VariantList *pVList)
{
}

void SliderComponent::OnProgressChanged(Variant *pDataObject)
{
	SetSliderPosition();
}

void SliderComponent::SetSliderPosition()
{
	assert(m_pSliderButton && "Must set var 'sliderButton' to a valid entity");
	
	CL_Vec2f vPos = m_pSliderButton->GetVar("pos2d")->GetVector2();
	vPos.x = m_pSize2d->x* *m_pProgress;
	m_pSliderButton->GetVar("pos2d")->Set(vPos);
};

void SliderComponent::OnSliderButtonChanged(Variant *pDataObject)
{
	assert(!m_pSliderButton && "Um, don't set this twice");
	m_pSliderButton = pDataObject->GetEntity();
	SetSliderPosition();


	//map to slider button
	m_pSliderButton->GetFunction("OnTouchEnd")->sig_function.connect(1, boost::bind(&SliderComponent::OnTouchEnd, this, _1));
	m_pSliderButton->GetFunction("OnTouchStart")->sig_function.connect(1, boost::bind(&SliderComponent::OnTouchStart, this, _1));
	m_pSliderButton->GetFunction("OnOverEnd")->sig_function.connect(1, boost::bind(&SliderComponent::OnOverEnd, this, _1));

}


void SliderComponent::OnTouchStart(VariantList *pVList)
{
	m_sliderButtonSelected = true;
	//LogMsg("slider but down");
//	m_pClickStartPos = m_pSliderButton->GetVar("pos2d")->GetVector2();
}


void SliderComponent::OnTouchEnd(VariantList *pVList)
{
	//LogMsg("Released button while highlighted");
	//LogMsg("slider but up");
	//m_sliderButtonSelected = false;
	
//	m_pClickStartPos = CL_Vec2f(0,0);
}

void SliderComponent::OnOverEnd(VariantList *pVList)
{
	//LogMsg("Released button while highlighted");
	//LogMsg("slider over end");
	m_sliderButtonSelected = false;

	//	m_pClickStartPos = CL_Vec2f(0,0);
}
void SliderComponent::UpdatePositionByTouch(CL_Vec2f pt)
{
	if (!m_sliderButtonSelected) return;

	CL_Vec2f vPos = m_pSliderButton->GetVar("pos2d")->GetVector2();
	vPos.x += (pt-m_pClickStartPos).x;
	ForceRange(vPos.x, 0, m_pSize2d->x);
	m_pSliderButton->GetVar("pos2d")->Set(vPos);
	m_pClickStartPos = pt;

	GetVar("progress")->Set(vPos.x / m_pSize2d->x);
}


void SliderComponent::OnInput( VariantList *pVList )
{
	//0 = message type, 1 = parent coordinate offset
	CL_Vec2f pt = pVList->Get(1).GetVector2();
	//pt += GetAlignmentOffset(*m_pSize2d, eAlignment(*m_pAlignment));

	switch (eMessageType( int(pVList->Get(0).GetFloat())))
	{
	case MESSAGE_TYPE_GUI_CLICK_START:
		m_pClickStartPos = pt;
		break;
	case MESSAGE_TYPE_GUI_CLICK_END:
	UpdatePositionByTouch(pt);
	break;
	
	case MESSAGE_TYPE_GUI_CLICK_MOVE:
		//LogMsg("Got move message: %s", PrintVector2(pt).c_str());
		UpdatePositionByTouch(pt);
		break;
	}	
}
