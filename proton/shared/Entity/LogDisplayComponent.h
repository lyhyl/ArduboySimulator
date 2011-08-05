//  ***************************************************************
//  LogDisplayComponent - Creation date: 2/23/2011
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2011 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef LogDisplayComponent_h__
#define LogDisplayComponent_h__

#include "Entity/Component.h"
#include "Entity/Entity.h"
#include "Manager/Console.h"

/*
This shows a log of stuff, scrolling so the newest stuff is on the bottom.
If you use GetFunction("AddLine") (send it a VariantList with a string) to add your own stuff, it will use its internal logging
system.  

Otherwise, it will display the proton system log.

An easy way to turn use this to show the system log is to use the helper function below.

Just do:

SetConsole(true); and it will create it as an overlay.

*/

class LogDisplayComponent: public EntityComponent
{
public:
	LogDisplayComponent();
	virtual ~LogDisplayComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

	//if you REALLY need direct access to this...
	Console * GetConsole() {return m_pActiveConsole;}

private:

	void OnRender(VariantList *pVList);
	void AddLine(VariantList *pVList);
	CL_Vec2f *m_pPos2d;
	CL_Vec2f *m_pSize2d;
	uint32 *m_pFontID;
	
	CL_Vec2f *m_pScale2d;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	float *m_pAlpha;
	uint32 *m_pAlignment;
	float *m_pRotation; //in degrees
	
	Console * m_pActiveConsole;
	Console * m_pInternalConsole;
};

void ToggleConsole();
void SetConsole(bool bOn);
#endif // LogDisplayComponent_h__