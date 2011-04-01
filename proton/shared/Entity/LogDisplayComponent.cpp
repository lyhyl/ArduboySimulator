#include "PlatformPrecomp.h"
#include "LogDisplayComponent.h"
#include "Entity/EntityUtils.h"

LogDisplayComponent::LogDisplayComponent()
{
	SetName("LogDisplay");
	m_pActiveConsole = NULL;
	m_pInternalConsole = NULL;
}

LogDisplayComponent::~LogDisplayComponent()
{

	SAFE_DELETE(m_pInternalConsole);
}

void LogDisplayComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);

	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	m_pSize2d = &GetParent()->GetVar("size2d")->GetVector2();
	m_pFontID = &GetVarWithDefault("font", uint32(FONT_SMALL))->GetUINT32();
	
	/*
		m_pScale2d = &GetParent()->GetShared()->GetVarWithDefault("scale2d", Variant(1.0f, 1.0f))->GetVector2();
	m_pRotation = &GetParent()->GetVar("rotation")->GetFloat();  //in degrees

	m_pColor = &GetParent()->GetShared()->GetVarWithDefault("color", Variant(MAKE_RGBA(255,255,255,255)))->GetUINT32();
	m_pColorMod = &GetParent()->GetShared()->GetVarWithDefault("colorMod", Variant(MAKE_RGBA(255,255,255,255)))->GetUINT32();
	m_pAlpha = &GetParent()->GetShared()->GetVarWithDefault("alpha", Variant(1.0f))->GetFloat();
	m_pAlignment = &GetParent()->GetVar("alignment")->GetUINT32();
	*/

	if (!m_pActiveConsole) m_pActiveConsole = GetBaseApp()->GetConsole();
	//register ourselves to render if the parent does
	GetParent()->GetFunction("OnRender")->sig_function.connect(1, boost::bind(&LogDisplayComponent::OnRender, this, _1));
	GetFunction("AddLine")->sig_function.connect(1, boost::bind(&LogDisplayComponent::AddLine, this, _1));
//	GetParent()->GetFunction("OnUpdate")->sig_function.connect(1, boost::bind(&LogDisplayComponent::OnUpdate, this, _1));
}

void LogDisplayComponent::OnRemove()
{
	EntityComponent::OnRemove();
}


void LogDisplayComponent::AddLine(VariantList *pVList)
{
 if (!m_pInternalConsole)
 {
	 m_pInternalConsole = new Console;
	 m_pActiveConsole = m_pInternalConsole;
 }

 //word wrap it into lines if needed
 deque<string> lines;
CL_Vec2f enclosedSize2d;
 GetBaseApp()->GetFont(eFont(*m_pFontID))->MeasureTextAndAddByLinesIntoDeque(*m_pSize2d, pVList->Get(0).GetString(), &lines, 1, enclosedSize2d);

 for (;lines.size();)
 {
	 m_pActiveConsole->AddLine(lines.front());
	 lines.pop_front();
 }

}

void LogDisplayComponent::OnRender(VariantList *pVList)
{
	CL_Vec2f vFinalPos = pVList->m_variant[0].GetVector2()+*m_pPos2d;
	
	int lines = m_pActiveConsole->GetTotalLines();

	eFont fontID = eFont(*m_pFontID);
	float fontScale = 1.0f;
	RTFont *pFont = GetBaseApp()->GetFont(fontID);

	float y = vFinalPos.y + m_pSize2d->y;
	assert(m_pSize2d->y > 0);
	y -= pFont->GetLineHeight(fontScale);

	int curLine = lines-1;
	RenderBatcher b;

	
	while (y > vFinalPos.y && curLine >= 0)
	{
		pFont->DrawScaled(vFinalPos.x, y, m_pActiveConsole->GetLine(curLine), fontScale,
			MAKE_RGBA(255,255,255, 255), NULL, &b);

		curLine--;
		y -= pFont->GetLineHeight(fontScale);
	}

	b.Flush();
}


void SetConsole(bool bOn)
{
	Entity *pConsole = GetEntityRoot()->GetEntityByName("ConsoleEnt");

	if (bOn && pConsole) return; //already on
	if (!bOn && !pConsole) return; //already off

	if (pConsole)
	{
		//kill it
		KillEntity(pConsole);
	} else
	{
		pConsole = GetEntityRoot()->AddEntity(new Entity("ConsoleEnt"));
		pConsole->GetVar("pos2d")->Set(CL_Vec2f(GetScreenSizeXf()/2, 80));
		pConsole->GetVar("size2d")->Set(CL_Vec2f(GetScreenSizeXf()/2, GetScreenSizeYf()/2));

		EntityComponent *pComp = new LogDisplayComponent;
		pConsole->AddComponent(pComp);
		AddFocusIfNeeded(pConsole);
	}
}

void ToggleConsole()
{
	Entity *pConsole = GetEntityRoot()->GetEntityByName("ConsoleEnt");

	if (pConsole)
	{
		//kill it
		SetConsole(false);
	} else
	{
		SetConsole(true);
	}
}
