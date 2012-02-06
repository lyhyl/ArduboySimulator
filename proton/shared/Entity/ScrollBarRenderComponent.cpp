#include "PlatformPrecomp.h"

#include "ScrollBarRenderComponent.h"
#include "util/GLESUtils.h"
#include "BaseApp.h"
#include "Entity/EntityUtils.h"

ScrollBarRenderComponent::ScrollBarRenderComponent()
{
	m_pSurf = NULL;
	SetName("ScrollBarRender");
}

ScrollBarRenderComponent::~ScrollBarRenderComponent()
{
}

void ScrollBarRenderComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);
	//shared with the rest of the entity
	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	m_pSize2d = &GetParent()->GetVar("size2d")->GetVector2();
	m_pScale2d = &GetParent()->GetVarWithDefault("scale2d", Variant(1.0f, 1.0f))->GetVector2();
	m_pAlpha = &GetParent()->GetVarWithDefault("alpha", Variant(0.3f))->GetFloat();
	m_pColor = &GetParent()->GetVarWithDefault("color", Variant(MAKE_RGBA(224,188,130,255)))->GetUINT32();
	m_pColorMod = &GetParent()->GetVarWithDefault("colorMod", Variant(MAKE_RGBA(255,255,255,255)))->GetUINT32();
	
	m_pFileName = &GetVar("fileName")->GetString(); //local to us
	
	GetParent()->GetFunction("OnUpdate")->sig_function.connect(1, boost::bind(&ScrollBarRenderComponent::OnUpdate, this, _1));
	GetParent()->GetFunction("OnRender")->sig_function.connect(1, boost::bind(&ScrollBarRenderComponent::OnRender, this, _1));
	GetParent()->GetFunction("OnOverStart")->sig_function.connect(1, boost::bind(&ScrollBarRenderComponent::OnTargetOverStart, this, _1));
	GetParent()->GetFunction("OnOverEnd")->sig_function.connect(1, boost::bind(&ScrollBarRenderComponent::OnTargetOverEnd, this, _1));
	//if "fileName" is set, we'll know about it here
	GetVar("fileName")->GetSigOnChanged()->connect(boost::bind(&ScrollBarRenderComponent::OnFileNameChanged, this, _1));

	GetVar("fileName")->Set("interface/scroll_bar_caps.rttex"); //default

	EntityComponent *pScrollComp = GetParent()->GetComponentByName("Scroll");
	if (!pScrollComp)
	{
		//assume our stuff will get set from the outside
		m_pBoundsRect = &GetParent()->GetVar("boundsRect")->GetRect();
		m_pProgress2d = &GetParent()->GetVar("progress2d")->GetVector2();

	} else
	{
		m_pBoundsRect = &pScrollComp->GetVar("boundsRect")->GetRect();
		m_pProgress2d = &pScrollComp->GetVar("progress2d")->GetVector2();
	}
}


void ScrollBarRenderComponent::OnFileNameChanged(Variant *pDataObject)
{
	SAFE_DELETE(m_pSurf);

	m_pSurf = GetResourceManager()->GetSurfaceAnim(pDataObject->GetString());
	if (m_pSurf)
	{
		m_pSurf->SetupAnim(1,2);
	} else
	{
		LogMsg("ScrollBarRenderComponent: Can't load scroll bar pieces");
	}

}
void ScrollBarRenderComponent::OnTargetOverStart(VariantList *pVList)
{	
	FadeEntity(GetParent(), false, 0.6f, 100);
}

void ScrollBarRenderComponent::OnTargetOverEnd(VariantList *pVList)
{	
	FadeEntity(GetParent(), false, 0.3f, 1000);
}

void ScrollBarRenderComponent::OnRemove()
{
	EntityComponent::OnRemove();
}

void ScrollBarRenderComponent::OnUpdate(VariantList *pVList)
{	
}

void ScrollBarRenderComponent::OnRender(VariantList *pVList)
{
	//NOTE: We don't support drawing a horizontal scroll bar yet!
	
	//LogMsg("Drawing progress bar: %.2f", progress);
	if (*m_pAlpha <= 0.07)
	{
		return; //not ready
	}

	float contentAreaRatio = (m_pBoundsRect->get_height()+m_pSize2d->y)/m_pSize2d->y;
	if (contentAreaRatio <= 1)
	{
		//don't need a scroll bar
		return;
	}

	GLboolean bScissorEnabled = false;
	glGetBooleanv(GL_SCISSOR_TEST, &bScissorEnabled);

	if (bScissorEnabled)
	{
		g_globalBatcher.Flush();
		//disable it temporarily
		glDisable(GL_SCISSOR_TEST);
	}

	float barHeight = m_pSize2d->y/contentAreaRatio;
	if (!m_pSurf) return; //can't do anything without the graphics loaded
	
	if (barHeight < m_pSurf->GetFrameHeight()*2) barHeight = m_pSurf->GetFrameHeight()*2;
	float barWidth = m_pSurf->GetFrameWidth();
	uint32 color = ColorCombine(*m_pColor, *m_pColorMod, *m_pAlpha);
	//LogMsg("percent scrolled is %.2f, contentAreaRation is %.2f", m_pProgress2d->y, contentAreaRatio);

	CL_Vec2f vFinalPos = pVList->m_variant[0].GetVector2()+ *m_pPos2d + CL_Vec2f(m_pSize2d->x, 0);

	if (vFinalPos.x >= GetScreenSizeXf())
	{
		//position on the inside, not the outside
		vFinalPos.x -= ( barWidth+(iPadMapX(8) )); //adjust the spacer with the screensize
	}
	//slide it down to the right position:
	vFinalPos.y += (m_pSize2d->y - barHeight)* m_pProgress2d->y;

	//draw the top of the capsule
	m_pSurf->BlitAnim(vFinalPos.x, vFinalPos.y,0,0, color);
	vFinalPos.y += m_pSurf->GetFrameHeight(); 
	barHeight -=  m_pSurf->GetFrameHeight()*2;
	//draw the bottom end cap
	m_pSurf->BlitAnim(vFinalPos.x, vFinalPos.y+barHeight,0,1, color);
	//first draw the first end cap
	CL_Rectf r = CL_Rectf(0, 0, barWidth, barHeight);
	ApplyOffset(&r, vFinalPos);
	DrawFilledRect(r, color);
	if (bScissorEnabled)
	{
		g_globalBatcher.Flush();
		glEnable(GL_SCISSOR_TEST);
	}
	
}