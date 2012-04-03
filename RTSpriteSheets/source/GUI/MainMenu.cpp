#include "MainMenu.h"
#include "BasicSpriteFramesScreen.h"
#include "BasicSpriteAnimationScreen.h"

#include "App.h"

#include "Entity/EntityUtils.h"
#include "Entity/CustomInputComponent.h"

void MainMenuOnSelect(VariantList *pVList)
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	Entity *pEntClickedParent = pVList->m_variant[1].GetEntity()->GetParent();
	pEntClickedParent->RemoveComponentByName("FocusInput");
	SlideScreen(pEntClickedParent, false);
	GetMessageManager()->CallEntityFunction(pEntClickedParent, 500, "OnDelete", NULL);

	if (pEntClicked->GetName() == "BasicSpriteFrames")
	{
		BasicSpriteFramesScreenCreate(pEntClickedParent->GetParent());
	} else if (pEntClicked->GetName() == "BasicSpriteAnimation")
	{
		BasicSpriteAnimationScreenCreate(pEntClickedParent->GetParent());
	}
}

Entity * MainMenuCreate(Entity *pParentEnt)
{
	Entity *pBG = pParentEnt->AddEntity(new Entity("Background"));
	
	AddFocusIfNeeded(pBG);
	
	//for android, so the back key (or escape on windows) will quit out of the game
	EntityComponent *pComp = pBG->AddComponent(new CustomInputComponent);
	//tell the component which key has to be hit for it to be activated
	pComp->GetFunction("OnActivated")->sig_function.connect(1, boost::bind(&App::OnExitApp, GetApp(), _1));
	pComp->GetVar("keycode")->Set(uint32(VIRTUAL_KEY_BACK));

	Entity *pButtonEntity;
	float x = 50;
	float y = 40;
	float yDistance = 30;

	pButtonEntity = CreateTextButtonEntity(pBG, "BasicSpriteFrames", x, y, "Basic sprite frames");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MainMenuOnSelect);
	y += yDistance;

	pButtonEntity = CreateTextButtonEntity(pBG, "BasicSpriteAnimation", x, y, "Basic sprite animation");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MainMenuOnSelect);

	SlideScreen(pBG, true);

	return pBG;
}
