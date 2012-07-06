#include "PlatformPrecomp.h"
#include "GamepadProvider60Beat.h"
#include "Gamepad60Beat.h"
#include "GamepadManager.h"
#import "SBJoystick.h"
#import "GamepadProvider60BeatNative.h"

id m_padDriver = nil;


GamepadProvider60Beat::GamepadProvider60Beat()
{
	Kill();
}

GamepadProvider60Beat::~GamepadProvider60Beat()
{
}

bool GamepadProvider60Beat::Init()
{
	LogMsg("Initting 60Beat gamepad provider");
    [SBJoystick sharedInstance].loggingEnabled = YES;
    [SBJoystick sharedInstance].enabled = YES;
    
    assert(m_padDriver == 0);
    m_padDriver = [SBeatPadNative new];
    [m_padDriver Start];
    
    m_pPad = new Gamepad60Beat;
    m_pPad->SetProvider(this);
    GetGamepadManager()->AddGamepad(m_pPad);
    return true; //success
}

void GamepadProvider60Beat::Kill()
{
	[SBJoystick sharedInstance].enabled = NO;
    [m_padDriver release];
    m_padDriver = nil;
}


void GamepadProvider60Beat::Update()
{
	[m_padDriver Update: m_pPad];
}

CL_Vec2f GamepadProvider60Beat::GetLeftStickPos()
{
    
    return CL_Vec2f([m_padDriver GetLeftStickPos].x,[m_padDriver GetLeftStickPos].y);
}

CL_Vec2f GamepadProvider60Beat::GetRightStickPos()
{
    
     return CL_Vec2f([m_padDriver GetRightStickPos].x,[m_padDriver GetRightStickPos].y);
}

