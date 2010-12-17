#ifndef ClanlibUtils_h__
#define ClanlibUtils_h__

#include "PlatformSetup.h"

class TextScanner
{
public:

	TextScanner(string fName);
	~TextScanner();

	void Kill();

	bool LoadFile(string fName);

	string GetParmString(string label, int index);

	string GetMultipleLineStrings(string label, string token = "|");

	vector<string> m_lines;

private:
	char *m_pBuff;
	unsigned int m_size;
	int m_lastLine; //used during searches to remember the state
};

bool RunRTPackByCommandLine(string command_string);

#endif // ClanlibUtils_h__
