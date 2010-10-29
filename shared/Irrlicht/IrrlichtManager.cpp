#include "IrrlichtManager.h"
//#include "source/Irrlicht/COGLESDriver.h"
//#include "source/Irrlicht/COpenGLDriver.h"

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
		m_pScene->getMeshCache()->clear(); 
		m_pScene->clear();	
		const video::SMaterial m;
		m_pDriver->setMaterial(m); 
		m_pDriver->removeAllTextures();
		//m_pDriver->se
	

	}
}

void IrrlichtManager::BeginScene()
{
	
	if (m_pDriver)
	{
		//((COGLES1Driver*)m_pDriver)->setRenderStates3DMode();
		m_pDriver->beginScene(false, false, SColor(255,100,101,140));
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
		m_pDriver->endScene();
		const video::SMaterial m;
		m_pDriver->setMaterial(m); 
		
		m_pDriver->setRenderStates3DMode(); //let irrlicht turn off its last material
		
		glDisable(GL_ALPHA_TEST);
		glDisable( GL_BLEND );
		glDisable( GL_LIGHTING );
		
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable(GL_TEXTURE_2D);

	}

}

bool IrrlichtManager::IsRunning()
{
	if (m_pDriver)
	return m_pDevice->run();

	return false;
}