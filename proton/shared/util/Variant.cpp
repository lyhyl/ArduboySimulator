#include "PlatformPrecomp.h"
#include "Variant.h"
#include "MiscUtils.h"
#include "ResourceUtils.h"
#include "RenderUtils.h"

Variant::~Variant()
{
	SAFE_DELETE(m_pSig_onChanged);
}

boost::signal<void (Variant*)> * Variant::GetSigOnChanged()
{
	if (!m_pSig_onChanged)
	{
		m_pSig_onChanged = new boost::signal<void (Variant*)>;
	}
	return m_pSig_onChanged;
}

void Variant::Set( string const &var )
{
	assert(m_type == TYPE_UNUSED || m_type == TYPE_STRING);
	m_type = TYPE_STRING;
	m_string = var;
	if (m_pSig_onChanged) (*m_pSig_onChanged)(this);
}

void Variant::SetVariant( Variant *pVar ) //needed this because boost was confused...
{
	Set(*pVar);
}

void Variant::Set( Variant &v )
{
	
	//update our data from another variant object
	switch(v.GetType())
	{
	case TYPE_FLOAT:
		Set(v.GetFloat());
		break;
	case TYPE_STRING:
		Set(v.GetString());
		break;
	case TYPE_VECTOR2:
		Set(v.GetVector2());
		break;
	case TYPE_VECTOR3:
		Set(v.GetVector3());
		break;
	case TYPE_UINT32:
		Set(v.GetUINT32());
		break;
	case TYPE_ENTITY:
		Set(v.GetEntity());
		break;
	case TYPE_COMPONENT:
		Set(v.GetComponent());
		break;
	case TYPE_RECT:
		Set(v.GetRect());
		break;
	
	default:

		assert(!"Add support for this var type?");
	}
}

std::string Variant::Print()
{
	switch(GetType())
	{
	case TYPE_FLOAT:
		return toString(GetFloat());
		break;
	case TYPE_STRING:
		return GetString();
		break;
	
	case TYPE_VECTOR2:
		return PrintVector2(GetVector2());
		break;
	case TYPE_VECTOR3:
		return PrintVector3(GetVector3());
		break;

	case TYPE_UINT32:
		return toString(GetUINT32());
		break;
	case TYPE_ENTITY:
		//return GetEntity()->GetName();
		return "An entity"; //I don't want to include the entity.h here right now
		break;
	case TYPE_COMPONENT:
		//return GetEntity()->GetName();
		return "A component"; //I don't want to include the entity.h here right now
		break;
	case TYPE_RECT:
		return PrintRect(GetRect());
		break;

	case TYPE_UNUSED:
		return "Unknown";
		break;


	default:

		assert(!"Add support for this var type?");
	}

	return "Unknown";
}



//curPos is between 0 (value of A) and 1 (value of B)

void Variant::Interpolate(Variant *pA,Variant *pB, float curPos, eInterpolateType type)
{

	assert(GetType() == pA->GetType() && GetType() == pB->GetType() && "these should all be of the same type");
	bool bAsColor = false;

	switch (type)
	{
	case INTERPOLATE_LINEAR_AS_COLOR:
		bAsColor = true;
		break; //as is

	case INTERPOLATE_SMOOTHSTEP:
		curPos = SMOOTHSTEP(curPos);
		break;

	case INTERPOLATE_SMOOTHSTEP_AS_COLOR:
		curPos = SMOOTHSTEP(curPos);
		bAsColor = true;
		break;

	case INTERPOLATE_EASE_TO:
		curPos = EASE_TO(curPos);
		break;
	case INTERPOLATE_EASE_FROM:
		curPos = EASE_FROM(curPos);
		break;
	case INTERPOLATE_LINEAR:
		break; //as is

	default:
		LogError("Unknown interpolation type");
		assert(0);
	}

	switch (pA->GetType())
	{

	case Variant::TYPE_FLOAT:
		{
			Set( pA->GetFloat() + (   (pB->GetFloat()- pA->GetFloat()  ) * curPos));
		}
		break;

	case Variant::TYPE_VECTOR2:
		{
			Set( pA->GetVector2() + (   (pB->GetVector2()- pA->GetVector2()  ) * curPos));
		}

		break;

		case Variant::TYPE_UINT32:
		{
			if (bAsColor)
			{
				Set(ColorCombineMix(pA->GetUINT32(), pB->GetUINT32(), curPos));
			} else
			{
				Set( uint32(  float(pA->GetUINT32()) + (   (float(pB->GetUINT32())- float(pA->GetUINT32())  ) * curPos)));

			}
		}

		break;

	default:
		LogError("Interpolate: Don't handle this combination yet");
		assert(0);
	}
}

bool Variant::Save( FILE *fp, const string &varName )
{
	//first, filter out types we can't save, like pointers
	int dataSizeBytes = 0;
	
	if (GetType() == TYPE_STRING)
	{
		dataSizeBytes = m_string.size();
	} else
	{
		dataSizeBytes = GetSizeOfData(GetType());
	}
	
	if (dataSizeBytes == 0) return true; //can't save this type, but not an error

	uint32 varType = GetType();
	int bytesRead = fwrite(&varType, 1, sizeof(uint32), fp);
	if (bytesRead == 0) return false;

	//save the variable name
	SaveToFile(varName, fp);
	
	if (GetType() == TYPE_STRING)
	{
		SaveToFile(m_string, fp);
	} else
	{
		fwrite(m_var, dataSizeBytes, 1, fp);
	}

	
	return true;
}

int GetSizeOfData(Variant::eType type)
{
	switch(type)
	{
	case Variant::TYPE_UNUSED:
	case Variant::TYPE_COMPONENT:
	case Variant::TYPE_ENTITY:
		return 0;
		break;

	case Variant::TYPE_UINT32:
	case Variant::TYPE_FLOAT:
		return 4;
	case Variant::TYPE_VECTOR2:
		return sizeof(CL_Vec2f);
	case Variant::TYPE_VECTOR3:
		return sizeof(CL_Vec3f);
	case Variant::TYPE_RECT:
		return sizeof(CL_Rectf);

	default:

		assert(!"Uh..");
	}

	return 0;
}