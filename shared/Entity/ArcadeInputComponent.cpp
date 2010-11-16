#include "PlatformPrecomp.h"
#include "ArcadeInputComponent.h"
#include "util/GLESUtils.h"
#include "Entity/EntityUtils.h"
#include "BaseApp.h"


void MoveButtonState::OnPress(int timeToAddMS)
{
	VariantList v;

	if (!m_bIsDown)
	{
		v.Get(0).Set(uint32(m_keyType));
		v.Get(1).Set(uint32(VIRTUAL_KEY_PRESS)); 
		GetBaseApp()->m_sig_arcade_input(&v);
	}
	
	m_bIsDown = true;
	m_releaseTimer = rt_max(GetTick(TIMER_SYSTEM), m_releaseTimer);
	m_releaseTimer += timeToAddMS;
}

void MoveButtonState::OnPressToggle(bool bDown)
{
	VariantList v;

	if (bDown)
	{

	
	if (!m_bIsDown)
	{
		v.Get(0).Set(uint32(m_keyType));
		v.Get(1).Set(uint32(VIRTUAL_KEY_PRESS)); 
		GetBaseApp()->m_sig_arcade_input(&v);
	}

	m_bIsDown = true;
	m_releaseTimer = rt_max(GetTick(TIMER_SYSTEM), m_releaseTimer);
	m_releaseTimer += 1000*60;
	} else
	{
		ReleaseIfNeeded();
	}
}
void MoveButtonState::Update()
{
	if (m_bIsDown && m_releaseTimer < GetTick(TIMER_SYSTEM))
	{
		//time to release, since Android doesn't send one
		ReleaseIfNeeded();
	}
}

void MoveButtonState::ReleaseIfNeeded()
{
	if (m_bIsDown)
	{
		VariantList v;

		v.Get(0).Set(uint32(m_keyType));
		v.Get(1).Set(uint32(VIRTUAL_KEY_RELEASE)); 
		GetBaseApp()->m_sig_arcade_input(&v);

		m_bIsDown = false;
	}
}

ArcadeInputComponent::ArcadeInputComponent()
{
	SetName("ArcadeInput");

	m_buttons[MOVE_BUTTON_DIR_LEFT].SetKeyType(VIRTUAL_KEY_DIR_LEFT);
	m_buttons[MOVE_BUTTON_DIR_RIGHT].SetKeyType(VIRTUAL_KEY_DIR_RIGHT);
	m_buttons[MOVE_BUTTON_DIR_UP].SetKeyType(VIRTUAL_KEY_DIR_UP);
	m_buttons[MOVE_BUTTON_DIR_DOWN].SetKeyType(VIRTUAL_KEY_DIR_DOWN);
}

ArcadeInputComponent::~ArcadeInputComponent()
{
}

void ArcadeInputComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);

	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	
	//register ourselves to render if the parent does
	//GetParent()->GetFunction("OnRender")->sig_function.connect(1, boost::bind(&MovementInputComponent::OnRender, this, _1));
	GetBaseApp()->m_sig_update.connect(1, boost::bind(&ArcadeInputComponent::OnUpdate, this, _1));
	GetBaseApp()->m_sig_trackball.connect(1, boost::bind(&ArcadeInputComponent::OnTrackball, this, _1));
	GetBaseApp()->m_sig_raw_keyboard.connect(1, boost::bind(&ArcadeInputComponent::OnRawKeyboard, this, _1));
	
	GetFunction("AddKeyBinding")->sig_function.connect(1, boost::bind(&ArcadeInputComponent::AddKeyBinding, this, _1));
	m_pTrackballMode = &GetVarWithDefault("trackball_mode", uint32(TRACKBALL_MODE_WALKING))->GetUINT32();

	//get notified when this changes:
	GetVar("trackball_mode")->GetSigOnChanged()->connect(1, boost::bind(&ArcadeInputComponent::OnTrackballModeChanged, this, _1));

	//example of adding a keybinding:
	//pComp->GetFunction("AddKeyBinding")->sig_function(&VariantList(string("Left"), uint32(VIRTUAL_KEY_DIR_LEFT), uint32(VIRTUAL_KEY_DIR_LEFT)));
}

void ArcadeInputComponent::OnRemove()
{
	EntityComponent::OnRemove();
}

void ArcadeInputComponent::OnTrackballModeChanged(Variant *pVar)
{
	if (pVar->GetUINT32() == TRACKBALL_MODE_MENU_SELECTION)
	{
		m_trackball = CL_Vec2f(0,0); //clear whatever was there before
	}
}

void ArcadeInputComponent::AddKeyBinding(VariantList *pVList)
{
	ArcadeKeyBind b;
	b.m_name = pVList->Get(0).GetString();
	b.m_inputkeycode = pVList->Get(1).GetUINT32();
	b.m_outputkeycode = pVList->Get(2).GetUINT32();

	m_bindings.push_back(b);
}

void ArcadeInputComponent::ActivateBinding(ArcadeKeyBind *pBind, bool bDown)
{
	//special handling for directional keys, so they work in tandem with the trackball or whatever else also does directions

	switch (pBind->m_inputkeycode)
	{
		case VIRTUAL_KEY_DIR_LEFT:  m_buttons[MOVE_BUTTON_DIR_LEFT].OnPressToggle(bDown); break;
		case VIRTUAL_KEY_DIR_RIGHT:  m_buttons[MOVE_BUTTON_DIR_RIGHT].OnPressToggle(bDown); break;
		case VIRTUAL_KEY_DIR_UP:  m_buttons[MOVE_BUTTON_DIR_UP].OnPressToggle(bDown); break;
		case VIRTUAL_KEY_DIR_DOWN:  m_buttons[MOVE_BUTTON_DIR_DOWN].OnPressToggle(bDown); break;

	default:

		//for other keys, just send it through the arcade signal:
		{
			VariantList v;
			v.Get(0).Set(pBind->m_outputkeycode);
			v.Get(1).Set(uint32(bDown != 1)); 
			GetBaseApp()->m_sig_arcade_input(&v);

		}
	}

}
void ArcadeInputComponent::OnRawKeyboard(VariantList *pVList)
{
	int keyCode = pVList->Get(0).GetUINT32();
	bool bDown = pVList->Get(1).GetUINT32() == 1;

#ifdef _DEBUG
	LogMsg("Got key %d, %d", keyCode, int(bDown));
#endif

	ArcadeBindList::iterator itor = m_bindings.begin();

	for (;itor != m_bindings.end(); itor++)
	{
		if (keyCode == itor->m_inputkeycode)
		{
			ActivateBinding( &(*itor), bDown );
		}
	}
	
}

void ArcadeInputComponent::OnTrackball(VariantList *pVList)
{

	float x = pVList->Get(1).GetVector3().x;
	float y = pVList->Get(1).GetVector3().y;
	
	
	if (*m_pTrackballMode == TRACKBALL_MODE_MENU_SELECTION)
	{
		m_trackball.x += x;
		m_trackball.y += y;

		if (m_trackball.x < -1.0f) 
		{
			m_buttons[MOVE_BUTTON_DIR_LEFT].OnPress(0);
			m_buttons[MOVE_BUTTON_DIR_LEFT].ReleaseIfNeeded();
			m_trackball.x = 0;
		} else
		{
			if (m_trackball.x > 1.0f) 
			{
				m_buttons[MOVE_BUTTON_DIR_RIGHT].OnPress(0);
				m_buttons[MOVE_BUTTON_DIR_RIGHT].ReleaseIfNeeded();
				m_trackball.x = 0;
			}
		}

		if (m_trackball.y < -1.0f)
		{
			m_buttons[MOVE_BUTTON_DIR_UP].OnPress(0);
			m_buttons[MOVE_BUTTON_DIR_UP].ReleaseIfNeeded();
			m_trackball.y = 0;
		}else
		{
			if (m_trackball.y > 1.0f)
			{
				m_buttons[MOVE_BUTTON_DIR_DOWN].OnPress(0);
				m_buttons[MOVE_BUTTON_DIR_DOWN].ReleaseIfNeeded();
				m_trackball.y = 0;
			}
		}

		return;
	}


	int releaseTime = 50;

	if (x < 0) 
	{
		m_buttons[MOVE_BUTTON_DIR_LEFT].OnPress(releaseTime);
		m_buttons[MOVE_BUTTON_DIR_RIGHT].ReleaseIfNeeded();
	} else
	{
		if (x > 0) 
		{
			m_buttons[MOVE_BUTTON_DIR_RIGHT].OnPress(releaseTime);
			m_buttons[MOVE_BUTTON_DIR_LEFT].ReleaseIfNeeded();
		}
	}
	
	if (y < 0)
	{
		m_buttons[MOVE_BUTTON_DIR_UP].OnPress(releaseTime);
		m_buttons[MOVE_BUTTON_DIR_DOWN].ReleaseIfNeeded();
	}else
	{
		if (y > 0)
		{
			m_buttons[MOVE_BUTTON_DIR_DOWN].OnPress(releaseTime);
			m_buttons[MOVE_BUTTON_DIR_UP].ReleaseIfNeeded();
		}
	}
}

void ArcadeInputComponent::OnUpdate(VariantList *pVList)
{
	for (int i=0; i < MOVE_BUTTON_DIR_COUNT; i++)
	{
		m_buttons[i].Update();
	}
}