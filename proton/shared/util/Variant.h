//  ***************************************************************
//  Variant - Creation date: 04/13/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef Variant_h__
#define Variant_h__

#include "MathUtils.h"

//#define SETH_SIGNALS

#ifdef SETH_SIGNALS
#include "sethsignal.h"

#else

#include "util/boost/boost/signal.hpp"
#include "util/boost/boost/bind.hpp"

#endif

class Entity;
class EntityComponent;
class VariantList;

enum eInterpolateType
{
	INTERPOLATE_LINEAR = 0,
	INTERPOLATE_SMOOTHSTEP,
	INTERPOLATE_EASE_TO,
	INTERPOLATE_EASE_FROM,
	INTERPOLATE_EASE_TO_QUARTIC,
	INTERPOLATE_EASE_FROM_QUARTIC,
	INTERPOLATE_BOUNCE_TO,
	INTERPOLATE_SMOOTHSTEP_AS_COLOR,
	INTERPOLATE_LINEAR_AS_COLOR

};

#define C_VAR_SPACE_BYTES 16 //enough to fit a rect

//signals are ignored when copying these
class Variant
{
public:

	enum eType
	{
		TYPE_UNUSED,
		TYPE_FLOAT,
		TYPE_STRING,
		TYPE_VECTOR2,
		TYPE_VECTOR3,
		TYPE_UINT32,
		TYPE_ENTITY,
		TYPE_COMPONENT,
		TYPE_RECT,
		TYPE_INT32
				
	};
	Variant(uint32 var) {SetDefaults(); Set(var);}
	Variant(int32 var) {SetDefaults(); Set(var);}

	Variant(float var) {SetDefaults(); Set(var);}
	Variant(const string &var) {SetDefaults(); Set(var);}
	Variant(float x, float y) {SetDefaults(); Set(CL_Vec2f(x,y));}
	Variant(float x, float y, float z) {SetDefaults(); Set(CL_Vec3f(x,y,z));}
	Variant(const CL_Vec2f &v2) {SetDefaults(); Set(v2);}
	Variant(const CL_Vec3f &v3) {SetDefaults(); Set(v3);}
	Variant(const CL_Rectf &r) {SetDefaults(); Set(r);}
	Variant(Entity *pEnt) {SetDefaults(); Set(pEnt);}
	Variant(EntityComponent *pEnt) {SetDefaults(); Set(pEnt);}
	
	Variant()
	{
		SetDefaults();
	}
	~Variant();
	

	void Reset()
	{
		m_type = TYPE_UNUSED;
		SAFE_DELETE(m_pSig_onChanged);
	}

	boost::signal<void (Variant*)> * GetSigOnChanged();
	
	void Set(Variant &v);
	void SetVariant( Variant *pVar ) /*needed this because boost was confused... */;

	void Set(float var) 
	{
		assert(m_type == TYPE_UNUSED || m_type == TYPE_FLOAT);
		m_type = TYPE_FLOAT; *((float*)m_var) = var;
		if (m_pSig_onChanged) (*m_pSig_onChanged)(this);
	}
	
	float & GetFloat()
	{
		if (m_type == TYPE_UNUSED) Set(float(0));
		assert(m_type == TYPE_FLOAT);
		return  *((float*)m_var);
	}
	
	void Set(Entity *pEnt)
	{
		assert(m_type == TYPE_UNUSED || m_type == TYPE_ENTITY);
		m_type = TYPE_ENTITY;   m_pVoid =  pEnt;
		if (m_pSig_onChanged) (*m_pSig_onChanged)(this);
	}
	
	Entity * GetEntity()
	{
		if (m_type == TYPE_UNUSED) Set((Entity*)NULL);
		assert(m_type == TYPE_ENTITY);
		return  ((Entity*)m_pVoid);
	}

	void Set(EntityComponent *pEnt)
	{
		assert(m_type == TYPE_UNUSED || m_type == TYPE_COMPONENT);
		m_type = TYPE_COMPONENT;   m_pVoid =  pEnt;
		if (m_pSig_onChanged) (*m_pSig_onChanged)(this);
	}

	EntityComponent * GetComponent()
	{
		if (m_type == TYPE_UNUSED) Set((Entity*)NULL);
		assert(m_type == TYPE_COMPONENT);
		return  ((EntityComponent*)m_pVoid);
	}

	void Set(uint32 var) 
	{
		assert(m_type == TYPE_UNUSED || m_type == TYPE_UINT32);
		m_type = TYPE_UINT32; *((uint32*)m_var) = var;
		if (m_pSig_onChanged) (*m_pSig_onChanged)(this);
	}

	uint32 & GetUINT32()
	{
		if (m_type == TYPE_UNUSED) Set(uint32(0));
		assert(m_type == TYPE_UINT32);
		return  *((uint32*)m_var);
	}

	void Set(int32 var) 
	{
		assert(m_type == TYPE_UNUSED || m_type == TYPE_INT32);
		m_type = TYPE_INT32; *((int32*)m_var) = var;
		if (m_pSig_onChanged) (*m_pSig_onChanged)(this);
	}

	int32 & GetINT32()
	{
		if (m_type == TYPE_UNUSED) Set(int32(0));
		assert(m_type == TYPE_INT32);
		return  *((int32*)m_var);
	}

	void Set(string const &var);
	string & GetString() 
	{
		return m_string;
	}
	
	void Set(CL_Vec2f const &var) 
	{
		assert(m_type == TYPE_UNUSED || m_type ==TYPE_VECTOR2);
		m_type = TYPE_VECTOR2; *((CL_Vec2f*)m_var) = var;
		if (m_pSig_onChanged) (*m_pSig_onChanged)(this);
	}
	CL_Vec2f & GetVector2()
	{
		if (m_type == TYPE_UNUSED)
		{
			//set a default
			Set(CL_Vec2f(0,0));
		}
		return  *((CL_Vec2f*)m_var);
	}

	void Set(float x, float y) 
	{
		Set(CL_Vec2f(x,y));
	}

	void Set(CL_Vec3f const &var) 
	{
		assert(m_type == TYPE_UNUSED || m_type == TYPE_VECTOR3);
		m_type = TYPE_VECTOR3; *((CL_Vec3f*)m_var) = var;
		if (m_pSig_onChanged) (*m_pSig_onChanged)(this);
	}
	CL_Vec3f & GetVector3()
	{
		if (m_type == TYPE_UNUSED)
		{
			//set a default
			Set(CL_Vec3f(0,0,0));
		}
		return  *((CL_Vec3f*)m_var);
	}

	void Set(CL_Rectf const &var) 
	{
		assert(m_type == TYPE_UNUSED || m_type == TYPE_RECT);
		m_type = TYPE_RECT; *((CL_Rectf*)m_var) = var;
		if (m_pSig_onChanged) (*m_pSig_onChanged)(this);
	}
	CL_Rectf & GetRect()
	{
		if (m_type == TYPE_UNUSED)
		{
			//set a default
			Set(CL_Rectf(0,0,0,0));
		}
		return  *((CL_Rectf*)m_var);
	}


	void Set(float x, float y, float z) 
	{
		Set(CL_Vec3f(x,y,z));
	}


	eType GetType() const  {return m_type;}
	string Print();


	Variant(const Variant &v)
	{
		SetDefaults();
		//our special copy won't include the sig crap, that stuff can't be copied
		*this = v;
	}


	Variant & operator= (const Variant &rhs)
	{
		m_type = rhs.m_type;
		m_pVoid = rhs.m_pVoid;
		memcpy(m_var, rhs.m_var, C_VAR_SPACE_BYTES);
		m_string = rhs.m_string;
		
		if (m_pSig_onChanged)
		{
			//if anyone was connected to us, let them know the valid change.  Up to them to figure out that
			//the type might have as well!

			if (m_pSig_onChanged) (*m_pSig_onChanged)(this);
		}
		return *this; 
	}



	void Interpolate(Variant *pA,Variant *pB, float curPos, eInterpolateType type);

	bool Save(FILE *fp, const string &varName ); //assumes you've already fopen'ed something and pass the file pointer in

	void ClearConnections(); //clear any boost connections that were connections to its onchanged signal.

	friend class VariantList;

private:

	void SetDefaults()
	{
		m_type = TYPE_UNUSED;
		m_pSig_onChanged = NULL;
	}
	eType m_type;
	void *m_pVoid;
	
	union
	{
		byte m_var[C_VAR_SPACE_BYTES]; //large enough so we can use the space for all the datatypes we care about
		
		//don't actually use these, these unions help me look at vars when debugging easier
		float m_as_floats[4];
		uint32 m_as_uint32s[4];
		int32 m_as_int32s[4];
	};
	string m_string;
	boost::signal<void (Variant*)> *m_pSig_onChanged; //not initialized unless used

};

//a VariantList holds a group of variants, we pass these when we don't know in advance how many variants we want to use

#define C_MAX_VARIANT_LIST_PARMS 6

/*
//example of memory serialization of a VariantList

VariantList v;
v.Get(0).Set(uint32(42));
v.Get(1).Set("Hey guys");

//save to mem
byte *pData = v.SerializeToMem(&size, NULL);

//load from mem
VariantList b;
b.SerializeFromMem(pData, size);

//display
LogMsg("%s, the answer to life is %d", b.Get(1).GetString().c_str(), b.Get(0).GetUINT32());

//clean up the mem we initted earlier
SAFE_DELETE_ARRAY(pData);
*/

class VariantList
{
	public:

		VariantList(){};

		Variant & Get(int parmNum) {assert(parmNum < C_MAX_VARIANT_LIST_PARMS); return m_variant[parmNum];}
		VariantList(Variant v0) {m_variant[0] = v0;}
		VariantList(Variant v0, Variant v1) {m_variant[0] = v0; m_variant[1] = v1;}
		VariantList(Variant v0, Variant v1, Variant v2) {m_variant[0] = v0; m_variant[1] = v1; m_variant[2] = v2;}
		VariantList(Variant v0, Variant v1, Variant v2, Variant v3) {m_variant[0] = v0; m_variant[1] = v1; m_variant[2] = v2; m_variant[3] = v3;}
		VariantList(Variant v0, Variant v1, Variant v2, Variant v3, Variant v4) {m_variant[0] = v0; m_variant[1] = v1; m_variant[2] = v2; m_variant[3] = v3; m_variant[4] = v4;}
		VariantList(Variant v0, Variant v1, Variant v2, Variant v3, Variant v4, Variant v5) {m_variant[0] = v0; m_variant[1] = v1; m_variant[2] = v2; m_variant[3] = v3; m_variant[4] = v4;  m_variant[5] = v5;}

		void Reset()
		{
			for (int i=0; i < C_MAX_VARIANT_LIST_PARMS; i++)
			{
				m_variant[i].Reset();
			}
		}

		byte * SerializeToMem(uint32 *pSizeOut, byte *pDest); //pass in NULL for dest and it will new[] the memory itself
		bool SerializeFromMem(byte *pSrc, int length );
		Variant m_variant[C_MAX_VARIANT_LIST_PARMS]; //non-dynamic for speed
		void GetVariantListStartingAt(VariantList *pOut, int startIndex); //shift variables over to the right, deleting some.  0 based index
};

int GetSizeOfData(Variant::eType type);

#endif // Variant_h__
