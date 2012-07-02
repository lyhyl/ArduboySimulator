#include "PlatformPrecomp.h"
#include "GamepadProvider_iCade.h"
#include "Gamepad_iCade.h"
#include "GamepadManager.h"


GamepadProvider_iCade::GamepadProvider_iCade()
{
	Kill();
}

GamepadProvider_iCade::~GamepadProvider_iCade()
{
}

bool GamepadProvider_iCade::Init()
{
	LogMsg("Initting _iCade gamepad provider");
	
	m_pPad = new Gamepad_iCade;
	//m_pPad->SetProvider(this);
	GetGamepadManager()->AddGamepad(m_pPad);
	return true; //success
}

void GamepadProvider_iCade::Kill()
{
}

void GamepadProvider_iCade::Update()
{
	
}

