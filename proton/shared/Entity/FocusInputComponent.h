//  ***************************************************************
//  FocusInputComponent - Creation date: 04/14/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef FocusInputComponent_h__
#define FocusInputComponent_h__

#include "Component.h"

class Entity;

class FocusInputComponent: public EntityComponent
{
public:
	FocusInputComponent();
	virtual ~FocusInputComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();
	
	void OnInput(VariantList *pVList);
	void LinkMoveMessages(VariantList *pVList);
	//our stuff

	enum eMode
	{
		MODE_START_NORMAL,
		MODE_START_NONE  //don't actually wire to receive any messages, useful if you want to hand-wire what it will catch later
	};

private:

	
};

#endif // FocusInputComponent_h__