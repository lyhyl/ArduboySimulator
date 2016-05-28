//  ***************************************************************
//  MyEEPROM - Creation date: 05/26/2016
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2016 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef MyEEPROM_h__
#define MyEEPROM_h__

#include "core/core.h"
class MyEEPROM
{
public:
	MyEEPROM();
	virtual ~MyEEPROM();

	/*
	uint8_t read( int idx )              { return EERef( idx ); }
	void write( int idx, uint8_t val )   { (EERef( idx )) = val; }
	void update( int idx, uint8_t val )  { EERef( idx ).update( val ); }
*/
	uint8_t read( int idx )              { return 0;}
	void write( int idx, uint8_t val )   { }
	void update( int idx, uint8_t val )  { }
	

private:
};

#endif // MyEEPROM_h__