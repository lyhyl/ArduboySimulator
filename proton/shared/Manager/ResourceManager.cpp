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

SurfaceAnim * ResourceManager::GetSurfaceAnim(const string &fileName)
{
	return GetSurfaceResource<SurfaceAnim>(fileName, Surface::TYPE_GUI);
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

	case TYPE_SURFACE:
		delete m_pResource;
		break;

	default:
		LogMsg("Wat is type %d", m_type);
		assert(!"Huh?");
	}
}

