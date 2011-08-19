#include "PlatformPrecomp.h"

#include "TouchHandlerComponent.h"
#include "Manager/MessageManager.h"
#include "Util/MiscUtils.h"
#include "BaseApp.h"

TouchHandlerComponent::TouchHandlerComponent()
{
	SetName("TouchHandler");
}

TouchHandlerComponent::~TouchHandlerComponent()
{
}

void TouchHandlerComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);
	m_pTouchOver = GetParent()->GetVar("touchOver");
	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	m_pSize2d = &GetParent()->GetVar("size2d")->GetVector2();
	m_pAlignment = &GetParent()->GetVar("alignment")->GetUINT32();
	m_pTouchPadding = &GetParent()->GetVarWithDefault(string("touchPadding"), Variant(CL_Rectf(20.0f, 5.0f, 20.0f, 15.0f)))->GetRect();
	m_pIgnoreTouchesOutsideRect = &GetParent()->GetVar("ignoreTouchesOutsideRect")->GetUINT32();

	GetParent()->GetFunction("OnInput")->sig_function.connect(1, boost::bind(&TouchHandlerComponent::OnInput, this, _1));
	//GetFunction("EndClick")->sig_function.connect(1, boost::bind(&TouchHandlerComponent::EndClick, this, _1));

	EntityComponent::OnAdd(pEnt);
}

void TouchHandlerComponent::OnRemove()
{
	EntityComponent::OnRemove();
}

void TouchHandlerComponent::HandleClickStart(CL_Vec2f &pt, uint32 fingerID)
{
	
	if (m_pTouchOver->GetUINT32())
	{
		//LogMsg("Ignoring click start because we're already 'down'")	;
		return;
	}

	//first, determine if the click is on our area
	CL_Rectf r(*m_pPos2d, CL_Sizef(m_pSize2d->x, m_pSize2d->y));
	ApplyPadding(&r, *m_pTouchPadding);

	if (r.contains(pt))
	{
		m_pTouchOver->Set(uint32(1));
		GetParent()->GetFunction("OnTouchStart")->sig_function(&VariantList(pt, GetParent(), fingerID));
		GetParent()->GetFunction("OnOverStart")->sig_function(&VariantList(pt, GetParent(), fingerID));
	}
}

void TouchHandlerComponent::HandleClickMove( CL_Vec2f &pt, uint32 fingerID )
{
	//first, determine if the click is on our area
	CL_Rectf r(*m_pPos2d, CL_Sizef(m_pSize2d->x, m_pSize2d->y));
	ApplyPadding(&r, *m_pTouchPadding);

	if (*m_pIgnoreTouchesOutsideRect != 0)
	{
		if (!r.contains(pt))
		{
			//ignore this
			return;
		}
	}

	if (m_pTouchOver->GetUINT32())
	{
		//currently over, did we move off?
		if (r.contains(pt))
		{
			//still over, do nothing?
			FunctionObject *pFunc = GetParent()->GetShared()->GetFunctionIfExists("OnOverMove");

			if (pFunc)
			{
				// I guess someone cares about knowing if it moves as well
				pFunc->sig_function(&VariantList(pt, GetParent()));
			}

		} else
		{
			m_pTouchOver->Set(uint32(0));
			GetParent()->GetFunction("OnOverEnd")->sig_function(&VariantList(pt, GetParent(), fingerID));
		}
	} else
	{
		//currently not over, but did we move onto it?

		if (r.contains(pt))
		{
			m_pTouchOver->Set(uint32(1));
			GetParent()->GetFunction("OnOverStart")->sig_function(&VariantList(pt, GetParent(), fingerID));
		
		} else
		{
			//not on it, do nothing
		}

	}
}

void TouchHandlerComponent::HandleClickEnd( CL_Vec2f &pt, uint32 fingerID )
{
	if (!m_pTouchOver->GetUINT32()) return;

	//first, determine if the click is on our area
	CL_Rectf r(*m_pPos2d, CL_Sizef(m_pSize2d->x, m_pSize2d->y));
	ApplyPadding(&r, *m_pTouchPadding);

	if (*m_pIgnoreTouchesOutsideRect != 0)
	{
		if (!r.contains(pt))
		{
			//they lifted a finger, but not in our rect so ignore it
			return;
		}
	}

	m_pTouchOver->Set(uint32(0));
	GetParent()->GetFunction("OnOverEnd")->sig_function(&VariantList(pt, GetParent(), fingerID));

	if (r.contains(pt))
	{
			GetParent()->GetFunction("OnTouchEnd")->sig_function(&VariantList(pt, GetParent(), fingerID));
	}
}

void TouchHandlerComponent::OnInput( VariantList *pVList )
{
	//0 = message type, 1 = parent coordinate offset
	CL_Vec2f pt = pVList->Get(1).GetVector2();
	//pt += GetAlignmentOffset(*m_pSize2d, eAlignment(*m_pAlignment));

	uint32 fingerID = 0;
	if (pVList->Get(2).GetType() == Variant::TYPE_UINT32)
	{
		fingerID = pVList->Get(2).GetUINT32();
	}

	switch (eMessageType( int(pVList->Get(0).GetFloat())))
	{
		case MESSAGE_TYPE_GUI_CLICK_START:
			HandleClickStart(pt, fingerID);
			break;
		case MESSAGE_TYPE_GUI_CLICK_END:
			HandleClickEnd(pt, fingerID);
			break;
		case MESSAGE_TYPE_GUI_CLICK_MOVE:
			HandleClickMove(pt, fingerID);
			break;
	}	
	
}
