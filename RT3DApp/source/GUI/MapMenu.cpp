#include "PlatformPrecomp.h"
#include "MapMenu.h"
#include "Entity/EntityUtils.h"
#include "MainMenu.h"
#include "Irrlicht/IrrlichtManager.h"
#include "Component/FPSControlComponent.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;


void MapMenuOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());

	if (pEntClicked->GetName() == "Back")
	{
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		MainMenuCreate(pEntClicked->GetParent()->GetParent());
		GetIrrlichtManager()->ClearScene();
		
#ifdef WIN32
	//TODO: Make this less hacky
		GetIrrlichtManager()->GetDevice()->getFileSystem()->changeWorkingDirectoryTo( "..\\..");
#else
		GetIrrlichtManager()->GetDevice()->getFileSystem()->changeWorkingDirectoryTo(GetBaseAppPath().c_str());
#endif
	}

	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

void MapInitScene()
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
	//TO LOAD THE QUAKE LEVEL


	device->getFileSystem()->changeWorkingDirectoryTo( (GetBaseAppPath()+"game/quake").c_str());
	//scene::IAnimatedMesh* mesh = smgr->getMesh("maps/20kdm2.bsp");
	scene::IAnimatedMesh* mesh = smgr->getMesh("maps/20kdm2.bsp");
	scene::IMeshSceneNode* node = 0;
	if (mesh)
	{
		//driver->setMinHardwareBufferVertexCount(8);
		mesh->setHardwareMappingHint(scene::EHM_STATIC);
		//node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024); //for oct tree stuff, actually slower with current level
		node = smgr->addMeshSceneNode(mesh->getMesh(0));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
	}
	scene::ITriangleSelector* selector = 0;
	
	bool bNoVerticalMovement = true;	
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, 0, 0, 0, bNoVerticalMovement, 3.f);
	camera->setPosition(core::vector3df(50,50,-60)); //quake level
	camera->setTarget(core::vector3df(-70,30,-160));
	camera->setFarValue(32000);
	node->setPosition(core::vector3df(-1350,-130,-1400));

		if (node)
		{
			//node->setPosition(core::vector3df(-1350,-130,-1400));

			selector = smgr->createOctreeTriangleSelector(node->getMesh(), node, 64);
			//selector = smgr->createTriangleSelector(node->getMesh(), node);

			node->setTriangleSelector(selector);
			// We're not done with this selector yet, so don't drop it.
		}

		if (selector)
		{
			scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
				selector, camera, core::vector3df(30,50,30),
				core::vector3df(0,-10,0), core::vector3df(0,30,0));
			selector->drop(); // As soon as we're done with the selector, drop it.
			camera->addAnimator(anim);
			anim->drop();  // And likewise, drop the animator when we're done referring to it.
		}
			
		camera->setAspectRatio(GetScreenSizeXf()/GetScreenSizeYf());
		camera->setFOV((120 * M_PI / 360.0f));




	return;

}

Entity * MapMenuCreate(Entity *pParentEnt)
{
	Entity *pBG = pParentEnt->AddEntity(new Entity("MapMenu"));

	AddFocusIfNeeded(pBG);

	Entity *pButtonEntity;
	pButtonEntity = CreateTextButtonEntity(pBG, "Back", iPhoneMapX(240),iPhoneMapY(290), "Back"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MapMenuOnSelect);
	pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));

	//setup movement button
	Entity *pEnt = pBG->AddEntity(new Entity("controls", new FPSControlComponent));

	SlideScreen(pBG, true);
	MapInitScene();
	return pBG;
}

