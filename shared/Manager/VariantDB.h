//  ***************************************************************
//  SharedDB - Creation date: 04/11/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef VariantDB_h__
#define VariantDB_h__
#include "util/Variant.h"

class Variant;


#define C_VARIANT_DB_FILE_VERSION 1

class FunctionObject
{
public:

	boost::signal<void (VariantList*)> sig_function;
};

//#define C_USE_BOOSTS_HASHMAP 1

#ifdef C_USE_BOOSTS_HASHMAP

	#include "util/boost/boost/unordered/unordered_map.hpp"
	typedef boost::unordered_map<string, Variant*> dataList;
	typedef boost::unordered_map<string, FunctionObject*> functionList;

#else

#ifdef __APPLE__
#include <ext/hash_map>

namespace __gnu_cxx {
	/**
	Explicit template specialization of hash of a string class,
	which just uses the internal char* representation as a wrapper.
	*/
	template <>
	struct hash<std::string> {
		size_t operator() (const std::string& x) const {
			return hash<const char*>()(x.c_str());
			// hash<const char*> already exists
		}
	};
}


typedef  __gnu_cxx::hash_map<string, Variant*> dataList;
typedef  __gnu_cxx::hash_map<string, FunctionObject*> functionList;

#else
#include <hash_map>

#if defined RT_WEBOS_ARM || ANDROID_NDK

namespace __gnu_cxx {
	/**
	Explicit template specialization of hash of a string class,
	which just uses the internal char* representation as a wrapper.
	*/
	template <>
	struct hash<std::string> {
		size_t operator() (const std::string& x) const {
			return hash<const char*>()(x.c_str());
			// hash<const char*> already exists
		}
	};
}

typedef  __gnu_cxx::hash_map<string, Variant*> dataList;
typedef  __gnu_cxx::hash_map<string, FunctionObject*> functionList;

#else
typedef stdext::hash_map<string, Variant*> dataList;
typedef stdext::hash_map<string, FunctionObject*> functionList;

#endif

#endif

#endif

class VariantDB
{
public:
	VariantDB();
	virtual ~VariantDB();

	FunctionObject * GetFunction(const string &keyName); //created it needed, this is usually what you want
	FunctionObject * GetFunctionIfExists(const string &keyName); //returns null if not created yet
	void CallFunctionIfExists(const string &keyName, VariantList *pVList);

	Variant * GetVar(const string &keyName);  //created it needed, this is usually what you want
	Variant * GetVarIfExists(const string &keyName);  //returns null if not created yet
	Variant * GetVarWithDefault(const string &keyName, const Variant &vDefault);
	int DeleteVarsStartingWith(string deleteStr);
	//you can load and save the variables in the DB on the fly.  (Does nothing with the functions)
	bool Save(const string &fileName, bool bAddBasePath = true);
	bool Load(const string &fileName, bool *pFileExistedOut, bool bAddBasePath = true);
	
	void Print(); //for debugging, will LogMsg everything inside of it

private:

	void DeleteAll();

	dataList m_data;
	functionList m_functionData;
};

#endif // Variant_h__