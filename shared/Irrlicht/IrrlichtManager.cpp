#include "IrrlichtManager.h"
#include "Renderer/Surface.h"
#include "BaseApp.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;


//poor man's singleton
IrrlichtManager g_irrlichtManager;

IrrlichtManager * GetIrrlichtManager()
{
	return &g_irrlichtManager;
}

IrrlichtManager::IrrlichtManager()
{
	m_pDevice = NULL;
	m_pScene = NULL;
	m_pDriver = NULL;
	m_pWorld = NULL;
}


IrrlichtManager::~IrrlichtManager()
{
	Kill();
}


void IrrlichtManager::Kill()
{
	//LogMsg("Killing irrlicht");

	if (m_pDevice)
	{
		if (m_pWorld)
		{
			delete m_pWorld;
		}
		m_pDevice->drop();
		m_pDevice = NULL;
		m_pDriver = NULL;
		m_pScene = NULL;
	}

}


bool IrrlichtManager::Init(irr::IEventReceiver *pEventReceiver)
{
	LogMsg("initting irrlicht");

	E_DRIVER_TYPE driverType = video::EDT_OGLES1;

#ifdef C_GL_MODE
	driverType = video::EDT_OPENGL;
#endif

	m_pDevice = createDevice( driverType, dimension2d<u32>(GetPrimaryGLX(), GetPrimaryGLY()), 16, false, false, false, pEventReceiver);

	if (!m_pDevice)
	{
		LogError("Unable to create video driver");
		return false;
	}

	m_pDriver = m_pDevice->getVideoDriver();
	m_pScene = m_pDevice->getSceneManager();

	if (!m_pDevice->getFileSystem()->addFileArchive("", true, false, EFAT_PROTON))
	{
		LogMsg("Unable to mount Proton filesystem");
	}

#ifdef RT_IRRBULLET

	////////////////////////////
	// Create irrBullet World //
	////////////////////////////

	assert(!m_pWorld);
	m_pWorld = createIrrBulletWorld(m_pDevice, true, true);

	m_pWorld->setDebugMode(EPDM_DrawAabb |
		EPDM_DrawContactPoints);

	m_pWorld->setGravity(vector3df(0,-10,0));


#endif
	return true;
}

irr::video::ITexture * IrrlichtManager::GetTexture(const std::string &fileName)
{

	return m_pDriver->getTexture( (GetBaseAppPath() + fileName).c_str() );
}

void IrrlichtManager::ClearScene()
{
	if (m_pScene)
	{
		
	/*
		while (m_pDevice->getFileSystem()->getFileArchiveCount())
		{
			m_pDevice->getFileSystem()->removeFileArchive(0);
		}
	*/	
	
#ifdef RT_IRRBULLET
		while(m_pWorld->getNumCollisionObjects() > 0)
		{
			m_pWorld->removeCollisionObject(m_pWorld->getCollisionObject(0));
		}
#endif
		
		m_pScene->getRootSceneNode()->removeAll();
		m_pScene->getMeshCache()->clear(); 
		m_pScene->clear();	
		const video::SMaterial m;
		m_pDriver->setMaterial(m); 
		m_pDriver->removeAllTextures();
		

	}
}

void IrrlichtManager::BeginScene()
{
	if (m_pDriver)
	{
		m_pDriver->beginScene(false, false, SColor(255,100,101,140));
	
#ifdef RT_IRRBULLET
		
		if (m_pWorld)
		{
		// Step the simulation with our delta time
		m_pWorld->stepSimulation(GetBaseApp()->GetGameDelta()*0.02f, 120);

		//m_pWorld->debugDrawWorld(debugDraw);
		// This call will draw the technical properties of the physics simulation
		// to the GUI environment.
		//m_pWorld->debugDrawProperties(drawProperties);
		}
#endif
	}
}

void IrrlichtManager::Render()
{

	if (m_pScene)
	m_pScene->drawAll();

	
}

void IrrlichtManager::EndScene()
{
	if (m_pDriver)
	{
		if (m_pDevice->getGUIEnvironment())
		{
			m_pDevice->getGUIEnvironment()->drawAll();
		}
		
		m_pDriver->endScene();
	
		const video::SMaterial m;
		m_pDriver->setMaterial(m); 
		
		m_pDriver->setRenderStates3DMode(); //let irrlicht turn off its last material
		
		glDisable(GL_ALPHA_TEST);
		glDisable( GL_BLEND );
		glDisable( GL_LIGHTING );
		
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable(GL_TEXTURE_2D);
		
		g_lastBound = NO_TEXTURE_LOADED;
	}

}

bool IrrlichtManager::IsRunning()
{
	if (m_pDriver)
	return m_pDevice->run();

	return false;
}

core::rect<s32> CLRectToIrrlichtRect32(CL_Rectf clR)
{
	core::rect<s32> r;
	r.UpperLeftCorner.X = clR.left;
	r.UpperLeftCorner.Y = clR.top;
	r.LowerRightCorner.X = clR.right;
	r.LowerRightCorner.Y = clR.bottom;
	return r;
}