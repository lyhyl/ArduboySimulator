//  ***************************************************************
//  ArcadeInputComponent - Creation date: 11/8/2010
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2010 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

//This component reads the android trackball data, and keyboard data, and converts them to direction up/down key events.

//The way it does it with the trackball is it detects a direction, then keeps the key "down" until no movement is detected after 100
//ms or so.


//Check RTLooneyLadders for a working example.

/*

Example of usage:

EntityComponent *pComp = pIcon->AddComponent(new ArcadeInputComponent);
AddKeyBinding(pComp, "Up", VIRTUAL_KEY_DIR_UP, VIRTUAL_KEY_DIR_UP);
AddKeyBinding(pComp, "Unlock", 'U', 'U');

//route messages to the entity you want to have them:
GetBaseApp()->m_sig_arcade_input.connect(pBG->GetFunction("OnArcadeInput")->sig_function);
//connect that to a function
pBG->GetShared()->GetFunction("OnArcadeInput")->sig_function.connect(&OnSelectInput);




//another example, but overriding things so the messages are not broadcast through m_sig_arcade, but to a specific entity

//this example is how I did it from inside a component:
EntityComponent *pComp = GetParent()->AddComponent(new ArcadeInputComponent);
//these arrow keys will be triggered by the keyboard, if applicable
AddKeyBinding(pComp, "Fire", VIRTUAL_KEY_CONTROL, VIRTUAL_KEY_GAME_FIRE);
pComp->GetFunction("SetOutputEntity")->sig_function(&VariantList(GetParent())); //redirect its messages to our parent entity, will call OnArcadeInput
GetParent()->GetFunction("OnArcadeInput")->sig_function.connect(1, boost::bind(&VehicleControlComponent::OnArcadeInput, this, _1));	

*/


#ifndef ArcadeInputComponent_h__
#define ArcadeInputComponent_h__

#include "Entity/Component.h"
#include "Entity/Entity.h"

enum eMoveButtonDir
{
	MOVE_BUTTON_DIR_LEFT = 0,
	MOVE_BUTTON_DIR_RIGHT,
	MOVE_BUTTON_DIR_UP,
	MOVE_BUTTON_DIR_DOWN,
	
	MOVE_BUTTON_DIR_COUNT
};


class MoveButtonState
{
public:

	MoveButtonState()
	{
		m_bIsDown = false;
		m_releaseTimer = 0;
	}

	void SetKeyType(eVirtualKeys type)
	{
		m_keyType = type;
	}

	void ReleaseIfNeeded(boost::signal<void (VariantList*)> *pCustomSignal);
	void OnPress(int releaseTime, boost::signal<void (VariantList*)> *pCustomSignal);

	void Update(boost::signal<void (VariantList*)> *pCustomSignal);
	void OnPressToggle(bool bDown, boost::signal<void (VariantList*)> *pCustomSignal);
	bool m_bIsDown;
	unsigned int m_releaseTimer;
	eVirtualKeys m_keyType;
};


class ArcadeKeyBind
{
public:

	ArcadeKeyBind()
	{
		m_inputkeycode = 0;
		m_outputkeycode = 0;
		m_requireShift = false;
		m_requireAlt = false;
		m_requireCtrl = false;
	}

	string m_name;
	uint32 m_inputkeycode;
	uint32 m_outputkeycode;
	
	//unused right now
	bool m_requireShift;
	bool m_requireAlt;
	bool m_requireCtrl;

};

typedef list<ArcadeKeyBind> ArcadeBindList;
class ArcadeInputComponent: public EntityComponent
{

public:
	ArcadeInputComponent();
	virtual ~ArcadeInputComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();


	bool GetDirectionKeysAsVector(CL_Vec2f *pVecOut);

	enum TrackballMode
	{
		TRACKBALL_MODE_WALKING, //default, suitable for moving around a character, sends up/down notifications to mimic arrow key directions
		TRACKBALL_MODE_MENU_SELECTION //suitable for moving up/down menus, sends an up/down notification every 1.0f of notches are reached
	};

	void OnRawKeyboard(VariantList *pVList); //can call or signal bind to this to feed it more keys if needed

private:

	void OnUpdate(VariantList *pVList);
	void OnTrackball(VariantList *pVList);
	void AddKeyBinding(VariantList *pVList);
	void ActivateBinding(ArcadeKeyBind *pBind, bool bDown);
	void OnTrackballModeChanged(Variant *pVar);
	void OnCustomOutputRemoved(Entity *pEnt);
	void SetOutput(VariantList *pVList);
	CL_Vec2f *m_pPos2d;
	/*
	CL_Vec2f *m_pSize2d;
	CL_Vec2f *m_pScale2d;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	float *m_pAlpha;
	uint32 *m_pAlignment;
	float *m_pRotation; //in degrees
	*/

	MoveButtonState m_buttons[MOVE_BUTTON_DIR_COUNT];
	ArcadeBindList m_bindings;
	uint32 *m_pTrackballMode;
	CL_Vec2f m_trackball;
	
	boost::signal<void (VariantList*)> *m_customSignal; //if not null, messages will be sent here
};

void AddKeyBinding(EntityComponent *pComp, string name, uint32 inputcode, uint32 outputcode);

#endif // ArcadeInputComponent_h__