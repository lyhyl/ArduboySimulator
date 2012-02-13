#include "PlatformPrecomp.h"
#include "MenuMain.h"
#include "Entity/EntityUtils.h"
#include "MenuStore.h"
#include "MenuTapjoy.h"

void MenuMainOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	Entity *pMenu = pEntClicked->GetParent();

	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());

	if (pEntClicked->GetName() == "store")
	{
		MenuStoreCreate(pMenu->GetParent());
		KillEntity(pMenu);
	}

	
	if (pEntClicked->GetName() == "tapjoy")
	{
		MenuTapjoyCreate(pMenu->GetParent());
		KillEntity(pMenu);
	}

	DisableAllButtonsEntity(pEntClicked->GetParent());
	//GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

Entity * MenuMainCreate(Entity *pParentEnt)
{
	
	Entity *pBG = pParentEnt->AddEntity(new Entity);
	AddFocusIfNeeded(pBG);
	Entity * pButtonEntity;

	pButtonEntity = CreateTextButtonEntity(pBG, "store", 10, 30, "Test android market in app billing (android)"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuMainOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "tapjoy", 10, 100, "Test Tapjoy integration stuff (android)"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuMainOnSelect);


	return pBG;
}

