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
                
            default:
                LogMsg("HUH?! Unknown IAP return state");
		}

		GetApp()->m_IAPManager.Reset();
	}

}

void MenuPurchaseStart(VariantList *pVList)
{
	string item = pVList->Get(0).GetString();
	GetApp()->m_IAPManager.BuyItem(item);
}

Entity * MenuPurchaseCreate(Entity *pParentEnt, string itemToBuy)
{
	Entity *pBG = CreateOverlayRectEntity(pParentEnt, CL_Vec2f(0,0), GetScreenSize(), MAKE_RGBA(0,0,0,170));

	pBG->SetName("MenuPurchase");

	AddFocusIfNeeded(pBG);
	Entity * pButtonEntity;

	pButtonEntity = CreateTextLabelEntity(pBG, "status", GetScreenSizeXf()/2, GetScreenSizeYf()/2, "Please wait.....");
	SetAlignmentEntity(pButtonEntity, ALIGNMENT_CENTER);

	EntityComponent *pTextTyper = TypeTextLabelEntity(pButtonEntity);
	pTextTyper->GetVar("mode")->Set(uint32(TyperComponent::MODE_REPEAT));

	pBG->GetFunction("StartPurchase")->sig_function.connect(&MenuPurchaseStart);
	if (!itemToBuy.empty())
	{
		//let's start the real purchase in a bit, after we draw the "waiting" screen, useful for webOS
		//which is a blocking operation, otherwise you get hit with this big pause right when you tap
		//the button which feels unresponsive
		VariantList vList(itemToBuy);
		GetMessageManager()->CallEntityFunction(pBG, 1000, "StartPurchase", &vList);
	}

	//we'll need to check every frame to see if we're done or not
	pBG->GetFunction("OnUpdate")->sig_function.connect(&MenuPurchaseUpdate);

	return pBG;
}

