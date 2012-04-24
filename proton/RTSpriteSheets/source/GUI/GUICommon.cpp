#include "GUICommon.h"

#include "Entity/EntityUtils.h"
#include "MainMenu.h"

void GoBack(VariantList *pVList)
{
	Entity *pEntClickedParent = pVList->m_variant[1].GetEntity()->GetParent();
	pEntClickedParent->RemoveComponentByName("FocusInput");
	SlideScreen(pEntClickedParent, false);
	GetMessageManager()->CallEntityFunction(pEntClickedParent, 500, "OnDelete", NULL);
	MainMenuCreate(pEntClickedParent->GetParent());
}

Entity* CreateCommonControls(Entity *pParentEnt)
{
	Entity *pBG = CreateOverlayEntity(pParentEnt, "background", "interface/menu_bg.rttex", 0, 0);
	AddFocusIfNeeded(pBG);

	Entity* pButtonEntity = CreateTextButtonEntity(pBG, "Back", 80, 290, "Back");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&GoBack);
	AddHotKeyToButton(pButtonEntity, VIRTUAL_KEY_BACK); //for android's back button, or escape key in windows

	SlideScreen(pBG, true, 500);

	return pBG;
}
