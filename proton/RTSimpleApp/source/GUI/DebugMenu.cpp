#include "PlatformPrecomp.h"
#include "DebugMenu.h"
#include "Entity/EntityUtils.h"

void DebugMenuOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	LogMsg("Clicked %s entity", pEntClicked->GetName().c_str());

	if (pEntClicked->GetName() == "FPS")
	{
		GetBaseApp()->SetFPSVisible(!GetBaseApp()->GetFPSVisible());
	}
	if (pEntClicked->GetName() == "music_on")
	{
		GetAudioManager()->Play("audio/techno.mp3", true, true);
	}

	if (pEntClicked->GetName() == "music_off")
	{
		GetAudioManager()->StopMusic();
	}

	if (pEntClicked->GetName() == "Back")
	{
		//slide it off the screen and then kill the whole menu tree
		RemoveFocusIfNeeded(pEntClicked->GetParent());
		SlideScreen(pEntClicked->GetParent(), false);
		AddFocusIfNeeded(pEntClicked->GetParent()->GetParent());
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		//DebugMenuCreate(GetParent()
	}

	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}


Entity * DebugMenuCreate(Entity *pParentEnt)
{
	Entity *pBG = CreateOverlayEntity(pParentEnt, "DebugMenu", "interface/summary_bg.rttex", 0,0);
	AddFocusIfNeeded(pBG);

	Entity *pButtonEntity;
	float x = 80;
	float y = 100;
	float ySpacer = 40;

	pButtonEntity = CreateTextButtonEntity(pBG, "FPS", x, y, "Toggle FPS Display"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&DebugMenuOnSelect);
	//pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));
	
	pButtonEntity = CreateTextButtonEntity(pBG, "music_on", x, y, "Play bg music"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&DebugMenuOnSelect);
	//pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));
	
	pButtonEntity = CreateTextButtonEntity(pBG, "music_off", x, y, "Stop bg music"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&DebugMenuOnSelect);
	//pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));


	pButtonEntity = CreateTextButtonEntity(pBG, "Back", x, y, "Back"); y += ySpacer;
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&DebugMenuOnSelect);
	//pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));
	
	SlideScreen(pBG, true, 500);
	

	return pBG;
}

