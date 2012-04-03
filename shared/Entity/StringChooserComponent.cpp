#include "StringChooserComponent.h"

#include "util/MiscUtils.h"
#include "Entity.h"

StringChooserComponent::StringChooserComponent() :
	m_pComponentName(NULL),
	m_pVarName(NULL),
	m_pVar(NULL)
{
	SetName("StringChooser");
}

StringChooserComponent::~StringChooserComponent()
{
}

void StringChooserComponent::UpdateVarValue()
{
	UpdateVarValueWithIndex(GetVar("index"));
}

void StringChooserComponent::UpdateVarValueWithIndex(Variant *pIndex)
{
	if (m_pVar)
	{
		uint32 index = pIndex->GetUINT32();
		if (index < m_pStringParts.size())
		{
			m_pVar->Set(m_pStringParts.at(index));
		}
	}
}

void StringChooserComponent::UpdateStringParts(Variant *pStrings)
{
	m_pStringParts = StringTokenize(pStrings->GetString(), ":");
	UpdateVarValue();
}

void StringChooserComponent::UpdateVarPointer(Variant *unused)
{
	EntityComponent * pComp = GetParent()->GetComponentByName(*m_pComponentName);
	if (pComp)
	{
		m_pVar = pComp->GetVar(*m_pVarName);
		UpdateVarValue();
	} else
	{
		m_pVar = NULL;
	}
}

void StringChooserComponent::OnAdd(Entity *pEnt)
{
	EntityComponent::OnAdd(pEnt);

	m_pComponentName = &GetVar("component_name")->GetString();
	m_pVarName = &GetVar("var_name")->GetString();

	UpdateVarPointer(NULL);
	UpdateStringParts(GetVar("strings"));

	GetVar("index")->GetSigOnChanged()->connect(boost::bind(&StringChooserComponent::UpdateVarValueWithIndex, this, _1));
	GetVar("strings")->GetSigOnChanged()->connect(boost::bind(&StringChooserComponent::UpdateStringParts, this, _1));
	GetVar("component_name")->GetSigOnChanged()->connect(boost::bind(&StringChooserComponent::UpdateVarPointer, this, _1));
	GetVar("var_name")->GetSigOnChanged()->connect(boost::bind(&StringChooserComponent::UpdateVarPointer, this, _1));
}
