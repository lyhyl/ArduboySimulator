#ifndef EntityUtils_h__
#define EntityUtils_h__
//Misc junk for making the entity code cleaner
#include "BaseApp.h"
#include "Entity.h"

#include "Entity/OverlayRenderComponent.h"
#include "Entity/TextRenderComponent.h"
#include "Entity/TouchHandlerComponent.h"
#include "Entity/TouchStripComponent.h"
#include "Entity/Button2DComponent.h"
#include "Entity/FocusInputComponent.h"
#include "Entity/FocusUpdateComponent.h"
#include "Entity/FocusRenderComponent.h"
#include "Entity/EntityUtils.h"
#include "Entity/InterpolateComponent.h"
#include "Entity/ProgressBarComponent.h"
#include "Entity/UnderlineRenderComponent.h"
#include "Entity/TapSequenceDetectComponent.h"
#include "Entity/HTTPComponent.h"
#include "Entity/TyperComponent.h"
#include "Entity/RectRenderComponent.h"
#include "Entity/ScrollComponent.h"
#include "Entity/ScrollBarRenderComponent.h"
#include "Entity/TextBoxRenderComponent.h"
#include "Entity/InputTextRenderComponent.h"
#include "Entity/FilterInputComponent.h"
#include "Entity/TrailRenderComponent.h"
#include "Entity/RenderClipComponent.h"
#include "Entity/SelectButtonWithCustomInputComponent.h"
#include "Entity/SliderComponent.h"
#include "Entity/EmitVirtualKeyComponent.h"

Entity * CreateTextLabelEntity(Entity *pParentEnt, string name, float x, float y, string text);
Entity * CreateTextButtonEntity(Entity *pParentEnt, string name, float x, float y, string text, bool bUnderline = true);
Entity * CreateOverlayEntity(Entity *pParentEnt, string name, string fileName, float x, float y);
Entity * CreateOverlayButtonEntity(Entity *pParentEnt, string name, string fileName, float x, float y);
Entity * CreateOverlayRectEntity(Entity *pParent, CL_Rectf posAndBoundsRect, uint32 color, RectRenderComponent::eVisualStyle style = RectRenderComponent::STYLE_NORMAL);
Entity * CreateOverlayRectEntity(Entity *pParent, CL_Vec2f vPos, CL_Vec2f vBounds, uint32 color, RectRenderComponent::eVisualStyle style = RectRenderComponent::STYLE_NORMAL);
Entity * CreateButtonHotspot(Entity *pParentEnt, string name, CL_Vec2f vPos, CL_Vec2f vBounds, Button2DComponent::eButtonStyle buttonStyle = Button2DComponent::BUTTON_STYLE_CLICK_ON_TOUCH);
Entity * CreateTextBoxEntity(Entity *pParent, string entName, CL_Vec2f vPos, CL_Vec2f vTextAreaSize, string msg, float scale = 1.0f);
Entity * CreateInputTextEntity(Entity *pParentEnt, string name, float x, float y, string text, float sizeX=0, float sizeY=0);

void DisableAllButtonsEntity(Entity *pEnt, bool bRecursive = true);
void SlideScreen(Entity *pEnt, bool bIn, int speedMS = 500, int delayToStartMS = 0);
void SlideScreenVertical(Entity *pEnt, bool bIn, int speedMS = 500, int delayToStartMS = 0);
void BobEntity(Entity *pEnt, float bobAmount = 3);
void OneTimeBobEntity(Entity *pEnt, float bobAmount = -10, int delayBeforeBob = 0, int durationMS = 100);
void AddFocusIfNeeded(Entity *pEnt, bool bAlsoLinkMoveMessages = false, int delayInputMS = 0); //add input, think, and render focuses to an entity (if they don't exist)
void AddInputMovementFocusIfNeeded(Entity *pEnt);
void RemoveFocusIfNeeded(Entity *pEnt); //remove input, think, and render focuses from an entity
void FadeOutEntity(Entity *pEnt, bool bRecursive=true, int timeMS=300, int delayBeforeFadingMS = 0);
void FadeOutAndKillEntity(Entity *pEnt, bool bRecursive = true, int timeMS=300, int delayBeforeFadingMS = 0);
void FadeOutAndKillChildrenEntities(Entity *pEnt, int timeMS=300, int delayBeforeFadingMS = 0);
void FadeInEntity(Entity *pEnt, bool bRecursive=true, int timeMS=300, int delayBeforeFadingMS = 0);
void KillEntity(Entity *pEnt, int timeMS = 0);
EntityComponent * PulsateColorEntity(Entity *pEnt, bool bRecursive, unsigned int color);
EntityComponent * TypeTextLabelEntity(Entity *pEnt, int delayBeforeActionMS = 0, uint32 textTypeSpeedMS = 50); //modifies an existing textlabel to 'type' itself out.  Deletes any pre-existing Typer effect

EntityComponent * ZoomToPositionFromThisOffsetEntity(Entity *pEnt, CL_Vec2f vPos, unsigned int speedMS, eInterpolateType interpolateType = INTERPOLATE_SMOOTHSTEP,  int delayBeforeActionMS = 0);
EntityComponent * ZoomToPositionEntity(Entity *pEnt, CL_Vec2f vPos, unsigned int speedMS, eInterpolateType interpolateType = INTERPOLATE_SMOOTHSTEP,  int delayBeforeActionMS = 0);
EntityComponent * ZoomFromPositionEntity(Entity *pEnt, CL_Vec2f vPos, unsigned int speedMS, eInterpolateType interpolateType = INTERPOLATE_SMOOTHSTEP,  int delayBeforeActionMS = 0);
EntityComponent * ZoomToPositionOffsetEntity(Entity *pEnt, CL_Vec2f vPos, unsigned int speedMS, eInterpolateType interpolateType = INTERPOLATE_SMOOTHSTEP,  int delayBeforeActionMS = 0);

//like above, but allows more than one to be layered instead of assuming you want to ignore the previous one
EntityComponent * ZoomToPositionEntityMulti(Entity *pEnt, CL_Vec2f vPos, unsigned int speedMS, eInterpolateType interpolateType = INTERPOLATE_SMOOTHSTEP,  int delayBeforeActionMS = 0);

EntityComponent * MorphToVec2Entity(Entity *pEnt, string targetVar, CL_Vec2f vTargetSize, unsigned int speedMS, eInterpolateType interpolateType = INTERPOLATE_SMOOTHSTEP,  int delayBeforeActionMS = 0);
EntityComponent * MorphToSizeEntity(Entity *pEnt, CL_Vec2f vTargetSize, unsigned int speedMS, eInterpolateType interpolateType = INTERPOLATE_SMOOTHSTEP,  int delayBeforeActionMS = 0);
void MorphToColorEntity(Entity *pEnt, bool bRecursive, int timeMS, unsigned int color, int delayBeforeActionMS=0, bool bAllowMultipleAtOnce = false);
EntityComponent * MorphToFloatEntity(Entity *pEnt, string targetVar, float target, unsigned int speedMS, eInterpolateType interpolateType = INTERPOLATE_SMOOTHSTEP,  int delayBeforeActionMS = 0);
EntityComponent * MorphToFloatComponent(EntityComponent *pTargetComp, string targetVar, float target, unsigned int speedMS, eInterpolateType interpolateType = INTERPOLATE_SMOOTHSTEP,  int delayBeforeActionMS = 0);

void FlashStartEntity(Entity *pEnt, int flashSpeedMS = 250);
void FlashStopEntity(Entity *pEnt);
void FlashOnceEntity(Entity *pEnt, int flashSpeedMS);

EntityComponent * SetupAnimEntity(Entity *pEnt, uint32 frameCountX, uint32 frameCountY = 1, int curFrameX = -1, int curFrameY = -1); //-1 means don't set it
void AnimateEntity(Entity *pEnt, int startFrame, int endFrame, int animSpeedMS, InterpolateComponent::eOnFinish type, int delayToStartMS);
void AnimateStopEntity(Entity *pEnt, int delayToStartMS);
void AnimateStopEntityAndSetFrame(Entity *pEnt, int delayToStartMS, int frameX, int frameY);
void AnimateEntitySetMirrorMode(Entity *pEnt, bool flipX, bool flipY);

void ScaleEntity(Entity *pEnt, float scaleStart, float scaleEnd, int timeMS, int delayBeforeStartingMS = 0); //send -1 for scaleStart to use current scale as start

void FadeEntity(Entity *pEnt, bool bRecursive, float alpha, int timeMS, int delayBeforeFadingMS = 0, bool bAllowMultipleFadesActiveAtOnce = false);
void FadeScreen( Entity *pParent, float defaultStartAlpha, float targetAlpha, int fadeDurationMS, bool bDeleteWhenDone);

CL_Rectf MeasureEntityAndChildren(Entity *pEnt, bool bFirst = true);

//simple helpers to set parameters
void SetupTextEntity(Entity *pEnt, eFont fontID, float scale=0); //0 means no change
void SetAlignmentEntity(Entity *pEnt, eAlignment align);
void SetTouchPaddingEntity(Entity *pEnt, CL_Rectf padding);
EntityComponent * SetButtonVisualStyleEntity(Entity *pEnt, Button2DComponent::eVisualStyle style);
EntityComponent * SetButtonStyleEntity(Entity *pEnt, Button2DComponent::eButtonStyle style);
void SetButtonClickSound(Entity *pEnt, string fileName);
void SetAlphaEntity(Entity *pEnt, float alpha);
void PreloadKeyboard(OSMessage::eParmKeyboardType keyboardType=OSMessage::PARM_KEYBOARD_TYPE_ASCII);
void SendFakeInputMessageToEntity(Entity *pEnt, eMessageType msg, CL_Vec2f vClickPos);
void SetButtonRepeatDelayMS(Entity *pEnt, uint32 delayMS);
void LightBarOnChange(VariantList *pVList);
void SetupLightBarSelect(Entity *pBG, string entNamePrefix, int defaultOption, uint32 color);
void ResizeScrollBounds(VariantList *pVList);
void DisableHorizontalScrolling(Entity *pEnt);

EntityComponent * DisableComponentByName(Entity *pEnt, const string &compName, int delayBeforeActionMS=0); //applicable to components that check a var named "disabled", like Button2D or InputTextRender
EntityComponent * EnableComponentByName(Entity *pEnt, const string &compName, int delayBeforeActionMS=0); //applicable to components that check a var named "disabled", like Button2D or InputTextRender
CL_Vec2f ConvertEntityClickToScreenCoords(CL_Vec2f pt, Entity *pEnt);
void GetUsedTextures(vector<string> &usedTextures, Entity *pEnt);
void DestroyUnusedTextures();
bool EntityRetinaRemapIfNeeded(Entity *pEnt, bool bAdjustPosition = true, bool bAdjustScale = false, bool bApplyToIpadAlso = true, bool bPerserveAspectRatio = false); //moves and scale entities to fit a retina screen like an iphone screen
void EntitySetScaleBySize(Entity *pEnt, CL_Vec2f vDestSize);
EntityComponent * AddHotKeyToButton(Entity *pEnt, uint32 keycode);
EntityComponent * MakeButtonEmitVirtualGameKey(Entity *pEnt, uint32 keycode);
EntityComponent * CreateSlider(Entity *pBG, float x, float y, float sizeX, string buttonFileName, string left, string middle, string right);
void AdjustGUIElementForWindowView(Entity *pEnt, CL_Rectf r, float rotation);
void ManuallySetAlignmentEntity(Entity *pEnt, eAlignment alignment); //takes into account rotation as in screen rotation
bool IsDisabledEntity(Entity *pEnt);
void RemovePaddingEntity(Entity *pEnt);
void FakeClickAnEntity(Entity *pEnt); //good for pretending to click a Button2D programically or something
#endif // EntityUtils_h__