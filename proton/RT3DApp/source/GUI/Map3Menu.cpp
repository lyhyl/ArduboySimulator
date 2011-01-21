#include "PlatformPrecomp.h"
#include "Map3Menu.h"
#include "Entity/EntityUtils.h"
#include "MainMenu.h"
#include "Irrlicht/IrrlichtManager.h"
#include "Component/FPSControlComponent.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;


void Map3MenuOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());

	if (pEntClicked->GetName() == "Back")
	{
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		MainMenuCreate(pEntClicked->GetParent()->GetParent());
		GetIrrlichtManager()->ClearScene();
		DestroyUnusedTextures();
	}

	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

void Map3InitScene()
{
	if (!GetIrrlichtManager()->GetDevice())
	{
		LogError("Error initializing Irrlicht");
		return;
	}
	GetIrrlichtManager()->GetDevice()->getTimer()->setTime(0);
	video::IVideoDriver* driver = GetIrrlichtManager()->GetDriver();
	scene::ISceneManager* smgr = GetIrrlichtManager()->GetScene();
	IrrlichtDevice * device = GetIrrlichtManager()->GetDevice();

	smgr->getParameters()->setAttribute(B3D_LOADER_IGNORE_MIPMAP_FLAG, true);


	scene::IAnimatedMesh* mesh = smgr->getMesh((GetBaseAppPath()+"game/house_scene/house_scene.b3d").c_str());
	scene::IMeshSceneNode* node = 0;


	if (mesh)
	{
		//node = smgr->addOctTreeSceneNode(mesh->getMesh(0), 0, -1, 128);
		driver->setMinHardwareBufferVertexCount(64);

		mesh->setHardwareMappingHint(scene::EHM_STATIC);
		node = smgr->addMeshSceneNode(mesh->getMesh(0));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
	} else
	{
		LogError("Aborting, can't load mesh");
		return;
	}

	node->setScale(core::vector3df(30,30,30));
	
	scene::ITriangleSelector* selector = 0;
	scene::ICameraSceneNode* camera =
	smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, 0, 0, 0, true, 3.f);
	camera->setPosition(core::vector3df(242,72,-620)); //for prey level
	camera->setTarget(core::vector3df(-70,30,-160));

	if (node)
	{
			
		/*
		selector = smgr->createOctTreeTriangleSelector(
		node->getMesh(), node, 8);
		node->setTriangleSelector(selector);
		*/

		//custom collision mesh


		scene::IAnimatedMesh* colmesh = smgr->getMesh((GetBaseAppPath()+"game/house_scene/house_scene_col.b3d").c_str());
		scene::IMeshSceneNode* colnode = smgr->addMeshSceneNode(colmesh->getMesh(0));


		colnode->setScale(core::vector3df(30,30,30));

		//selector = smgr->createOctTreeTriangleSelector(
		//		colmesh, colnode, 128);
		selector = smgr->createTriangleSelector(colmesh, node);

		node->setTriangleSelector(selector);
		colnode->setVisible(false);

		if (selector)
		{
			scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
				selector, camera, core::vector3df(30,50,30),
				core::vector3df(0,-10,0), core::vector3df(0,30,0));
			selector->drop(); // As soon as we're done with the selector, drop it.
			camera->addAnimator(anim);
			anim->drop();  // And likewise, drop the animator when we're done referring to it.
		}


	}

	//	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0,100.0f,0.5f);

	float fov = float(GetPrimaryGLX())/ float(GetPrimaryGLY());
	LogMsg("Setting fov to %.2f", fov);
	camera->setAspectRatio(fov);
	camera->setFOV((120 * M_PI / 360.0f));
  
	//camera->setFarValue(700.0f);
	/*
	The mouse cursor needs not be visible, so we hide it via the
	irr::IrrlichtDevice::ICursorControl.
	*/

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

Entity * Map3MenuCreate(Entity *pParentEnt)
{
	Entity *pBG = pParentEnt->AddEntity(new Entity("Map3Menu"));

	AddFocusIfNeeded(pBG);

	Entity *pButtonEntity;
	pButtonEntity = CreateTextButtonEntity(pBG, "Back", iPhoneMapX(240),iPhoneMapY(290), "Back"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&Map3MenuOnSelect);
	pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));


	//setup movement button
	Entity *pEnt = pBG->AddEntity(new Entity("controls", new FPSControlComponent));

	SlideScreen(pBG, true);

	Map3InitScene();
	return pBG;
}

