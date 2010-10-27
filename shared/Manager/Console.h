//  ***************************************************************
//  Console - Creation date:  03/06/2009
//  -------------------------------------------------------------
//  Robinson Technologies - Check license.txt for license info.
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef Console_h__
#define Console_h__

using namespace std;

class Console
{
public:
	Console();
	virtual ~Console();

	void AddLine(string line);
	void SetMaxLines(unsigned int num) {m_maxLines = num;}
	string GetAsSingleString();
	void Clear();
	size_t GetTotalLines(){return m_log.size();}
	string GetLine(int index) {return m_log.at(index);}
	
private:

	unsigned int m_maxLines;
	deque<string> m_log;
};

#endif // Console_h__