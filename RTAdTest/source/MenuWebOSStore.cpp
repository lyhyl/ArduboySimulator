#include "PlatformPrecomp.h"
#include "MenuWebOSStore.h"
#include "Entity/EntityUtils.h"
#include "MenuPurchase.h"
#include "MenuMain.h"

void MenuWebOSStoreOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();
	Entity *pMenu = pEntClicked->GetParent();

	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());

	DisableAllButtonsEntity(pEntClicked->GetParent());

	if (pEntClicked->GetName() == "BuySword")
	{
		MenuPurchaseCreate(pEntClicked->GetParent(), "COINS_200");
	}

	if (pEntClicked->GetName() == "BuyPoints")
	{
		MenuPurchaseCreate(pEntClicked->GetParent(), "COINS_500");
	}

	if (pEntClicked->GetName() == "back")
	{
		KillEntity(pMenu);
		MenuMainCreate(GetEntityRoot());
	}

	//GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

Entity * MenuWebOSStoreCreate(Entity *pParentEnt)
{

	Entity *pBG = pParentEnt->AddEntity(new Entity);
	AddFocusIfNeeded(pBG);
	Entity * pButtonEntity;

	float y = 70;
	float ySpacer = 70;

	pButtonEntity = CreateTextButtonEntity(pBG, "BuySword", 10, y+=ySpacer, "Buy 200 coins"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuWebOSStoreOnSelect);


	pButtonEntity = CreateTextButtonEntity(pBG, "BuyPoints", 10, y+=ySpacer, "Buy 500 coins"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuWebOSStoreOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "back", 10, y+=ySpacer, "Back"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuWebOSStoreOnSelect);


	return pBG;
}

