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
#ifdef RT_IRRBULLET
#include "irrBullet/irrbullet.h"
#endif
#include "util/MathUtils.h"

//helpers for conversions to and from different vector types (they all have the same data format)

#define ToIrrVector3(a) (*(irr::core::vector3df*)(&(a)))
#define ToCLVector3(a) (*(CL_Vec3f*)(&(a)))

using namespace irr;

class irrBulletWorld;

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

	irrBulletWorld * GetBulletWorld() {return m_pWorld;}

private:
	
	irr::IrrlichtDevice * m_pDevice;
	irr::video::IVideoDriver* m_pDriver;
	irr::scene::ISceneManager* m_pScene;

	irrBulletWorld *m_pWorld;

};

IrrlichtManager * GetIrrlichtManager();
core::rect<s32> CLRectToIrrlichtRect32(CL_Rectf clR);

#endif // IrrlichtManager_h__