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

Or, you can cast to this class and use SetConsole() to set your own custom console to feed in data.

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
	void SetConsole(Console *pConsole); //set a custom Console object to feed in the text/lines of the log data.  We won't try to delete it

private:

	void OnRender(VariantList *pVList);
	void AddLine(VariantList *pVList);
	void OnEnableScrollingChanged(Variant *pVariant);
	void UpdateScrollBar();
	void OnTouchDragUpdate(VariantList *pVList);
	void OnOverStart(VariantList *pVList);
	void OnOverEnd(VariantList *pVList);
	void ModCurLine(float mod);
	void OnTextAdded();
	void OnUpdate(VariantList *pVList);
	void ModByDistance(float mod);
	CL_Vec2f *m_pPos2d;
	CL_Vec2f *m_pSize2d;
	uint32 *m_pFontID;
	
	CL_Vec2f *m_pScale2d;
	uint32 *m_pColor;
	uint32 *m_pColorMod;
	float *m_pAlpha;
	uint32 *m_pAlignment;
	float *m_pRotation; //in degrees
	float *m_pFontScale; //so we can set it independently of our main size, although that will be applied to the font too
	uint32 *m_pEnableScrolling; 
	Console * m_pActiveConsole;
	Console * m_pInternalConsole;
	EntityComponent *m_pScrollBarComp; //optional, only used if enabledScrolling was set to 1
	bool m_bIsDraggingLook;
	float m_curLine; //a float, so we can move slower than one whole line at a time if we wish
	bool m_bUsingCustomConsole;

	//for scrolling momentum stuff, simular to how the ScrollComponent works but.. I recreated it here as I
	//didn't think it made sense to somehow hook a ScrollComponent into this. -Seth

	CL_Vec2f m_vecDisplacement;
	float * m_pFriction, *m_pMaxScrollSpeed, *m_pPowerMod;

};

void ToggleConsole();
void SetConsole(bool bOn);
#endif // LogDisplayComponent_h__