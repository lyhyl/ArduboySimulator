#include "PlatformPrecomp.h"
#include "GamepadProvideriCade.h"
#include "GamepadiCade.h"
#include "GamepadManager.h"


GamepadProvideriCade::GamepadProvideriCade()
{
	Kill();
}

GamepadProvideriCade::~GamepadProvideriCade()
{
}

bool GamepadProvideriCade::Init()
{
	LogMsg("Initting _iCade gamepad provider");
	m_pPad = new GamepadiCade;
	m_pPad->SetProvider(this);
	GetGamepadManager()->AddGamepad(m_pPad);
	return true; //success
}

void GamepadProvideriCade::Kill()
{
}

void GamepadProvideriCade::Update()
{
	
}

