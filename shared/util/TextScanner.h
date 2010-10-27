//  ***************************************************************
//  TextScanner - Creation date: 06/09/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef TextScanner_h__
#define TextScanner_h__

class TextScanner
{
public:

	TextScanner();
	TextScanner(const char *pCharArray);
	TextScanner(const string &fName);
	TextScanner(const string &fName, bool bAddBasePath );
	~TextScanner();

	void Kill();

	bool LoadFile(const string &fName, bool bAddBasePath = true);
	string GetParmString(string label, int index, string token = "|");
	string GetMultipleLineStrings(string label, string token = "|");
	string GetLine(int lineNum); //0 based, returns "" if out of range
	vector<string> m_lines;

	bool IsLoaded() {return !m_lines.empty();}
	bool SetupFromMemoryAddress(const char *pCharArray);

	void StripLeadingSpaces();
	string GetAll();

private:

	int m_lastLine; //used during searches to remember the state
};


#endif // TextScanner_h__