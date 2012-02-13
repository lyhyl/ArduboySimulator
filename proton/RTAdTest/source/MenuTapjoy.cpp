#include "PlatformPrecomp.h"
#include "MenuTapjoy.h"
#include "Entity/EntityUtils.h"
#include "MenuMain.h"
#include "MenuAdWait.h"

void MenuTapjoyOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	Entity *pMenu = pEntClicked->GetParent();

	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());

	if (pEntClicked->GetName() == "banner on")
	{
		GetApp()->m_adManager.SetTapjoyAdVisible(true);
	}

	if (pEntClicked->GetName() == "banner off")
	{
		GetApp()->m_adManager.SetTapjoyAdVisible(false);
	}

	if (pEntClicked->GetName() == "banner refresh")
	{
		GetApp()->m_adManager.CacheTapjoyAd();
	}


	if (pEntClicked->GetName() == "offerwall")
	{
		GetApp()->m_adManager.OpenTapjoyOfferWall();
	}
	if (pEntClicked->GetName() == "get")
	{
		GetApp()->m_adManager.GetTapPointsFromServer();
	}

	if (pEntClicked->GetName() == "spend")
	{
		GetApp()->m_adManager.ModifyTapPoints(-120);
		DisableAllButtonsEntity(pEntClicked->GetParent());
		MenuAdWaitCreate(pEntClicked->GetParent());
	}

	if (pEntClicked->GetName() == "watchad")
	{
		//GetApp()->m_adManager.OpenTapjoyWatchAd();

		DisableAllButtonsEntity(pEntClicked->GetParent());
		MenuAdWaitCreate(pEntClicked->GetParent());
	}

	if (pEntClicked->GetName() == "back")
	{
		KillEntity(pMenu);
		MenuMainCreate(GetEntityRoot());
	}

GetApp()->m_adManager.m_tapPointVariant.ClearConnections();
//	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

void OnTapPointsChanged(Variant *pVar)
{
	LogMsg("Tappoints changed to %d", pVar->GetINT32());
}

Entity * MenuTapjoyCreate(Entity *pParentEnt)
{

	/*
	VariantList vList((int32)50);
	GetApp()->m_AdManager.m_sig_tappoints_awarded(&vList); //called when awarded tap points
*/

	GetApp()->m_adManager.SetTapjoyAdVisible(true);

	Entity *pBG = pParentEnt->AddEntity(new Entity);
	AddFocusIfNeeded(pBG);
	Entity * pButtonEntity;

	CreateTextLabelEntity(pBG, "points", 10, 25, "Tap Points: "+GetApp()->m_adManager.GetPointsString());
	
	GetApp()->m_adManager.m_tapPointVariant.GetSigOnChanged()->connect(&OnTapPointsChanged);
	
	float y = 70;
	float ySpacer = 70;

	pButtonEntity = CreateTextButtonEntity(pBG, "banner on", 10, y+=ySpacer, "Banner on"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuTapjoyOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "banner off", 10, y+=ySpacer, "Banner off"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuTapjoyOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "banner refresh", 10, y+=ySpacer, "Banner Refresh"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuTapjoyOnSelect);
	
	pButtonEntity = CreateTextButtonEntity(pBG, "offerwall", 10, y+=ySpacer, "Open Offer Wall"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuTapjoyOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "spend", 10, y+=ySpacer, "Spend 120 tap points"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuTapjoyOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "get", 10, y+=ySpacer, "Get current tap points"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuTapjoyOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "watchad", 10, y+=ySpacer, "Watch ad"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuTapjoyOnSelect);

	pButtonEntity = CreateTextButtonEntity(pBG, "back", 10, y+=ySpacer, "Back"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuTapjoyOnSelect);


	return pBG;
}

