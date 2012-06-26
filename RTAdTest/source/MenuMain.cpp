#include "PlatformPrecomp.h"
#include "MenuMain.h"
#include "Entity/EntityUtils.h"
#include "MenuStore.h"
#include "MenuTapjoy.h"
#include "MenuTest.h"

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

	if (pEntClicked->GetName() == "run_tests")
	{
		MenuTestCreate(pMenu->GetParent());
		KillEntity(pMenu);
	}

	AdProvider *pProvider = GetApp()->m_adManager.GetProviderByType(AD_PROVIDER_CHARTBOOST);

	if (pEntClicked->GetName() == "chartboost_interstitial")
	{
		if (pProvider)
		{
			pProvider->ShowInterstitial();
		} else
		{
			LogMsg("Chartboost not active.  (RT_CHARTBOOST_ENABLED not defined) ");
		}
        return;
	}

	if (pEntClicked->GetName() == "chartboost_more_games")
	{
		if (pProvider)
		{
			pProvider->ShowMoreApps();
		} else
		{
			LogMsg("Chartboost not active.  (RT_CHARTBOOST_ENABLED not defined) ");
		}
        return;
	}

	DisableAllButtonsEntity(pEntClicked->GetParent());
	//GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

Entity * MenuMainCreate(Entity *pParentEnt)
{
	
	Entity *pBG = pParentEnt->AddEntity(new Entity);
	AddFocusIfNeeded(pBG);
	Entity * pButtonEntity;

	float x = 10;
	float y = 10;
	float spacerY = 50;

	pButtonEntity = CreateTextButtonEntity(pBG, "store", x, y +=spacerY, "Test IAP (android, iOS, webOS)"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuMainOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "tapjoy", x, y +=spacerY, "Test Tapjoy ads (android)"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuMainOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "chartboost_interstitial", x, y +=spacerY, "Request Chartboost Interstitial (iOS, Android)"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuMainOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "chartboost_more_games", x, y +=spacerY, "Request Chartboost More Games Dialog (iOS, Android)"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuMainOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "run_tests", x, y +=spacerY, "Run tests");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuMainOnSelect);

	GetApp()->GetAdManager()->TrackingOnPageView();

	return pBG;
}

