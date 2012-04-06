#include "BasicSpriteFramesScreen.h"
#include "GUICommon.h"

#include "Entity/EntityUtils.h"
#include "Entity/SpriteAnimationUtils.h"

Entity* CreateSpriteEntity(Entity* pParentEnt, float x, float y, const std::string& frameName)
{
	const float OriginLeft = 30.0f;
	const float OriginTop = 30.0f;
	const float HorizontalDistance = 120.0f;
	const float VerticalDistance = 120.0f;

	Entity *ent = pParentEnt->AddEntity(new Entity);
	SetPos2DEntity(ent, CL_Vec2f(OriginLeft + x * HorizontalDistance, OriginTop + y * VerticalDistance));
	SpriteAnimationUtils::SetupAnimationEntity(ent, "game/eateat_bird.anim", "Jump");
	return ent;
}

Entity* BasicSpriteFramesScreenCreate(Entity *pParentEnt)
{
	Entity *pBG = CreateOverlayEntity(pParentEnt, "background", "interface/menu_bg.rttex", 0, 0);
	AddFocusIfNeeded(pBG);

	float x = 80;
	float y = 290;

	Entity* pButtonEntity = CreateTextButtonEntity(pBG, "Back", x, y, "Back");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&GoBack);
	AddHotKeyToButton(pButtonEntity, VIRTUAL_KEY_BACK); //for android's back button, or escape key in windows

	CreateSpriteEntity(pBG, 0, 0, "0");

	Entity* sprite = CreateSpriteEntity(pBG, 1, 0, "1");
	SetScale2DEntity(sprite, CL_Vec2f(1.2f, 0.8f));

	sprite = CreateSpriteEntity(pBG, 0, 1, "2");
	sprite->GetVar("rotation")->Set(45.0f);

	sprite = CreateSpriteEntity(pBG, 1, 1, "3");
	sprite->GetVar("color")->Set(MAKE_RGBA(0, 255, 255, 255));
	sprite->GetVar("colorMod")->Set(MAKE_RGBA(200, 200, 200, 255));
	sprite->GetVar("alpha")->Set(0.75f);

	sprite = CreateSpriteEntity(pBG, 2, 0, "0");
	EntityComponent* comp = sprite->GetComponentByName("SpriteAnimationRender");
	comp->GetVar("flipX")->Set(uint32(1));

	sprite = CreateSpriteEntity(pBG, 3, 0, "0");
	comp = sprite->GetComponentByName("SpriteAnimationRender");
	comp->GetVar("flipY")->Set(uint32(1));

	sprite = CreateSpriteEntity(pBG, 2, 1, "2");
	comp = sprite->GetComponentByName("SpriteAnimationRender");
	comp->GetVar("flipX")->Set(uint32(1));
	comp->GetVar("flipY")->Set(uint32(1));

	SlideScreen(pBG, true, 500);
	return pBG;
}
