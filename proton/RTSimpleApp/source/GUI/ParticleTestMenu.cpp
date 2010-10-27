#include "PlatformPrecomp.h"
#include "ParticleTestMenu.h"
#include "Entity/EntityUtils.h"
#include "MainMenu.h"
#include "Component/ParticleTestComponent.h"

void ParticleTestOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());

	if (pEntClicked->GetName() == "Back")
	{
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		MainMenuCreate(pEntClicked->GetParent()->GetParent());
	}

	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

Entity * ParticleTestCreate(Entity *pParentEnt)
{
	Entity *pBG = CreateOverlayEntity(pParentEnt, "ParticleTest", "interface/summary_bg.rttex", 0,0);
	AddFocusIfNeeded(pBG);

	Entity *pParticleEnt = pBG->AddEntity(new Entity(new ParticleTestComponent));
	
	Entity *pButtonEntity;
	pButtonEntity = CreateTextButtonEntity(pBG, "Back", 240, 290, "Back"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&ParticleTestOnSelect);
	pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));

	SlideScreen(pBG, true);

	
	return pBG;
}

