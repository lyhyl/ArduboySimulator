//  ***************************************************************
//  StringChooserComponent - Creation date: 04/03/2012
//  -------------------------------------------------------------
//
//  ***************************************************************
//  Programmer(s):  Aki Koskinen
//  ***************************************************************

#ifndef StringChooserComponent_h__
#define StringChooserComponent_h__

#include "Component.h"

/**
 * A component that selects one string from a list of strings and sets the value to
 * another \c Component's \c Variant.
 *
 * This \c Component is initialized with a list of strings. Later on an index variable can
 * be set in order to select any member in the string list. The chosen member is used to
 * set a string \c Variant in another \c Component. The other \c Component must have the same
 * parent \c Entity with this \c Component but the name of the \c Component as well as the
 * name of the \c Variant in that \c Component can be chosen freely.
 * 
 * The name of the \c Component is initially set to "StringChooser".
 * 
 * The following named variants are used inside the component itself:
 * - <b>"strings" (string):</b> the list of strings to choose from. Supply this as one string where
 *   the members of the list are separated with a ":" (a colon).
 * - <b>"index" (uint32):</b> the index to the list, starts from 0. If the index is bigger than there
 *   are members in the string list then the \c Variant in the target \c Component is left unchanged.
 * - <b>"component_name" (string):</b> specifies the name of the target \c Component.
 * - <b>"var_name" (string):</b> specifies the name of the affected \c Variant in the target
 *   \c Component. The type of the target \c Variant must be string.
 */
class StringChooserComponent: public EntityComponent
{
public:
	StringChooserComponent();
	virtual ~StringChooserComponent();

	virtual void OnAdd(Entity *pEnt);
	
private:
	void UpdateVarValue();
	void UpdateVarValueWithIndex(Variant *pIndex);
	void UpdateStringParts(Variant *pStrings);
	void UpdateVarPointer(Variant *unused);

	std::vector<std::string> m_pStringParts;
	std::string *m_pComponentName;
	std::string *m_pVarName;
	Variant *m_pVar;

};

#endif // StringChooserComponent_h__
