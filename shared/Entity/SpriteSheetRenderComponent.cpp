#include "SpriteSheetRenderComponent.h"
#include "Renderer/SpriteSheetSurface.h"
#include "BaseApp.h"

SpriteSheetRenderComponent::SpriteSheetRenderComponent() :
	m_pPos2d(NULL),
	m_pSize2d(NULL),
	m_pScale2d(NULL),
	m_pRotation(NULL),
	m_pColor(NULL),
	m_pColorMod(NULL),
	m_pAlpha(NULL),
	m_pTex(NULL),
	m_pFileName(NULL),
	m_pFrameName(NULL),
	m_pVisible(NULL),
	m_pFlipX(NULL),
	m_pFlipY(NULL)
{
	SetName("SpriteSheetRender");
}

SpriteSheetRenderComponent::~SpriteSheetRenderComponent()
{
}

void SpriteSheetRenderComponent::UpdateSizeVar()
{
	if (m_pTex && m_pTex->IsLoaded())
	{
		GetParent()->GetVar("size2d")->Set(m_pTex->GetFrameSize(*m_pFrameName) * (*m_pScale2d));
	}
}

void SpriteSheetRenderComponent::UpdateFrameSizeVar()
{
	if (m_pTex && m_pTex->IsLoaded())
	{
		GetVar("frameSize2d")->Set(m_pTex->GetFrameSize(*m_pFrameName));
	} else {
		GetVar("frameSize2d")->Set(0.0f, 0.0f);
	}
}

void SpriteSheetRenderComponent::OnFileNameChanged(Variant *pDataObject)
{
	m_pTex = GetResourceManager()->GetSurfaceResource<SpriteSheetSurface>(pDataObject->GetString());
	
	UpdateSizeVar();
	UpdateFrameSizeVar();
}

void SpriteSheetRenderComponent::OnFrameNameChanged(Variant *pDataObject)
{
	UpdateSizeVar();
	UpdateFrameSizeVar();
}

void SpriteSheetRenderComponent::OnScaleChanged(Variant *pDataObject)
{
	UpdateSizeVar();
}

void SpriteSheetRenderComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);

	// shared with the rest of the entity
	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	m_pSize2d = &GetParent()->GetVar("size2d")->GetVector2();
	m_pScale2d = &GetParent()->GetVarWithDefault("scale2d", Variant(1.0f, 1.0f))->GetVector2();
	m_pRotation = &GetParent()->GetVar("rotation")->GetFloat();
	m_pColor = &GetParent()->GetVarWithDefault("color", Variant(PURE_WHITE))->GetUINT32();
	m_pColorMod = &GetParent()->GetVarWithDefault("colorMod", Variant(PURE_WHITE))->GetUINT32();
	m_pAlpha = &GetParent()->GetVarWithDefault("alpha", Variant(1.0f))->GetFloat();
	m_pVisible = &GetParent()->GetVarWithDefault("visible", uint32(1))->GetUINT32();
	
	m_pFlipX = &GetVar("flipX")->GetUINT32();
	m_pFlipY = &GetVar("flipY")->GetUINT32();

	m_pFileName = &GetVar("fileName")->GetString();
	m_pFrameName = &GetVar("frameName")->GetString();
	
	GetVarWithDefault("frameSize2d", Variant(0.0f, 0.0f));

	GetVar("fileName")->GetSigOnChanged()->connect(boost::bind(&SpriteSheetRenderComponent::OnFileNameChanged, this, _1));
	GetVar("frameName")->GetSigOnChanged()->connect(boost::bind(&SpriteSheetRenderComponent::OnFrameNameChanged, this, _1));
	pEnt->GetVar("scale2d")->GetSigOnChanged()->connect(boost::bind(&SpriteSheetRenderComponent::OnScaleChanged, this, _1));
	
	// register ourselves to render if the parent does
	GetParent()->GetFunction("OnRender")->sig_function.connect(1, boost::bind(&SpriteSheetRenderComponent::OnRender, this, _1));
}

void SpriteSheetRenderComponent::OnRender(VariantList *pVList)
{
	if (*m_pVisible == 0) return;

	if (m_pTex && m_pTex->IsLoaded() && *m_pAlpha > 0.01)
	{
		if (m_pScale2d->x == 0 || m_pScale2d->y == 0) return;

		CL_Vec2f vFinalPos = pVList->m_variant[0].GetVector2() + *m_pPos2d;
		if (*m_pRotation == 0) // if rotation is enabled, we don't do this early exit check as it could be incorrect
		{
			if (vFinalPos.y < -m_pSize2d->y || vFinalPos.y > GetOrthoRenderSizeYf()) return;
		}
	
		unsigned int finalColor = ColorCombine(*m_pColor, *m_pColorMod, *m_pAlpha);
		if (GET_ALPHA(finalColor) == 0) return;

		CL_Vec2f vRotationPt(vFinalPos);
		vRotationPt += *m_pSize2d / 2;
		
		m_pTex->BlitFrame(vFinalPos.x, vFinalPos.y, *m_pFrameName, *m_pScale2d, finalColor, *m_pRotation, vRotationPt, *m_pFlipX != 0, *m_pFlipY != 0);
	}
}
