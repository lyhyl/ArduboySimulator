#include "PlatformPrecomp.h"
#include "MenuStore.h"
#include "Entity/EntityUtils.h"
#include "MenuPurchase.h"
#include "MenuMain.h"

void MenuStoreOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();
	Entity *pMenu = pEntClicked->GetParent();

	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());

	if (pEntClicked->GetName() != "back" && pEntClicked->GetName() != "sync")
	{
		GetApp()->GetAdManager()->TrackingLog("ClickedIAP", "item", pEntClicked->GetName());
	}

	if (pEntClicked->GetName() == "BuyTestPurchased")
	{
		GetApp()->m_IAPManager.BuyItem("android.test.purchased");
	}

	if (pEntClicked->GetName() == "BuyTestRefunded")
	{
		GetApp()->m_IAPManager.BuyItem("android.test.refunded");
	}

	if (pEntClicked->GetName() == "BuyTestCanceled")
	{
		GetApp()->m_IAPManager.BuyItem("android.test.canceled");
	}

	if (pEntClicked->GetName() == "BuySword")
	{
		GetApp()->m_IAPManager.BuyItem("com.rtsoft.rtadtest.sword");
	}

	if (pEntClicked->GetName() == "BuyShield")
	{
		GetApp()->m_IAPManager.BuyItem("com.rtsoft.rtadtest.shield");
	}

	if (pEntClicked->GetName() == "BuyPoints")
	{
		GetApp()->m_IAPManager.BuyItem("com.rtsoft.rtadtest.points");
	}
	
	if (pEntClicked->GetName() == "BuyGas")
	{
		GetApp()->m_IAPManager.BuyItem("gas", "developer data we want to embed for android");
	}

	if (pEntClicked->GetName() == "ConsumeGas")
	{
		GetApp()->m_IAPManager.ConsumeItem("gas");
		return;
	}

	if (pEntClicked->GetName() == "sync")
	{
		GetApp()->m_IAPManager.SyncPurchases();
		return;
	}

	if (pEntClicked->GetName() == "back")
	{
		KillEntity(pMenu);
		MenuMainCreate(GetEntityRoot());
		return;
	}

	DisableAllButtonsEntity(pEntClicked->GetParent());
	MenuPurchaseCreate(pEntClicked->GetParent());
	
	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

Entity * MenuStoreCreate(Entity *pParentEnt)
{
	Entity *pBG = pParentEnt->AddEntity(new Entity);
	AddFocusIfNeeded(pBG);
	Entity * pButtonEntity;

	float y = 20;
	float ySpacer = 55;

    if (GetEmulatedPlatformID() == PLATFORM_ID_ANDROID)
    {
        //Android has some extra ways to test
		pButtonEntity = CreateTextButtonEntity(pBG, "sync", 10, y+=ySpacer, "Sync purchases (android)"); 
		pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

		pButtonEntity = CreateTextButtonEntity(pBG, "BuyTestPurchased", 10, y+=ySpacer, "Buy android.test.purchased"); 
        pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

        pButtonEntity = CreateTextButtonEntity(pBG, "BuyTestRefunded", 10, y+=ySpacer, "Buy android.test.refunded"); 
        pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

        pButtonEntity = CreateTextButtonEntity(pBG, "BuyTestCanceled", 10, y+=ySpacer, "Buy android.test.canceled"); 
        pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

      }
     
    pButtonEntity = CreateTextButtonEntity(pBG, "BuySword", 10, y+=ySpacer, "Buy com.rtsoft.rtadtest.sword"); 
    pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);
   
	//pButtonEntity = CreateTextButtonEntity(pBG, "BuyShield", 10, y+=ySpacer, "Buy com.rtsoft.rtadtest.shield"); 
	//pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

  //  pButtonEntity = CreateTextButtonEntity(pBG, "BuyPoints", 10, y+=ySpacer, "Buy com.rtsoft.rtadtest.points (consumable/unmanaged)"); 
  //  pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);
    
	pButtonEntity = CreateTextButtonEntity(pBG, "BuyGas", 10, y+=ySpacer, "Buy gas (consumable/unmanaged)"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "ConsumeGas", 10, y+=ySpacer, "Consume gas (how android does consumables)"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

	//pButtonEntity = CreateTextButtonEntity(pBG, "BuyLife", 10, y+=ySpacer, "Buy com.rtsoft.rtadtest.life"); 
	//pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);


	pButtonEntity = CreateTextButtonEntity(pBG, "back", 10, y+=ySpacer, "Back"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);
	GetApp()->GetAdManager()->TrackingOnPageView();
	GetApp()->GetAdManager()->TrackingLog("ClickStore");


	return pBG;
}

