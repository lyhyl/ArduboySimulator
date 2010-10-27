#include "PlatformPrecomp.h"
#include "Component.h"

EntityComponent::EntityComponent()
{
	m_parent = NULL;
}

EntityComponent::~EntityComponent()
{
}
EntityComponent::EntityComponent( string name )
{
	SetName(name);
}
void EntityComponent::SetName( string name )
{
	m_name = name;
}

void EntityComponent::OnAdd(Entity *pEnt)
{
	m_parent = pEnt;
}

void EntityComponent::OnRemove()
{
	FunctionObject *pFunc = GetShared()->GetFunctionIfExists("OnDelete");
	if (pFunc)
	{
		//looks like someone wanted notification
		pFunc->sig_function(&VariantList(this));

	}
}
