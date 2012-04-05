#include "SpriteAnimationScreen.h"
#include "GUICommon.h"

#include "Entity/SpriteAnimationRenderComponent.h"
#include "Entity/EntityUtils.h"
#include "Entity/SpriteAnimationUtils.h"

void StartAnimation(VariantList *pVList)
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	Entity *birdEntity = pEntClicked->GetParent()->GetEntityByName("Bird");

	SpriteAnimationUtils::StartAnimationEntity(birdEntity, pEntClicked->GetName(), 500);
}

void CreateSpriteAnimation(Entity* pParent, float x, float y)
{
	Entity *ent = pParent->AddEntity(new Entity("Bird"));
	SetPos2DEntity(ent, CL_Vec2f(x, y));
	SetAlignmentEntity(ent, ALIGNMENT_DOWN_CENTER);

	SpriteAnimationUtils::SetupAnimationEntity(ent, "game/eateat_bird.anim", "Bow");
}

Entity* SpriteAnimationScreenCreate(Entity *pParentEnt)
{
	Entity *pBG = CreateOverlayEntity(pParentEnt, "background", "interface/menu_bg.rttex", 0, 0);
	AddFocusIfNeeded(pBG);

	float x = 80;
	float y = 290;

	Entity* pButtonEntity = CreateTextButtonEntity(pBG, "Back", x, y, "Back");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&GoBack);
	AddHotKeyToButton(pButtonEntity, VIRTUAL_KEY_BACK); //for android's back button, or escape key in windows

	y = 250;
	pButtonEntity = CreateTextButtonEntity(pBG, "Bow", x, y, "Bow");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&StartAnimation);

	x = 300;
	pButtonEntity = CreateTextButtonEntity(pBG, "Jump", x, y, "Jump");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&StartAnimation);

	const float OriginLeft = 240.0f;
	const float OriginTop = 320.0f;

	CreateSpriteAnimation(pBG, OriginLeft, OriginTop);

	SlideScreen(pBG, true, 500);
	return pBG;
}
