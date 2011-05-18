#include "PlatformPrecomp.h"
#include "FPSControlComponent.h"
#include "util/GLESUtils.h"
#include "Entity/EntityUtils.h"
#include "BaseApp.h"
#include "Irrlicht/source/Irrlicht/CCameraSceneNode.h"

using namespace irr;

FPSControlComponent::FPSControlComponent()
{
	SetName("FPSControl");
}

FPSControlComponent::~FPSControlComponent()
{

}


void FPSControlComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);
	m_pArrowEnt = NULL;
	m_pTurnArrowEnt = NULL;
	m_bTurningNow = false;

	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	m_lastTouchDir = CL_Vec2f(0, 0); //the middle
	m_lastTurnDir = CL_Vec2f(0,0);

	/*
	m_pSize2d = &GetParent()->GetVar("size2d")->GetVector2();
	m_pScale = &GetParent()->GetShared()->GetVarWithDefault("scale", Variant(1.0f))->GetFloat();
	m_pRotation = &GetParent()->GetVar("rotation")->GetFloat();  //in degrees

	m_pColor = &GetParent()->GetShared()->GetVarWithDefault("color", Variant(MAKE_RGBA(255,255,255,255)))->GetUINT32();
	m_pColorMod = &GetParent()->GetShared()->GetVarWithDefault("colorMod", Variant(MAKE_RGBA(255,255,255,255)))->GetUINT32();
	m_pAlpha = &GetParent()->GetShared()->GetVarWithDefault("alpha", Variant(1.0f))->GetFloat();
	m_pAlignment = &GetParent()->GetVar("alignment")->GetUINT32();
	*/

	//register ourselves to render if the parent does
	GetParent()->GetFunction("OnRender")->sig_function.connect(1, boost::bind(&FPSControlComponent::OnRender, this, _1));
	GetParent()->GetFunction("OnUpdate")->sig_function.connect(1, boost::bind(&FPSControlComponent::OnUpdate, this, _1));

	//movement arrows

	{
	m_pArrowEnt = CreateOverlayEntity(GetParent(), "arrow_gui", "interface/arrows.rttex", iPhoneMapX(20), GetScreenSizeYf()-170);
	EntityComponent *pStripComp = m_pArrowEnt->AddComponent(new TouchStripComponent);
	m_pArrowEnt->GetVarWithDefault(string("touchPadding"), Variant(CL_Rectf(100, 100, 100, 100)))->GetRect();

	m_pArrowEnt->GetFunction("OnTouchStripUpdate")->sig_function.connect(1, boost::bind(&FPSControlComponent::OnStripUpdate, this, _1));	
	m_pArrowEnt->GetFunction("OnOverStart")->sig_function.connect(1, boost::bind(&FPSControlComponent::OnOverStart, this, _1));	
	m_pArrowEnt->GetFunction("OnOverEnd")->sig_function.connect(1, boost::bind(&FPSControlComponent::OnOverEnd, this, _1));	
	
	//limit it to touches only on the left side of the screen
	EntityComponent *pFilter = m_pArrowEnt->AddComponent(new FilterInputComponent);
	pFilter->GetVar("mode")->Set(uint32(FilterInputComponent::MODE_CLIP_INPUT_TO_ABSOLUTE_CLIP_RECT));
	pFilter->GetVar("clipRect")->Set(CL_Rectf(0,0,GetScreenSizeXf()/2, GetScreenSizeY()));
	}

	//turn arrows
	{
	m_pTurnArrowEnt = CreateOverlayEntity(GetParent(), "turn_arrow_gui", "interface/turn_arrows.rttex", GetScreenSizeXf()-150, GetScreenSizeYf()-150);
	EntityComponent *pStripComp = m_pTurnArrowEnt->AddComponent(new TouchStripComponent);
	m_pTurnArrowEnt->GetVarWithDefault(string("touchPadding"), Variant(CL_Rectf(100, 100, 100, 100)))->GetRect();

	m_pTurnArrowEnt->AddComponent(new TouchHandlerComponent);

	m_pTurnArrowEnt->GetFunction("OnTouchStripUpdate")->sig_function.connect(1, boost::bind(&FPSControlComponent::OnTurnStripUpdate, this, _1));	
	m_pTurnArrowEnt->GetFunction("OnOverStart")->sig_function.connect(1, boost::bind(&FPSControlComponent::OnTurnOverStart, this, _1));	
	m_pTurnArrowEnt->GetFunction("OnOverEnd")->sig_function.connect(1, boost::bind(&FPSControlComponent::OnTurnOverEnd, this, _1));	

	//limit it to touches only on the left side of the screen
	EntityComponent *pFilter = m_pTurnArrowEnt->AddComponent(new FilterInputComponent);
	pFilter->GetVar("mode")->Set(uint32(FilterInputComponent::MODE_CLIP_INPUT_TO_ABSOLUTE_CLIP_RECT));
	pFilter->GetVar("clipRect")->Set(CL_Rectf(GetScreenSizeXf()/2,0,GetScreenSizeXf(), GetScreenSizeYf()));

	//m_pTurnArrowEnt->GetVar("ignoreTouchesOutsideRect")->Set(uint32(1));

	}

}

void FPSControlComponent::OnRemove()
{
	EntityComponent::OnRemove();
}

void FPSControlComponent::SendKey(irr::EKEY_CODE key, bool bIsDown)
{
	irr::SEvent e;

	e.EventType = irr::EET_KEY_INPUT_EVENT;
	e.KeyInput.Char = 0;
	e.KeyInput.Key = key; //irr::KEY_UP;
	e.KeyInput.Control = false;
	e.KeyInput.PressedDown = bIsDown;
	e.KeyInput.Shift = false;

	GetIrrlichtManager()->GetDevice()->postEventFromUser(e);
}

void FPSControlComponent::OnOverStart(VariantList *pVList)
{
	//calculate which sector they are holding the dpad in

	if (m_lastTouchDir.length() < 0.2)
	{
		//dead space
		return;
	}

	//convert to 360 degrees
	int dir = (int(RAD2DEG(atan2(m_lastTouchDir.y, m_lastTouchDir.x))+(180)));
	
	//for now, convert into 4 directions
	int finaldir = mod(dir-45, 360)/90;
	//LogMsg("Pressing %s, which is dir %d (final: %d)", PrintVector2(m_lastTouchDir).c_str(), dir, finaldir);
	
	switch (finaldir)
	{
	case 0:  SendKey(irr::KEY_UP, true); break;
	case 1:  SendKey(irr::KEY_RIGHT, true); break;
	case 2:  SendKey(irr::KEY_DOWN, true); break;
	case 3:  SendKey(irr::KEY_LEFT, true); break;
	}
	

	m_pArrowEnt->GetVar("alpha")->Set(0.7f);
}

void FPSControlComponent::OnOverEnd(VariantList *pVList)
{
	SendKey(irr::KEY_UP, false);
	SendKey(irr::KEY_DOWN, false);
	SendKey(irr::KEY_LEFT, false);
	SendKey(irr::KEY_RIGHT, false);
	m_pArrowEnt->GetVar("alpha")->Set(1.0f);

}

void FPSControlComponent::OnStripUpdate(VariantList *pVList)
{
	m_lastTouchDir = (pVList->Get(1).GetVector2()*2) - CL_Vec2f(1, 1);

	//update current position
	OnOverEnd(&VariantList(CL_Vec2f(0,0)));
	OnOverStart(&VariantList(CL_Vec2f(0,0)));

}

void FPSControlComponent::OnTurnOverStart(VariantList *pVList)
{
	m_pTurnArrowEnt->GetVar("alpha")->Set(0.7f);
	 m_bTurningNow = true;
}

void FPSControlComponent::OnTurnOverEnd(VariantList *pVList)
{
	m_pTurnArrowEnt->GetVar("alpha")->Set(1.0f);
	m_bTurningNow = false;
}

void FPSControlComponent::OnTurnStripUpdate(VariantList *pVList)
{
	m_lastTurnDir = (pVList->Get(1).GetVector2()*2) - CL_Vec2f(1, 1);

}

void FPSControlComponent::OnRender(VariantList *pVList)
{
	CL_Vec2f vFinalPos = pVList->m_variant[0].GetVector2()+*m_pPos2d;
}

void FPSControlComponent::OnUpdate(VariantList *pVList)
{

	if (m_bTurningNow)
	{
	
		irr::scene::CCameraSceneNode *camera = (irr::scene::CCameraSceneNode*) GetIrrlichtManager()->GetScene()->getActiveCamera(); 
		if (!camera)
		{
		//	LogError("This only works with scenes that have CSceneNodeAnimatorCameraFPS in them.  Seth's modified version, at that.");
			return;
		}

		
		// Update rotation
		core::vector3df target = (camera->getTarget() - camera->getAbsolutePosition());
		core::vector3df relativeRotation = target.getHorizontalAngle();
		core::vector3df pos = camera->getPosition();


		//update it
		float sensitivityX = 3;
		float sensitivityY = 2;

		relativeRotation.Y += m_lastTurnDir.x * GetBaseApp()->GetGameDelta()*sensitivityX;
		relativeRotation.X += m_lastTurnDir.y * GetBaseApp()->GetGameDelta()*sensitivityY;


	
		target.set(0,0, core::max_(1.f, pos.getLength()));
		core::vector3df movedir = target;

		core::matrix4 mat;
		mat.setRotationDegrees(core::vector3df(relativeRotation.X, relativeRotation.Y, 0));
		mat.transformVect(target);

		if (1)
		{
			mat.setRotationDegrees(core::vector3df(0, relativeRotation.Y, 0));
			mat.transformVect(movedir);
		}
		else
		{
			movedir = target;
		}

		movedir.normalize();

	// write right target
	target += pos;
	camera->setTarget(target);
	}

}