#include "PlatformPrecomp.h"
#include "MainMenu.h"
#include "Entity/EntityUtils.h"
#include "DebugMenu.h"
#include "EnterNameMenu.h"
#include "ParticleTestMenu.h"


void MainMenuOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());

	if (pEntClicked->GetName() == "ParticleTest")
	{
		//slide it off the screen and then kill the whole menu tree
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		ParticleTestCreate(pEntClicked->GetParent()->GetParent());
	}

	if (pEntClicked->GetName() == "InputTest")
	{
		//slide it off the screen and then kill the whole menu tree
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		EnterNameMenuCreate(pEntClicked->GetParent()->GetParent());
	}

	if (pEntClicked->GetName() == "Debug")
	{
		//overlay the debug menu over this one
		pEntClicked->GetParent()->RemoveComponentByName("FocusInput");
		DebugMenuCreate(pEntClicked->GetParent());
	}


	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

Entity * MainMenuCreate(Entity *pParentEnt)
{
	//Entity *pBG = CreateOverlayEntity(pParentEnt, "MainMenu", "interface/summary_bg.rttex", 0,0);
	Entity *pBG = pParentEnt->AddEntity(new Entity);
	
	AddFocusIfNeeded(pBG);
	
	Entity *pButtonEntity;
	float x = 50;
	float y = 40;
	float ySpacer = 45;
	
	pButtonEntity = CreateTextButtonEntity(pBG, "ParticleTest", x, y, "ParticleTest"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MainMenuOnSelect);
	//pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));
	//BobEntity(pButtonEntity);

	pButtonEntity = CreateTextButtonEntity(pBG, "InputTest", x, y, "Input Test"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MainMenuOnSelect);
	//pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));

	pButtonEntity = CreateTextButtonEntity(pBG, "Debug", x, y, "Debug and music test"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MainMenuOnSelect);
	//pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));


	
	SlideScreen(pBG, true);

	return pBG;
}

