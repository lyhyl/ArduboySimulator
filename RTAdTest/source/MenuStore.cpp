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

	if (pEntClicked->GetName() == "back")
	{
		KillEntity(pMenu);
		MenuMainCreate(GetEntityRoot());
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

	float y = 70;
	float ySpacer = 70;


	pButtonEntity = CreateTextButtonEntity(pBG, "BuyTestPurchased", 10, y+=ySpacer, "Buy android.test.purchased"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "BuyTestRefunded", 10, y+=ySpacer, "Buy android.test.refunded"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "BuyTestCanceled", 10, y+=ySpacer, "Buy android.test.canceled"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "BuySword", 10, y+=ySpacer, "Buy com.rtsoft.rtadtest.sword"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

	//pButtonEntity = CreateTextButtonEntity(pBG, "BuyShield", 10, 190, "Buy com.rtsoft.rtadtest.shield"); 
	//pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

	//pButtonEntity = CreateTextButtonEntity(pBG, "BuyLife", 10, y+=ySpacer, "Buy com.rtsoft.rtadtest.life"); 
	//pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "BuyPoints", 10, y+=ySpacer, "Buy com.rtsoft.rtadtest.points (unmanaged)"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "back", 10, y+=ySpacer, "Back"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuStoreOnSelect);


	return pBG;
}

