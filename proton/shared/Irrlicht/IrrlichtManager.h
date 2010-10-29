//  ***************************************************************
//  IrrlichtManager - Creation date: 11/19/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef IrrlichtManager_h__
#define IrrlichtManager_h__

#include "PlatformSetup.h"
#include "irrlicht.h"

class IrrlichtManager
{
public:
	IrrlichtManager();
	virtual ~IrrlichtManager();

	bool Init(irr::IEventReceiver *pEventReceiver);
	void Kill();

	bool IsRunning();
	void BeginScene();
	void Render();
	void EndScene();
	void ClearScene();

	irr::video::ITexture * GetTexture(const std::string &fileName);
	irr::IrrlichtDevice * GetDevice() {return m_pDevice;}
	irr::video::IVideoDriver* GetDriver() {return m_pDriver;}
	irr::scene::ISceneManager* GetScene() {return m_pScene;}


private:
	
	irr::IrrlichtDevice * m_pDevice;
	irr::video::IVideoDriver* m_pDriver;
	irr::scene::ISceneManager* m_pScene;
};

IrrlichtManager * GetIrrlichtManager();

#endif // IrrlichtManager_h__