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
