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

#include "Renderer/Surface.h"
class SurfaceAnim;

//this class caches out resources by detecting similar filenames through a hashed map.

class Resource
{
public:

	enum eResourceType
	{
		TYPE_UNKNOWN,
		TYPE_SURFACE
	};
	Resource(){m_pResource = NULL; m_type = TYPE_UNKNOWN;}
	~Resource();

	Surface *m_pResource;

	eResourceType m_type;
};

typedef std::map<string, Resource*> ResourceMap;


class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();
	SurfaceAnim * GetSurfaceAnim(const string &fileName);

	template<class T>
	T * GetSurfaceResource(const string &fileName, Surface::eTextureType textureType = Surface::TYPE_DEFAULT) {
		if (fileName.empty()) return NULL;

		Resource *pData = FindDataByKey(fileName);
		if (!pData)
		{
			Surface *pSurf = new T;

			pSurf->SetTextureType(textureType);

			if (!pSurf->LoadFile(fileName))
			{
				delete pSurf;

				LogMsg("ResourceManager::GetResource: Unable to load %s", fileName.c_str());
				return NULL;
			}

			pData = new Resource;
			if (!pData)
			{
				delete pSurf;
				return NULL;
			}

			pData->m_type = Resource::TYPE_SURFACE;
			pData->m_pResource = pSurf;
			m_data[fileName] = pData;

		}

		return dynamic_cast<T*>(pData->m_pResource);
	}

	void KillAllResources();
	void RemoveTexturesNotInExclusionList(const vector<string> &exclusionList);

private:

	Resource * FindDataByKey(const string &keyName);

	ResourceMap m_data;
};

#endif // ResourceManager_h__
