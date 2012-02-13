#include "PlatformPrecomp.h"
#include "MenuAdWait.h"
#include "Entity/EntityUtils.h"
#include "App.h"

void MenuAdWaitOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());


	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

void MenuAdWaitUpdate(VariantList *pVList)
{
	if (GetApp()->m_adManager.GetReturnState() != AdManager::RETURN_STATE_WAITING)
	{
		//we got a reply, kill the menu
		Entity *pEnt = GetEntityRoot()->GetEntityByName("MenuAdWait");
		KillEntity(pEnt);
		EnableAllButtonsEntity(pEnt->GetParent(), true);
		AddFocusIfNeeded(pEnt->GetParent(), false, 300);

		//get real result of the AdWait attempt

		switch(GetApp()->m_adManager.GetReturnState())
		{
		case AdManager::RETURN_STATE_ERROR:
			LogMsg("** Error - %s", GetApp()->m_adManager.GetLastErrorString().c_str());
			break;

		case AdManager::RETURN_STATE_SUCCESS:
			LogMsg("Successfully finished transaction.");
			break;
		}
	}
}

Entity * MenuAdWaitCreate(Entity *pParentEnt)
{
	Entity *pBG = CreateOverlayRectEntity(pParentEnt, CL_Vec2f(0,0), GetScreenSize(), MAKE_RGBA(0,0,0,170));

	pBG->SetName("MenuAdWait");

	AddFocusIfNeeded(pBG);
	Entity * pButtonEntity;

	pButtonEntity = CreateTextLabelEntity(pBG, "status", GetScreenSizeXf()/2, GetScreenSizeYf()/2, "Talking to server.....");
	SetAlignmentEntity(pButtonEntity, ALIGNMENT_CENTER);

	EntityComponent *pTextTyper = TypeTextLabelEntity(pButtonEntity);
	pTextTyper->GetVar("mode")->Set(uint32(TyperComponent::MODE_REPEAT));

	//we'll need to check every frame to see if we're done or not
	pBG->GetFunction("OnUpdate")->sig_function.connect(&MenuAdWaitUpdate);

	return pBG;
}

