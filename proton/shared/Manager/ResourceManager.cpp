#include "PlatformPrecomp.h"
#include "ResourceManager.h"
#include "Renderer/SurfaceAnim.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	KillAllResources();
}

Resource * ResourceManager::FindDataByKey(const string &keyName)
{
	ResourceMap::iterator itor = m_data.find(keyName);

	if (itor != m_data.end())
	{
		//found it
		return itor->second;
	}
	return NULL; //fail
}

SurfaceAnim * ResourceManager::GetSurfaceAnim( string fileName )
{
	if (fileName.empty()) return NULL;

	Resource *pData = FindDataByKey(fileName);
	if (!pData)
	{
		SurfaceAnim *pSurf = new SurfaceAnim;
		
		pSurf->SetTextureType(Surface::TYPE_GUI);

		if (!pSurf->LoadFile(fileName))
		{
			delete pSurf;
			return NULL;
		}

		pData = new Resource;
		if (!pData)
		{
			delete pSurf;
			return NULL;
		}

		pData->m_type = Resource::TYPE_SURFACE_ANIM;
		pData->m_pResource = pSurf;
		m_data[fileName] = pData;

	}

	return (SurfaceAnim*)pData->m_pResource;
}

void ResourceManager::KillAllResources()
{
	ResourceMap::iterator itor = m_data.begin();
	while (itor != m_data.end())
	{
		delete itor->second;
		itor++;
	}

	m_data.clear();
}

void ResourceManager::RemoveTexturesNotInExclusionList( const vector<string> &exclusionList )
{
	ResourceMap::iterator itor = m_data.begin();
	while (itor != m_data.end())
	{
		bool bIgnore = false;

		for (unsigned int i=0; i < exclusionList.size(); i++)
		{
			if (itor->first == exclusionList[i])
			{
				bIgnore = true;
				break;
			}
		}

		if (bIgnore)
		{
			itor++;
			continue;
		}
		ResourceMap::iterator itorTemp = itor;
		itor++;

		delete itorTemp->second;
		m_data.erase(itorTemp);
		
	}

}

Resource::~Resource()
{
	switch (m_type)
	{

	case TYPE_SURFACE_ANIM:
		delete (SurfaceAnim*) m_pResource;
		break;

	default:;
		LogMsg("Wat is type %d", m_type);
		assert(!"Huh?");
	}
}

