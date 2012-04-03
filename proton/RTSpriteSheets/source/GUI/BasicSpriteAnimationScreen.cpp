#include "BasicSpriteAnimationScreen.h"
#include "GUICommon.h"

#include "Renderer/SpriteSheetSurface.h"
#include "Entity/SpriteSheetRenderComponent.h"
#include "Entity/StringChooserComponent.h"
#include "Entity/EntityUtils.h"

EntityComponent* CreateSpriteSheetAnimation(Entity* pParent, float x, float y)
{
	Entity *ent = pParent->AddEntity(new Entity);
	SetPos2DEntity(ent, CL_Vec2f(x, y));
	EntityComponent *spriteSheetComp = ent->AddComponent(new SpriteSheetRenderComponent);
	spriteSheetComp->GetVar("fileName")->Set("game/eateat_bird.rttex");

	EntityComponent *stringChanger = ent->AddComponent(new StringChooserComponent);
	stringChanger->GetVar("strings")->Set("Bird/Walk/0:Bird/Walk/1:Bird/Walk/2:Bird/Walk/3:Bird/Walk/4:Bird/Walk/5:Bird/Walk/6:Bird/Walk/7:Bird/Walk/8");
	stringChanger->GetVar("component_name")->Set("SpriteSheetRender");
	stringChanger->GetVar("var_name")->Set("frameName");

	EntityComponent *animator = ent->AddComponent(new InterpolateComponent);
	animator->GetVar("component_name")->Set("StringChooser");
	animator->GetVar("var_name")->Set("index");
	animator->GetVar("target")->Set(uint32(9));
	animator->GetVar("interpolation")->Set(uint32(INTERPOLATE_LINEAR));
	animator->GetVar("on_finish")->Set(uint32(InterpolateComponent::ON_FINISH_REPEAT));
	animator->GetVar("duration_ms")->Set(uint32(1000));

	return spriteSheetComp;
}

Entity* BasicSpriteAnimationScreenCreate(Entity *pParentEnt)
{
	SpriteSheetSurface *spriteSheet = GetResourceManager()->GetSurfaceResource<SpriteSheetSurface>("game/eateat_bird.rttex");
	if (spriteSheet == NULL) {
		LogError("Resource manager didn't return a sprite sheet surface.");
		return NULL;
	}

	spriteSheet->AddFrame("Bird/Walk/0", CL_Rect(0, 98, 0+58, 98+98));
	spriteSheet->AddFrame("Bird/Walk/1", CL_Rect(196, 0, 196+59, 0+98));
	spriteSheet->AddFrame("Bird/Walk/2", CL_Rect(0, 196, 0+58, 196+98));
	spriteSheet->AddFrame("Bird/Walk/3", CL_Rect(0, 294, 0+58, 294+98));
	spriteSheet->AddFrame("Bird/Walk/4", CL_Rect(0, 392, 0+58, 392+98));
	spriteSheet->AddFrame("Bird/Walk/5", CL_Rect(255, 0, 255+59, 0+98));
	spriteSheet->AddFrame("Bird/Walk/6", CL_Rect(314, 0, 314+58, 0+98));
	spriteSheet->AddFrame("Bird/Walk/7", CL_Rect(372, 0, 372+58, 0+98));
	spriteSheet->AddFrame("Bird/Walk/8", CL_Rect(58, 294, 58+57, 294+98));

	Entity *pBG = CreateOverlayEntity(pParentEnt, "background", "interface/menu_bg.rttex", 0, 0);
	AddFocusIfNeeded(pBG);

	float x = 80;
	float y = 290;

	Entity* pButtonEntity = CreateTextButtonEntity(pBG, "Back", x, y, "Back");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&GoBack);
	AddHotKeyToButton(pButtonEntity, VIRTUAL_KEY_BACK); //for android's back button, or escape key in windows

	const float OriginLeft = 70.0f;
	const float OriginTop = 100.0f;
	const float HorizontalDistance = 120.0f;

	CreateSpriteSheetAnimation(pBG, OriginLeft, OriginTop);
	CreateSpriteSheetAnimation(pBG, OriginLeft + HorizontalDistance, OriginTop)->GetVar("flipX")->Set(uint32(1));

	SlideScreen(pBG, true, 500);
	return pBG;
}
