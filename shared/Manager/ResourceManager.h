//  ***************************************************************
//  ResourceManager - Creation date: 07/01/2009
//  -------------------------------------------------------------
//  Robinson Technologies (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef ResourceManager_h__
#define ResourceManager_h__

class SurfaceAnim;

//this class caches out resources by detecting similar filenames through a hashed map.

class Resource
{
public:

	enum eResourceType
	{
		TYPE_UNKNOWN,
		TYPE_SURFACE_ANIM,
	};
	Resource(){m_pResource = NULL; m_type = TYPE_UNKNOWN;}
	~Resource();

	void *m_pResource;	

	eResourceType m_type;
};

typedef std::map<string, Resource*> ResourceMap;


class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();
	SurfaceAnim * GetSurfaceAnim(string fileName);
	void KillAllResources();
	void RemoveTexturesNotInExclusionList(const vector<string> &exclusionList);

private:

	Resource * FindDataByKey(const string &keyName);

	ResourceMap m_data;
};

#endif // ResourceManager_h__