#include "PlatformPrecomp.h"
#include "PhysicsHelloMenu.h"
#include "Entity/EntityUtils.h"
#include "MainMenu.h"
#include "Irrlicht/IrrlichtManager.h"
#include "Component/FPSControlComponent.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;


IRigidBody* shootCube(const irr::core::vector3df &scale, irr::f32 mass, irr::core::stringc textureFile = "game/rockwall.rttex")
{

	scene::ICameraSceneNode* camera = GetIrrlichtManager()->GetScene()->getActiveCamera();

	video::IVideoDriver* driver = GetIrrlichtManager()->GetDriver();
	scene::ISceneManager* smgr = GetIrrlichtManager()->GetScene();
	IrrlichtDevice * device = GetIrrlichtManager()->GetDevice();
	irrBulletWorld * world = GetIrrlichtManager()->GetBulletWorld();

	irr::core::vector3df pos(camera->getPosition().X,camera->getPosition().Y,camera->getPosition().Z);

	irr::scene::ISceneNode *Node = device->getSceneManager()->addCubeSceneNode(1.0f);
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, GetIrrlichtManager()->GetTexture(textureFile.c_str()));

	//Node->setMaterialFlag(irr::video::EMF_WIREFRAME, drawWireFrame);

	ICollisionShape *shape = new IBoxShape(Node, mass, true);

	//shape->setMargin(0.01);

	IRigidBody *body = world->addRigidBody(shape);

	irr::core::vector3df rot = camera->getRotation();
	irr::core::matrix4 mat;
	mat.setRotationDegrees(rot);
	irr::core::vector3df forwardDir(irr::core::vector3df(mat[8],mat[9],mat[10]) * 120);

	body->setLinearVelocity(forwardDir);
	//body->setActivationState(EAS_DISABLE_DEACTIVATION);

	body->setDamping(0.2,0.2);
	body->setFriction(0.6f);

	//ICollisionObjectAffectorDelete *deleteAffector = new ICollisionObjectAffectorDelete(4000);
	//body->addAffector(deleteAffector);

	body->getAttributes()->addBool("collide", true);

	//ICollisionObjectAffectorAttract* affector = new ICollisionObjectAffectorAttract(irr::core::vector3df(0,1000,0), 2500);
	//ICollisionObjectAffectorAttract* affector = new ICollisionObjectAffectorAttract(device->getSceneManager()->getActiveCamera(), 2500);
	//body->addAffector(affector);

	return body;
}


void PhysicsHelloMenuOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
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

	if (pEntClicked->GetName() == "Shoot")
	{
		shootCube(vector3df(2,2,2), 1);
		return;
	}

	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

IRigidBody* addCube(const irr::core::vector3df &pos, const irr::core::vector3df &scale, irr::f32 mass, irr::core::stringc textureFile = "game/rockwall.rttex")
{
	irr::scene::ISceneNode *Node = GetIrrlichtManager()->GetScene()->addCubeSceneNode(1.0f);
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	Node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, GetIrrlichtManager()->GetTexture(textureFile.c_str()));
	
	//Node->setMaterialFlag(irr::video::EMF_WIREFRAME, drawWireFrame);

	ICollisionShape *shape = new IBoxShape(Node, mass, false);

	//shape->setMargin(0.01);

	IRigidBody *body;
	body = GetIrrlichtManager()->GetBulletWorld()->addRigidBody(shape);
	body->setDamping(0.2,0.2);
	body->setFriction(0.6f);

	return body;
}

void HelloMenuInitScene()
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

	scene::ICameraSceneNode* camera =
	smgr->addCameraSceneNodeFPS(0,100.0f,0.1f, 0, 0, 0, true, 3.f);
	camera->setPosition(core::vector3df(50,15,70)); 
	camera->setTarget(core::vector3df(20,7,10));

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

	device->getSceneManager()->addLightSceneNode(0, vector3df(20, 40, -50), SColorf(1.0f, 1.0f, 1.0f), 4000.0f);

	irrBulletWorld *world = GetIrrlichtManager()->GetBulletWorld();
	
	

	scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
		GetIrrlichtManager()->GetTexture("game/irrlicht2_up.rttex"),
		GetIrrlichtManager()->GetTexture("game/irrlicht2_dn.rttex"),
		GetIrrlichtManager()->GetTexture("game/irrlicht2_lf.rttex"),
		GetIrrlichtManager()->GetTexture("game/irrlicht2_rt.rttex"),
		GetIrrlichtManager()->GetTexture("game/irrlicht2_ft.rttex"),
		GetIrrlichtManager()->GetTexture("game/irrlicht2_bk.rttex"));
	//scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("../../media/skydome.jpg"),16,8,0.95f,2.0f);

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	//create ground

	ISceneNode *Node = device->getSceneManager()->addCubeSceneNode(1.0);
	Node->setScale(vector3df(300,3,300)); // 400, 3, 400
	Node->setPosition(vector3df(20,0,10));
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0,GetIrrlichtManager()->GetTexture("game/rockwall.rttex"));
	//  if(drawWireFrame)
	//     Node->setMaterialFlag(EMF_WIREFRAME, true);
	ICollisionShape *shape = new IBoxShape(Node, 0, false);
	//shape->setMargin(0.01);
	IRigidBody *body;
	body = world->addRigidBody(shape);


	//add the blocks

	int columns = 7;
	int rows = 7;

	for(u32 j=0; j < columns; j++)
	{
		for(u32 i=0; i < rows; i++)
		{

			if (rand()%2 == 1)
			addCube(vector3df(3 * i, (0 + 3 * j+ 3)+(j*3), 0), vector3df(3,3,3), 3);
		}
	}

	GetBaseApp()->GetGameTimer()->Reset();
}

Entity * PhysicsHelloMenuCreate(Entity *pParentEnt)
{
	Entity *pBG = pParentEnt->AddEntity(new Entity("PhysicsHelloMenu"));

	AddFocusIfNeeded(pBG);

	Entity *pButtonEntity;
	pButtonEntity = CreateTextButtonEntity(pBG, "Back", iPhoneMapX(240),iPhoneMapY(290), "Back"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&PhysicsHelloMenuOnSelect);
	pButtonEntity->GetVar("alignment")->Set(uint32(ALIGNMENT_CENTER));


	pButtonEntity = CreateTextButtonEntity(pBG, "Shoot", iPhoneMapX(390),iPhoneMapY(100), "Shoot Cube"); 
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&PhysicsHelloMenuOnSelect);
	
	//le's make the button sfx cooler:
	SetButtonClickSound(pButtonEntity, "audio/shoot.wav");

	//setup movement button
	Entity *pEnt = pBG->AddEntity(new Entity("controls", new FPSControlComponent));

	SlideScreen(pBG, true);

	HelloMenuInitScene();
	return pBG;
}

