#include "PlatformPrecomp.h"
#include "MenuPurchase.h"
#include "Entity/EntityUtils.h"

void KillStoreMenu()
{
	//we got a reply, kill the menu
	Entity *pEnt = GetEntityRoot()->GetEntityByName("PurchaseMenu");

	if (!pEnt || pEnt->GetTaggedForDeletion())
	{
		return; //already handled this
	}

	KillEntity(pEnt);

	EnableAllButtonsEntity(pEnt->GetParent(), true);
	AddFocusIfNeeded(pEnt->GetParent(), false, 300);

	
}

void StorePurchaseOnCancel(VariantList *pVList)
{
	LogMsg("Hit cancel button");

	KillStoreMenu();
	GetApp()->m_IAPManager.Reset();
}


void MenuPurchaseUpdate(VariantList *pVList)
{
	if (GetApp()->m_IAPManager.GetReturnState() != IAPManager::RETURN_STATE_NONE)
	{
		KillStoreMenu();

		//get real result of the purchase attempt

		switch(GetApp()->m_IAPManager.GetReturnState())
		{
		case IAPManager::RETURN_STATE_ALREADY_PURCHASED:
			LogMsg("** ITEM %s BOUGHT - (Was already purchased, so free)", GetApp()->m_IAPManager.GetItemID().c_str());
			break;

		case IAPManager::RETURN_STATE_PURCHASED:
			LogMsg("** ITEM %s BOUGHT", GetApp()->m_IAPManager.GetItemID().c_str());
			break;

		case IAPManager::RETURN_STATE_FAILED:
			LogMsg("** FAILED TO BUY ITEM %s", GetApp()->m_IAPManager.GetItemID().c_str());
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

	pBG->SetName("PurchaseMenu");

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


	//oh, let's make a cancel button pop up after a bit as well:

	int delayBeforeCancelButtonPopsUpMS = 5000;

	LogMsg("Initting cancel button");
	pButtonEntity = CreateTextButtonEntity(pBG, "store_cancel", (GetScreenSizeXf()/2) , (GetScreenSizeYf()/2)-iPhoneMapY(50), "Cancel");
	pButtonEntity->GetFunction("OnButtonSelected")->sig_function.connect(&StorePurchaseOnCancel);
	SetAlphaEntity(pButtonEntity, 0.0f);
	FadeInEntity(pButtonEntity, false, 500, delayBeforeCancelButtonPopsUpMS);
	DisableAllButtonsEntity(pButtonEntity);
	EnableAllButtonsEntity(pButtonEntity, false, delayBeforeCancelButtonPopsUpMS);
	SetAlignmentEntity(pButtonEntity, ALIGNMENT_CENTER);
	SetupTextEntity(pButtonEntity, FONT_SMALL);

	return pBG;
}

