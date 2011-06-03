#include "PlatformPrecomp.h"
#include "Mesh3DMenu.h"
#include "Entity/EntityUtils.h"
#include "MainMenu.h"
#include "Irrlicht/IrrlichtManager.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;


void Mesh3DMenuOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
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

void Mesh3DInitScene()
{

	if (!GetIrrlichtManager()->GetDevice())
	{
		LogError("Error initializing Irrlicht");
		return;
	}
	GetIrrlichtManager()->GetDevice()->getTimer()->setTime(0);

	scene::IAnimatedMeshSceneNode* anode = 0;

	IAnimatedMesh* mesh;
	
	ISceneManager *pScene = GetIrrlichtManager()->GetScene();
	
	
	//mesh = pScene->getMesh( (GetBaseAppPath() + "game/sydney.md2").c_str());
	video::IVideoDriver* driver = GetIrrlichtManager()->GetDriver();

	mesh = pScene->getMesh( (GetBaseAppPath() + "game/squirrel.x").c_str());
	//mesh = pScene->getMesh( (GetBaseAppPath() + "game/ninja.b3d").c_str());

	IAnimatedMeshSceneNode* node =pScene->addAnimatedMeshSceneNode( mesh );
	
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		//node->setMD2Animation(scene::EMAT_STAND);
		//driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	
		//node->setMaterialTexture( 0,GetIrrlichtManager()->GetDriver()->getTexture( (GetBaseAppPath() + "game/sydney.bmp").c_str() ));
		//node->setMaterialTexture( 0,GetIrrlichtManager()->GetDriver()->getTexture( (GetBaseAppPath() + "game/sydney.rttex").c_str() ));
		node->setMaterialTexture( 0,GetIrrlichtManager()->GetDriver()->getTexture( (GetBaseAppPath() + "game/squirrel_skin.rttex").c_str() ));
		//driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

		/*
		node->setFrameLoop(0, 31);
		node->setAnimationSpeed(30);
		*/
		node->setRotation(core::vector3df(0,-30,0));
	}

	
	/*
	IAnimatedMeshSceneNode* pCloneNode = (IAnimatedMeshSceneNode*)node->clone(0, pScene);
	pCloneNode->setRotation(core::vector3df(0,-30,0));
	pCloneNode->setCurrentFrame(4);
	pCloneNode->setPosition(pCloneNode->getPosition() + vector3df(2,0,0));
	//m_pScene->addCameraSceneNode(0, vector3df(-2,6,-7));
*/
	ICameraSceneNode *pCam = pScene->addCameraSceneNode(0, vector3df(0,2,-10));
	float fov = float(GetPrimaryGLX())/ float(GetPrimaryGLY());
	LogMsg("Setting fov to %.2f", fov);
	pCam->setAspectRatio(fov);
	pCam->setFOV((120 * M_PI / 360.0f));

}

Entity * Mesh3DMenuCreate(Entity *pParentEnt)
{
	Entity *pBG = pParentEnt->AddEntity(new Entity("Mesh3DMenu"));

	AddFocusIfNeeded(pBG);

	//Entity *pParticleEnt = pBG->AddEntity(new Entity(new Mesh3DMenuComponent));

	Entity *pButtonEntity;
	pButtonEntity = CreateTextButtonEntity(pBG, "Back", iPhoneMapX(240),iPhoneMapY(290), "Back"); 

	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&Mesh3DMenuOnSelect);
	pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));

	SlideScreen(pBG, true);
	
	Mesh3DInitScene();
	return pBG;
}

