#ifndef MainMenu_h__
#define MainMenu_h__

#include "App.h"


Entity * MainMenuCreate(Entity *pParentEnt);
void AddKeyBinding(EntityComponent *pComp, string name, uint32 inputcode, uint32 outputcode);
void FakeClickToButton(Entity *pChosenButton, int32 delayMS);
#endif // MainMenu_h__