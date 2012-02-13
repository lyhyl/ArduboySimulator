#include "PlatformPrecomp.h"
#include "MenuPurchase.h"
#include "Entity/EntityUtils.h"



void MenuPurchaseUpdate(VariantList *pVList)
{
	if (GetApp()->m_IAPManager.GetReturnState() != IAPManager::RETURN_STATE_NONE)
	{
		//we got a reply, kill the menu
		Entity *pEnt = GetEntityRoot()->GetEntityByName("MenuPurchase");
		KillEntity(pEnt);
		EnableAllButtonsEntity(pEnt->GetParent(), true);
		AddFocusIfNeeded(pEnt->GetParent(), false, 300);

		//get real result of the purchase attempt

		switch(GetApp()->m_IAPManager.GetReturnState())
		{
		case IAPManager::RETURN_STATE_ALREADY_PURCHASED:
			LogMsg("** ITEM BOUGHT - (Was already purchased, so free)");
			break;

		case IAPManager::RETURN_STATE_PURCHASED:
			LogMsg("** ITEM BOUGHT");
			break;

		case IAPManager::RETURN_STATE_FAILED:
			case IAPManager::RETURN_STATE_FINISHED:
			LogMsg("** FAILED TO BUY ITEM");
			break;
		}
	}

}

Entity * MenuPurchaseCreate(Entity *pParentEnt)
{
	Entity *pBG = CreateOverlayRectEntity(pParentEnt, CL_Vec2f(0,0), GetScreenSize(), MAKE_RGBA(0,0,0,170));

	pBG->SetName("MenuPurchase");

	AddFocusIfNeeded(pBG);
	Entity * pButtonEntity;

	pButtonEntity = CreateTextLabelEntity(pBG, "status", GetScreenSizeXf()/2, GetScreenSizeYf()/2, "Talking to server.....");
	SetAlignmentEntity(pButtonEntity, ALIGNMENT_CENTER);

	EntityComponent *pTextTyper = TypeTextLabelEntity(pButtonEntity);
	pTextTyper->GetVar("mode")->Set(uint32(TyperComponent::MODE_REPEAT));

	//we'll need to check every frame to see if we're done or not
	pBG->GetFunction("OnUpdate")->sig_function.connect(&MenuPurchaseUpdate);

	return pBG;
}

