#include "PlatformPrecomp.h"
#include "TerrainMenu.h"
#include "Entity/EntityUtils.h"
#include "MainMenu.h"
#include "Irrlicht/IrrlichtManager.h"
#include "Component/FPSControlComponent.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;


void TerrainMenuOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());

	if (pEntClicked->GetName() == "Back")
	{
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		MainMenuCreate(pEntClicked->GetParent()->GetParent());

		GetIrrlichtManager()->ClearScene();

	}

	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

void TerrainInitScene()
{
	if (!GetIrrlichtManager()->GetDevice())
	{
		LogError("Error initializing Irrlicht");
		return;
	}
	GetIrrlichtManager()->GetDevice()->getTimer()->setTime(0);
	video::IVideoDriver* driver = GetIrrlichtManager()->GetDriver();
	scene::ISceneManager* smgr = GetIrrlichtManager()->GetScene();
	
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0,100.0f,0.5f);

	camera->setAspectRatio(GetScreenSizeXf()/GetScreenSizeYf());

	camera->setPosition(core::vector3df(2700*2,255*2,2600*2));
	camera->setTarget(core::vector3df(2397*2,343*2,2700*2));
	camera->setFarValue(22000.0f);

	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		(GetBaseAppPath()+"game/terrain-heightmap.bmp").c_str(),
		0,					// parent node
		-1,					// node id
		core::vector3df(0.f, 0.f, 0.f),		// position
		core::vector3df(0.f, 130.f, 0.f),		// rotation
		core::vector3df(40.f, 4.4f, 40.f),	// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		4,					// maxLOD
		scene::ETPS_9,				// patchSize
		4					// smoothFactor
		);

	if (!terrain)
	{
		LogError("Looks like we couldn't find the heightmap.  Resources not built or copied?");
		return;
	}
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);

	terrain->setMaterialTexture(0,
		 GetIrrlichtManager()->GetTexture("game/terrain-texture.rttex"));
	terrain->setMaterialTexture(1,
		 GetIrrlichtManager()->GetTexture("game/detailmap3.rttex"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 20.0f);


	// create triangle selector for the terrain	
	scene::ITriangleSelector* selector
		= smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);

	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		selector, camera, core::vector3df(60,100,60),
		core::vector3df(0,0,0),
		core::vector3df(0,50,0));
	
	((ISceneNodeAnimatorCollisionResponse*)anim)->setGravity(vector3df(0,-10,0));
	selector->drop();
	
	camera->addAnimator(anim);
	anim->drop();

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
		GetIrrlichtManager()->GetTexture("game/irrlicht2_up.rttex"),
		GetIrrlichtManager()->GetTexture("game/irrlicht2_dn.rttex"),
		GetIrrlichtManager()->GetTexture("game/irrlicht2_lf.rttex"),
		GetIrrlichtManager()->GetTexture("game/irrlicht2_rt.rttex"),
		GetIrrlichtManager()->GetTexture("game/irrlicht2_ft.rttex"),
		GetIrrlichtManager()->GetTexture("game/irrlicht2_bk.rttex"));
	//scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("../../media/skydome.jpg"),16,8,0.95f,2.0f);

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);


}

Entity * TerrainMenuCreate(Entity *pParentEnt)
{
	Entity *pBG = pParentEnt->AddEntity(new Entity("TerrainMenu"));

	AddFocusIfNeeded(pBG);

	Entity *pButtonEntity;
	pButtonEntity = CreateTextButtonEntity(pBG, "Back", iPhoneMapX(240),iPhoneMapY(290), "Back"); 

	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&TerrainMenuOnSelect);
	pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));


	//setup movement button
	Entity *pEnt = pBG->AddEntity(new Entity("controls", new FPSControlComponent));
	
	SlideScreen(pBG, true);

	TerrainInitScene();
	return pBG;
}

