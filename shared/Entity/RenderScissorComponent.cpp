#include "PlatformPrecomp.h"
#include "RenderScissorComponent.h"
#include "BaseApp.h"


RenderScissorComponent::RenderScissorComponent()
{
	SetName("RenderScissor");
	m_bOldScissorEnabled= false;
	
	
}

RenderScissorComponent::~RenderScissorComponent()
{
}

void RenderScissorComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);

	//register to get updated every frame

	GetParent()->OnFilterAdd();
	GetParent()->GetFunction("FilterOnRender")->sig_function.connect(1, boost::bind(&RenderScissorComponent::FilterOnRender, this, _1));
	GetParent()->GetFunction("PostOnRender")->sig_function.connect(1, boost::bind(&RenderScissorComponent::PostOnRender, this, _1));

	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	m_pSize2d = &GetParent()->GetVar("size2d")->GetVector2();
	m_pAlignment = &GetParent()->GetVar("alignment")->GetUINT32();
	m_pScissorMode = &GetParent()->GetVarWithDefault("ScissorMode", uint32(POSITION_FROM_SIZE))->GetUINT32();

	//our own vars

}

void RenderScissorComponent::OnRemove()
{
	GetParent()->OnFilterRemove();
	EntityComponent::OnRemove();
}

void RenderScissorComponent::FilterOnRender(VariantList *pVList)
{
	g_globalBatcher.Flush();

	GLboolean b = false;
	glGetBooleanv(GL_SCISSOR_TEST, &b);
	m_bOldScissorEnabled = b;
	
	if (m_bOldScissorEnabled)
	{
		//warning: Untested code...
		GLint nums[4];
		glGetIntegerv(GL_SCISSOR_BOX, &nums[0]);
		m_oldScissorPos = CL_Vec2f(nums[0], nums[1]);
		m_oldScissorSize = CL_Vec2f(nums[2], nums[3]);
	}
	CL_Vec2f vFinalPos = pVList->m_variant[0].GetVector2()+*m_pPos2d;
	//vFinalPos -= GetAlignmentOffset(*m_pSize2d, eAlignment(*m_pAlignment));	
	
	//remember, glScissors x/y is the LOWER LEFT of the rect, not upper left.
	glScissor(vFinalPos.x, (GetScreenSizeY()-vFinalPos.y)-m_pSize2d->y, m_pSize2d->x, m_pSize2d->y);
	glEnable(GL_SCISSOR_TEST);

}

void RenderScissorComponent::PostOnRender(VariantList *pVList)
{
	g_globalBatcher.Flush();
	if (m_bOldScissorEnabled)
	{
		//leave it enabled.. setup the box to how it was
		glScissor(m_oldScissorPos.x, m_oldScissorPos.y, m_oldScissorSize.x, m_oldScissorSize.y);
	} else
	{
		glDisable(GL_SCISSOR_TEST);
	}
}

