#include "PlatformPrecomp.h"
#include "TextScanner.h"
#include "util/ResourceUtils.h"
#include "util/MiscUtils.h"

TextScanner::TextScanner()
{
	m_lastLine = 0;
}

TextScanner::TextScanner(const string &fName )
{
	m_lastLine = 0;
	LoadFile(fName);
}

TextScanner::TextScanner(const string &fName, bool bAddBasePath )
{
	m_lastLine = 0;
	LoadFile(fName, bAddBasePath);
}

TextScanner::TextScanner( const char *pCharArray )
{
	m_lastLine = 0;
	SetupFromMemoryAddress(pCharArray);
}


bool TextScanner::LoadFile( const string &fName, bool bAddBasePath )
{
	Kill();

	FileInstance f(fName, bAddBasePath);
	if (!f.IsLoaded()) return false;

	return SetupFromMemoryAddress(f.GetAsChars());
}

bool TextScanner::SetupFromMemoryAddress(const char *pCharArray)
{
	m_lines = StringTokenize(pCharArray, "\n");

	for (unsigned int i=0; i < m_lines.size(); i++)
	{
		StringReplace("\r", "", m_lines[i]);

	}
	return true;
}

std::string TextScanner::GetParmString( string label, int index,  string token)
{
	if (m_lines.empty())
	{
		LogError("Load a file first");
		return "";
	}

	for (unsigned int i=0; i < m_lines.size(); i++)
	{
		if (m_lines[i].empty()) continue;
		vector<string> line = StringTokenize(m_lines[i], token);
		if (line[0] == label)
		{
			//found it
			return line[index];
		}
	}

	return "";
}

TextScanner::~TextScanner()
{
	Kill();
}

void TextScanner::Kill()
{
	m_lines.clear();
	m_lastLine = 0;
}

std::string TextScanner::GetMultipleLineStrings( string label, string token )
{
	for (unsigned int i=m_lastLine; i < m_lines.size(); i++)
	{
		if (m_lines[i].empty()) continue;
		vector<string> line = StringTokenize(m_lines[i], token);
		if (line[0] == label)
		{
			//found it
			m_lastLine = i+1;
			return m_lines[i];
		}
	}
	m_lastLine = 0; //reset it
	return "";
}

void TextScanner::StripLeadingSpaces()
{
	for (unsigned int i=0; i < m_lines.size(); i++)
	{
		m_lines[i] = StripWhiteSpace(m_lines[i]);

	}

}

string TextScanner::GetLine( int lineNum )
{
	if ((int)m_lines.size() > lineNum && lineNum >= 0)
	{
		return m_lines[lineNum];
	}

	//invalid line
	return "";
}

string TextScanner::GetAll()
{
	string s;

	for (unsigned int i=0; i < m_lines.size(); i++)
	{
		s += StripWhiteSpace(m_lines[i])+"\n";
	}

	return s;
}