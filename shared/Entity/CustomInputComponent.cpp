#include "PlatformPrecomp.h"
#include "CustomInputComponent.h"
#include "EntityUtils.h"

CustomInputComponent::CustomInputComponent()
{///****************
	SetName("CustomInput");
}

CustomInputComponent::~CustomInputComponent()
{
}

void CustomInputComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);

	//first setup to listen to keyboard messages
	GetParent()->GetFunction("OnInput")->sig_function.connect(1, boost::bind(&CustomInputComponent::OnInput, this, _1)); //used for keyboard keys on Win
	m_pDisabled = &GetVarWithDefault("disabled", uint32(0))->GetUINT32();

	m_pKeys = &GetVar("keys")->GetString(); //local to us
	m_pKeyCode = &GetVar("keycode")->GetUINT32(); //local to us

	//if keys and keycode are not set, it will be activated on "any key"
	//if either are set, it will only activate if those key/keys are hit.

	//you should attach to its "OnActivated" function to know whey a key is hit
}

void CustomInputComponent::OnRemove()
{
	EntityComponent::OnRemove();
}

void CustomInputComponent::OnActivated()
{
	VariantList v;
	GetFunction("OnActivated")->sig_function(&v);
}


void CustomInputComponent::OnInput( VariantList *pVList )
{

	if (*m_pDisabled == 1) return;
	//0 = message type, 1 = parent coordinate offset, 2 = char, 3 reserved for filtering control messages

	switch (eMessageType( int(pVList->Get(0).GetFloat())))
	{

	case MESSAGE_TYPE_GUI_CHAR:

		if (*m_pKeyCode != 0)
		{
			if (pVList->Get(2).GetUINT32() == *m_pKeyCode)
			{
				OnActivated();
				return;
			}
		
			if (m_pKeys->empty()) return;
		}
		char c = (char)pVList->Get(2).GetUINT32();




		if (m_pKeys->size() > 0)
		{

			bool bFound = false;
			for (unsigned int i=0; i < m_pKeys->size(); i++)
			{

				if (c == m_pKeys->at(i))
				{
					bFound = true;
					break;
				}

			}

			if (!bFound)
			{

#ifdef _DEBUG
				//LogMsg("Got key %c, ignoring", c);
#endif
				//none of the accepted keys are here.  Bye
				return;
			}
		}



		OnActivated();	
		//another method
		/*
		VariantList vList(vClickPos, GetParent());
		GetMessageManager()->CallEntityFunction(GetParent(), 1, "OnButtonSelected", &vList);  //sends a vec2 with position and this entity
		*/
		break;
	}	

}
