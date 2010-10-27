//  ***************************************************************
//  CustomInputComponent - Creation date: 10/04/2010
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2010 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef CustomInputComponent_h__
#define CustomInputComponent_h__

#include "Entity/Component.h"

class CustomInputComponent: public EntityComponent
{
public:
	CustomInputComponent();
	virtual ~CustomInputComponent();

	virtual void OnAdd(Entity *pEnt);
	virtual void OnRemove();

private:
	string * m_pKeys;
	void OnInput( VariantList *pVList );
	void OnActivated();
	uint32 *m_pDisabled;
	uint32 *m_pKeyCode;

};

#endif // CustomInputComponent_h__