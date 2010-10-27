#include "PlatformPrecomp.h"
#include "TouchStripComponent.h"
#include "BaseApp.h"

TouchStripComponent::TouchStripComponent()
{
	SetName("TouchStrip");
}

TouchStripComponent::~TouchStripComponent()
{
}

void TouchStripComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);
	m_lastPos = CL_Vec2f(-1,-1);
	m_pDisabled = &GetVarWithDefault("disabled", uint32(0))->GetUINT32();
	m_pVisualStyle = &GetVarWithDefault("visualStyle", uint32(STYLE_NONE))->GetUINT32();
	m_pOnTouchStripUpdate = GetParent()->GetFunction("OnTouchStripUpdate");
	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	m_pSize2d = &GetParent()->GetVar("size2d")->GetVector2();
	m_pMult = &GetVarWithDefault("mult", CL_Vec2f(1,1))->GetVector2();
	m_pSwapXAndY = &GetVar("swapXAndY")->GetUINT32();
	m_pReverseX = &GetVar("reverseX")->GetUINT32();
	m_pReverseY = &GetVar("reverseY")->GetUINT32();
	
	//this will only be set if TouchStripComponent is initted before the TouchHandler...
	//GetParent()->GetVarWithDefault(string("touchPadding"), Variant(CL_Rectf(0.0f, 0.0f, 0.0f, 0.0f)))->GetRect();

	//register to get updated every frame

	GetParent()->GetFunction("OnOverStart")->sig_function.connect(1, boost::bind(&TouchStripComponent::OnOverStart, this, _1));
	GetParent()->GetFunction("OnOverEnd")->sig_function.connect(1, boost::bind(&TouchStripComponent::OnOverEnd, this, _1));
	GetParent()->GetFunction("OnOverMove")->sig_function.connect(1, boost::bind(&TouchStripComponent::OnOverMove, this, _1));

}

void TouchStripComponent::OnRemove()
{
	EntityComponent::OnRemove();
}

void TouchStripComponent::OnOverStart(VariantList *pVList)
{
	SetPosition(pVList->Get(0).GetVector2());
}

void TouchStripComponent::OnOverEnd(VariantList *pVList)
{
	SetPosition(pVList->Get(0).GetVector2());
}

void TouchStripComponent::OnOverMove(VariantList *pVList)
{
	SetPosition(pVList->Get(0).GetVector2());
}

void TouchStripComponent::SetPosition(CL_Vec2f vPos)
{
	//if (vPos == m_lastPos) return;
	m_lastPos = vPos;
	if (vPos.x < m_pPos2d->x) vPos.x = m_pPos2d->x;
	if (vPos.x > m_pPos2d->x + m_pSize2d->x) vPos.x = m_pPos2d->x + m_pSize2d->x;

	if (vPos.y < m_pPos2d->y) vPos.y = m_pPos2d->y;
	if (vPos.y > m_pPos2d->y + m_pSize2d->y) vPos.y = m_pPos2d->y + m_pSize2d->y;
	
	//convert to 0..1 range
	vPos -= *m_pPos2d;

	vPos.x = vPos.x / m_pSize2d->x;	
	vPos.y = vPos.y / m_pSize2d->y;

	if (*m_pSwapXAndY != 0)
	{
		swap(vPos.x, vPos.y);
	}
	if (*m_pReverseX != 0)
	{
		vPos.x = 1-vPos.x;
	}
	if (*m_pReverseY != 0)
	{
		vPos.y = 1-vPos.y;
	}
	vPos.x *= m_pMult->x;
	vPos.y *= m_pMult->y;
	
	m_pOnTouchStripUpdate->sig_function(&VariantList(this, vPos));
}

