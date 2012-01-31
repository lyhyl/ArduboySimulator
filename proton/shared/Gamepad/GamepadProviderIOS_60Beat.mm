#include "PlatformPrecomp.h"
#include "GamepadProviderIOS_60Beat.h"
#include "GamepadIOS_60Beat.h"
#include "GamepadManager.h"
#import "SBJoystick.h"
#import "GamepadProviderIOS_60BeatNative.h"

id m_padDriver = nil;


GamepadProviderIOS_60Beat::GamepadProviderIOS_60Beat()
{
	Kill();
}

GamepadProviderIOS_60Beat::~GamepadProviderIOS_60Beat()
{
}

bool GamepadProviderIOS_60Beat::Init()
{
	LogMsg("Initting IOS_60Beat gamepad provider");
    [SBJoystick sharedInstance].loggingEnabled = YES;
    [SBJoystick sharedInstance].enabled = YES;
    
    assert(m_padDriver == 0);
    m_padDriver = [SBeatPadNative new];
    [m_padDriver Start];
    
    m_pPad = new GamepadIOS_60Beat;
    m_pPad->SetProvider(this);
    GetGamepadManager()->AddGamepad(m_pPad);
    return true; //success
}

void GamepadProviderIOS_60Beat::Kill()
{
	[SBJoystick sharedInstance].enabled = NO;
    [m_padDriver release];
    m_padDriver = nil;
}


void GamepadProviderIOS_60Beat::Update()
{
	[m_padDriver Update: m_pPad];
}

CL_Vec2f GamepadProviderIOS_60Beat::GetLeftStickPos()
{
    
    return CL_Vec2f([m_padDriver GetLeftStickPos].x,[m_padDriver GetLeftStickPos].y);
}

CL_Vec2f GamepadProviderIOS_60Beat::GetRightStickPos()
{
    
     return CL_Vec2f([m_padDriver GetRightStickPos].x,[m_padDriver GetRightStickPos].y);
}

