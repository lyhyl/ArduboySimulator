#include "PlatformPrecomp.h"
#include "Gamepad_iCade.h"


void ArcadeKeyboardControl::Setup(Gamepad_iCade *pPad,  char keyDown, char keyUp, int buttonID, eVirtualKeys vKeyToSend )
{
	if (buttonID != -1)
	{
		pPad->GetButton(buttonID)->m_virtualKey = vKeyToSend;
	}

	m_keyDown = keyDown;
	m_keyUp = keyUp;
	m_keyToSend = vKeyToSend;
}


Gamepad_iCade::Gamepad_iCade()
{
	
}

Gamepad_iCade::~Gamepad_iCade()
{
	Kill();
}

bool Gamepad_iCade::Init()
{
	m_axisUsedCount = 2;

	//setup our keys.. later, we may want to add other configs for other keyboard/based fake controllers, but this one is for the iCade:

	m_keys[KEY_ARCADE_BUTTON_LEFT].Setup(this, 'Y', 'T', 0, VIRTUAL_DPAD_BUTTON_LEFT );
	m_keys[KEY_ARCADE_BUTTON_RIGHT].Setup(this, 'J', 'N', 1, VIRTUAL_DPAD_BUTTON_RIGHT );
	m_keys[KEY_ARCADE_BUTTON_UP].Setup(this, 'U', 'F', 2, VIRTUAL_DPAD_BUTTON_UP );
	m_keys[KEY_ARCADE_BUTTON_DOWN].Setup(this, 'H', 'R', 3, VIRTUAL_DPAD_BUTTON_DOWN );

	//more buttons.. I'm using naming that matches the new ICG07 (triggers/buttons), but it works with the old iCade too, just keep in mind they
	//aren't really triggers etc

	m_keys[KEY_ARCADE_BUTTON_L].Setup(this, 'I', 'M', 4, VIRTUAL_DPAD_LBUTTON );
	m_keys[KEY_ARCADE_BUTTON_R].Setup(this, 'K', 'P', 5, VIRTUAL_DPAD_RBUTTON );
	m_keys[KEY_ARCADE_TRIGGER_L].Setup(this, 'O', 'G', 6, VIRTUAL_DPAD_LTRIGGER);
	m_keys[KEY_ARCADE_TRIGGER_R].Setup(this, 'L', 'V', 7, VIRTUAL_DPAD_RTRIGGER);

	m_buttonsUsedCount = KEY_ARCADE_BUTTON_COUNT;

	//how we handle the real directional pad to fake it into stick values
	m_keys[KEY_ARCADE_DIR_LEFT].Setup(this, 'A', 'Q', -1, VIRTUAL_KEY_DIR_LEFT);
	m_keys[KEY_ARCADE_DIR_RIGHT].Setup(this, 'D', 'C', -1, VIRTUAL_KEY_DIR_RIGHT);
	m_keys[KEY_ARCADE_DIR_UP].Setup(this, 'W', 'E', -1, VIRTUAL_KEY_DIR_UP);
	m_keys[KEY_ARCADE_DIR_DOWN].Setup(this, 'X', 'Z', -1, VIRTUAL_KEY_DIR_DOWN);

	GetBaseApp()->m_sig_raw_keyboard.connect(1, boost::bind(&Gamepad_iCade::OnRawKeyboardInput, this, _1));
	return true;
}


void Gamepad_iCade::Kill()
{

}


void Gamepad_iCade::OnRawKeyboardInput( VariantList *pVList )
{
	//0 = keycode, 1 = pressed or released
	bool bOnDown = pVList->Get(1).GetUINT32() != VIRTUAL_KEY_RELEASE;

	int c = pVList->Get(0).GetUINT32();

	//iCade and bluetooth keyboards don't actually have a release, they use other keys to fake it.

	if (bOnDown)
	{
		LogMsg("Gamepad_iCade got: %c (%d)", (char)c, c);
		
		//with only this many options, iteratoring through our keys for a match is not going to be a speed issue...
	

		for (int i=0; i < KEY_ARCADE_TOTAL_COUNT; i++)
		{
			if (m_keys[i].m_keyDown == c)
			{
				//button/dir pushed down

				if (i < KEY_ARCADE_BUTTON_COUNT)
				{
					//handle as button
					VariantList v;
					v.Get(0).Set(uint32(m_buttons[i].m_virtualKey));
					v.Get(1).Set(uint32(VIRTUAL_KEY_PRESS)); 
					v.Get(2).Set(uint32(m_id));
					m_sig_gamepad_buttons(&v);
				} else
				{
					//handle as dir
					m_keys[i].m_bPushed = true;
				}
			
			} else if (m_keys[i].m_keyUp == c)
			{
				//button/dir released
	
				if (i < KEY_ARCADE_BUTTON_COUNT)
				{
					//handle as button
					VariantList v;
					v.Get(0).Set(uint32(m_buttons[i].m_virtualKey));
					v.Get(1).Set(uint32(VIRTUAL_KEY_RELEASE)); 
					v.Get(2).Set(uint32(m_id));
					m_sig_gamepad_buttons(&v);
				} else
				{
					//handle as dir
					m_keys[i].m_bPushed = false;
				}

			}
		}
	}
}

void Gamepad_iCade::Update()
{

	int degrees = -1; 

	//up diagonals and up
	if (m_keys[KEY_ARCADE_DIR_UP].m_bPushed)
	{
		if (m_keys[KEY_ARCADE_DIR_RIGHT].m_bPushed)
		{
			degrees = 45;
		} else if (m_keys[KEY_ARCADE_DIR_LEFT].m_bPushed)
		{
			degrees = 315;
		} else
		{
			degrees = 0;
		}
	} else if (m_keys[KEY_ARCADE_DIR_DOWN].m_bPushed)
	{
		//down diagonals and down
		if (m_keys[KEY_ARCADE_DIR_RIGHT].m_bPushed)
		{
			degrees = 135;
		} else if (m_keys[KEY_ARCADE_DIR_LEFT].m_bPushed)
		{
			degrees = 225;
		} else
		{
			degrees = 180;
		}
	} else if (m_keys[KEY_ARCADE_DIR_LEFT].m_bPushed)
	{
		degrees = 270;
	} else if (m_keys[KEY_ARCADE_DIR_RIGHT].m_bPushed)
	{
		degrees = 90;
	} else
	{
		//no keys down
	}

	if (degrees == -1)
	{
		SetAxis(0, 0);
		SetAxis(1, 0);
	} else
	{
		//convert to vector
		SetAxis(0, (float)sin(DEG2RAD((float)degrees))); 
		SetAxis(1, (float)cos(DEG2RAD((float)degrees))); 
	}
	
	Gamepad::Update();
}