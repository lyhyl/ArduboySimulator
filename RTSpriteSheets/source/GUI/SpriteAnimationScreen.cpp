#include "SpriteAnimationScreen.h"
#include "GUICommon.h"

#include "Entity/SpriteAnimationRenderComponent.h"
#include "Entity/EntityUtils.h"
#include "Entity/SpriteAnimationUtils.h"

void AnimationButtonClicked(VariantList *pVList)
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	Entity *birdEntity = pEntClicked->GetParent()->GetEntityByName("Bird");

	SpriteAnimationUtils::StartAnimationEntity(birdEntity, pEntClicked->GetName(), 500);
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

Entity* SpriteAnimationScreenCreate(Entity *pParentEnt)
{
	Entity *pBG = CreateCommonControls(pParentEnt);

	float x = 80;
	float y = 250;

	Entity* pButtonEntity = CreateTextButtonEntity(pBG, "Bow", x, y, "Bow");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&AnimationButtonClicked);

	x = 300;
	pButtonEntity = CreateTextButtonEntity(pBG, "Jump", x, y, "Jump");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&AnimationButtonClicked);

	const float OriginLeft = 240.0f;
	const float OriginTop = 320.0f;

	Entity *ent = pBG->AddEntity(new Entity("Bird"));
	SetPos2DEntity(ent, CL_Vec2f(OriginLeft, OriginTop));
	SetAlignmentEntity(ent, ALIGNMENT_DOWN_CENTER);
	SpriteAnimationUtils::SetupAnimationEntity(ent, "game/eateat_bird.anim", "Bow");

	return pBG;
}

class AnimationRandomizerComponent : public EntityComponent
{
public:
	AnimationRandomizerComponent()
	{
		animations.push_back("RollEyes");
		animations.push_back("MouthWinkle");
	}

	virtual ~AnimationRandomizerComponent()	{}

	virtual void OnAdd(Entity *pEnt)
	{
		EntityComponent::OnAdd(pEnt);

		GetFunction("StartNewAnimation")->sig_function.connect(boost::bind(&AnimationRandomizerComponent::StartRandomAnimation, this, _1));
	}

private:
	vector<string> animations;

	void StartRandomAnimation(VariantList *pVList)
	{
		SpriteAnimationUtils::StartAnimationEntity(GetParent(), animations.at(Random(animations.size())), 1000);
		GetMessageManager()->CallComponentFunction(this, 2000, "StartNewAnimation");
	}
};

Entity* MoreSpriteAnimationsScreenCreate(Entity *pParentEnt)
{
	Entity *pBG = CreateCommonControls(pParentEnt);

	const float OriginLeft = 240.0f;
	const float OriginTop = 160.0f;

	Entity *ent = pBG->AddEntity(new Entity("Face"));
	SetPos2DEntity(ent, CL_Vec2f(OriginLeft, OriginTop));
	SetAlignmentEntity(ent, ALIGNMENT_CENTER);
	SpriteAnimationUtils::SetupAnimationEntity(ent, "game/face.anim", "RollEyes");

	EntityComponent* randomizer = ent->AddComponent(new AnimationRandomizerComponent);
	GetMessageManager()->CallComponentFunction(randomizer, 1500, "StartNewAnimation");

	return pBG;
}
