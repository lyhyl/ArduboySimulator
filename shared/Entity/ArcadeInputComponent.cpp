#include "PlatformPrecomp.h"
#include "ArcadeInputComponent.h"
#include "util/GLESUtils.h"
#include "Entity/EntityUtils.h"
#include "BaseApp.h"


void MoveButtonState::OnPress(int releaseTime)
{
	VariantList v;

	if (!m_bIsDown)
	{
		v.Get(0).Set(uint32(m_keyType));
		v.Get(1).Set(uint32(VIRTUAL_KEY_PRESS)); 
		GetBaseApp()->m_sig_arcade_input(&v);
	}
	
	m_bIsDown = true;
	m_releaseTimer = GetTick(TIMER_SYSTEM)+releaseTime;
}

void MoveButtonState::Update()
{
	if (m_bIsDown && m_releaseTimer < GetTick(TIMER_SYSTEM))
	{
		//time to release, since Android doesn't send one
		VariantList v;
	
		v.Get(0).Set(uint32(m_keyType));
		v.Get(1).Set(uint32(VIRTUAL_KEY_RELEASE)); 
		GetBaseApp()->m_sig_arcade_input(&v);

		m_bIsDown = false;
	}
}

ArcadeInputComponent::ArcadeInputComponent()
{
	SetName("MovementInput");
	
	
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

}

void ArcadeInputComponent::OnRemove()
{
	EntityComponent::OnRemove();
}

void ArcadeInputComponent::OnTrackball(VariantList *pVList)
{

	float x = pVList->Get(1).GetVector3().x;
	float y = pVList->Get(1).GetVector3().y;
	
	int releaseTime = 100;

	if (x < 0) m_buttons[MOVE_BUTTON_DIR_LEFT].OnPress(releaseTime);
	else if (x > 0) m_buttons[MOVE_BUTTON_DIR_RIGHT].OnPress(releaseTime);
	
	if (y < 0) m_buttons[MOVE_BUTTON_DIR_UP].OnPress(releaseTime);
	else if (y > 0) m_buttons[MOVE_BUTTON_DIR_DOWN].OnPress(releaseTime);
}

void ArcadeInputComponent::OnUpdate(VariantList *pVList)
{
	for (int i=0; i < MOVE_BUTTON_DIR_COUNT; i++)
	{
		m_buttons[i].Update();
	}
}