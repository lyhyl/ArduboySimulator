#include "PlatformPrecomp.h"

#include "TouchHandlerArcadeComponent.h"
#include "Manager/MessageManager.h"
#include "util/MiscUtils.h"
#include "BaseApp.h"

TouchHandlerArcadeComponent::TouchHandlerArcadeComponent()
{
	m_activeFinger = -1;

	SetName("TouchHandlerArcade");
}

TouchHandlerArcadeComponent::~TouchHandlerArcadeComponent()
{
	if (m_activeFinger != -1)
	{
		//mark the touch we were using as unhandled now, so if we're recreated right at the same place controls don't
		//go dead until they release and touch again
		TouchTrackInfo *pTouch = GetBaseApp()->GetTouch(m_activeFinger);
		if (pTouch)
		{
			pTouch->SetWasHandled(false);
		}
	}
}

void TouchHandlerArcadeComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);
	m_pTouchOver = GetParent()->GetVar("touchOver");
	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	m_pSize2d = &GetParent()->GetVar("size2d")->GetVector2();
	m_pAlignment = &GetParent()->GetVar("alignment")->GetUINT32();
	m_pTouchPadding = &GetParent()->GetVarWithDefault(string("touchPadding"), Variant(CL_Rectf(20.0f, 5.0f, 20.0f, 15.0f)))->GetRect();
	m_pIgnoreTouchesOutsideRect = &GetParent()->GetVar("ignoreTouchesOutsideRect")->GetUINT32();

	GetParent()->GetFunction("OnInput")->sig_function.connect(1, boost::bind(&TouchHandlerArcadeComponent::OnInput, this, _1));
	//GetFunction("EndClick")->sig_function.connect(1, boost::bind(&TouchHandlerArcadeComponent::EndClick, this, _1));

	GetParent()->GetVar("pos2d")->GetSigOnChanged()->connect(boost::bind(&TouchHandlerArcadeComponent::UpdateTouchArea, this, _1));
	GetParent()->GetVar("size2d")->GetSigOnChanged()->connect(boost::bind(&TouchHandlerArcadeComponent::UpdateTouchArea, this, _1));
	GetParent()->GetVar("touchPadding")->GetSigOnChanged()->connect(boost::bind(&TouchHandlerArcadeComponent::UpdateTouchArea, this, _1));

	UpdateTouchArea(NULL);

	EntityComponent::OnAdd(pEnt);
}

void TouchHandlerArcadeComponent::OnRemove()
{
	EntityComponent::OnRemove();
}

void TouchHandlerArcadeComponent::HandleClickStart(CL_Vec2f &pt, uint32 fingerID)
{
	if (m_pTouchOver->GetUINT32())
	{
		LogMsg("Ignoring click start because we're already 'down'.  Shouldn't be possible really.")	;
		return;
	}


	//first, determine if the click is on our area
	if (m_touchArea.contains(pt))
	{
		TouchTrackInfo *pTouch = GetBaseApp()->GetTouch(fingerID);
		pTouch->SetWasHandled(true, GetParent());
		m_activeFinger = fingerID;
		m_pTouchOver->Set(uint32(1));
		VariantList vList(pt, GetParent(), fingerID, uint32(true));

		GetParent()->GetFunction("OnTouchStart")->sig_function(&vList);
		GetParent()->GetFunction("OnOverStart")->sig_function(&vList);
	}
}

void TouchHandlerArcadeComponent::HandleClickMove( CL_Vec2f &pt, uint32 fingerID )
{
	TouchTrackInfo *pTouch = GetBaseApp()->GetTouch(fingerID);
	
	if (pTouch->GetEntityThatHandledIt() == GetParent())
	{
		//currently over, did we move off?
		if (m_touchArea.contains(pt))
		{
			//still over, do nothing?
			FunctionObject *pFunc = GetParent()->GetShared()->GetFunctionIfExists("OnOverMove");

			if (pFunc)
			{
				// I guess someone cares about knowing if it moves as well
				VariantList vList(pt, GetParent(), fingerID, uint32(true));
				pFunc->sig_function(&vList);
			}

		} else
		{
			m_pTouchOver->Set(uint32(0));
			VariantList vList(pt, GetParent(), fingerID, uint32(false));
			GetParent()->GetFunction("OnOverEnd")->sig_function(&vList);
			m_activeFinger = -1;
			pTouch->SetWasHandled(false);
		}
	} else
	{
		//currently not over, but did we move onto it?
		if (m_touchArea.contains(pt))
		{
			m_pTouchOver->Set(uint32(1));
			VariantList vList(pt, GetParent(), fingerID, uint32(true));
			pTouch->SetWasHandled(true, GetParent());
			m_activeFinger = fingerID;
			GetParent()->GetFunction("OnOverStart")->sig_function(&vList);
		} else
		{
			//not on it, do nothing
		}

	}
}

void TouchHandlerArcadeComponent::HandleClickEnd( CL_Vec2f &pt, uint32 fingerID )
{
	TouchTrackInfo *pTouch = GetBaseApp()->GetTouch(fingerID);

	if (pTouch->GetEntityThatHandledIt() != GetParent())
	{
		return;
	}
	
	const bool touchAreaContainsTouchPoint = m_touchArea.contains(pt);
	
	m_pTouchOver->Set(uint32(0));
	VariantList vList(pt, GetParent(), fingerID, uint32(touchAreaContainsTouchPoint));

	GetParent()->GetFunction("OnOverEnd")->sig_function(&vList);

	if (touchAreaContainsTouchPoint)
	{
		GetParent()->GetFunction("OnTouchEnd")->sig_function(&vList);
	}
	m_activeFinger = -1;
	pTouch->SetWasHandled(false);

}

void TouchHandlerArcadeComponent::OnInput( VariantList *pVList )
{
	//0 = message type, 1 = parent coordinate offset
	CL_Vec2f pt = pVList->Get(1).GetVector2();

	uint32 fingerID = 0;
	if (pVList->Get(2).GetType() == Variant::TYPE_UINT32)
	{
		fingerID = pVList->Get(2).GetUINT32();
	} else
	{
		//what is this?
		return;
	}

	if (m_activeFinger != -1 && m_activeFinger != fingerID)
	{
		//don't care, we're tracking something else right now
		return;
	}

    TouchTrackInfo *pTouch;
    
	switch (eMessageType( int(pVList->Get(0).GetFloat())))
	{

	case MESSAGE_TYPE_GUI_CLICK_START:
	case MESSAGE_TYPE_GUI_CLICK_END:
	case MESSAGE_TYPE_GUI_CLICK_MOVE:

		pTouch = GetBaseApp()->GetTouch(fingerID);
		if (pTouch->WasHandled() && pTouch->GetEntityThatHandledIt() != GetParent()) return;

		if (pTouch->WasPreHandled()) return; //a scroll box marks if this way, if a scroller is in front of us,
		//we don't want to claim ownership of this
		break;

	default:;
		return;
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

        default:;
	}	

}

void TouchHandlerArcadeComponent::UpdateTouchArea(Variant *v)
{
	m_touchArea.set_top_left(*m_pPos2d);
	m_touchArea.set_size(CL_Sizef(m_pSize2d->x, m_pSize2d->y));
	ApplyPadding(&m_touchArea, *m_pTouchPadding);
}
