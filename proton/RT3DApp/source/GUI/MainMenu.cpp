#include "PlatformPrecomp.h"
#include "MainMenu.h"
#include "Entity/EntityUtils.h"
#include "DebugMenu.h"
#include "Mesh3DMenu.h"
#include "TerrainMenu.h"
#include "MapMenu.h"
#include "Map3Menu.h"

void MainMenuOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());


	if (pEntClicked->GetName() == "Debug")
	{
		//overlay the debug menu over this one
		pEntClicked->GetParent()->RemoveComponentByName("FocusInput");
		DebugMenuCreate(pEntClicked->GetParent());
	}

	if (pEntClicked->GetName() == "Mesh3D")
	{
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		Mesh3DMenuCreate(pEntClicked->GetParent()->GetParent());
	}

	if (pEntClicked->GetName() == "Terrain")
	{
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		TerrainMenuCreate(pEntClicked->GetParent()->GetParent());
	}

	if (pEntClicked->GetName() == "Map")
	{
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		MapMenuCreate(pEntClicked->GetParent()->GetParent());

	}

	if (pEntClicked->GetName() == "Map3")
	{
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		Map3MenuCreate(pEntClicked->GetParent()->GetParent());
	
	}

	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

Entity * MainMenuCreate(Entity *pParentEnt)
{
	Entity *pBG = CreateOverlayEntity(pParentEnt, "MainMenu", "interface/summary_bg.rttex", 0,0);
	
	AddFocusIfNeeded(pBG);
	
	Entity *pButtonEntity;
	float x = 50;
	float y = 40;
	float ySpacer = 45;
	
	pButtonEntity = CreateTextButtonEntity(pBG, "Debug", x, y, "Debug Options"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MainMenuOnSelect);
	//pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));

	pButtonEntity = CreateTextButtonEntity(pBG, "Mesh3D", x, y, "3D Animated Mesh"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MainMenuOnSelect);
	//pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));
	
	pButtonEntity = CreateTextButtonEntity(pBG, "Terrain", x, y, "3D Terrain"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MainMenuOnSelect);
	//pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));

	pButtonEntity = CreateTextButtonEntity(pBG, "Map", x, y, "Quake style BSP map"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MainMenuOnSelect);

	x = 330;
	y = 40;
	
	pButtonEntity = CreateTextButtonEntity(pBG, "Map3", x, y, "3D Scene"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MainMenuOnSelect);
	
	SlideScreen(pBG, true);

	return pBG;
}

