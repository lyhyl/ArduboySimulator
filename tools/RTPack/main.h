#pragma once

//a windows only tool to turn any image into a .rttex or .rtfont file

#include "PlatformSetup.h"
#include "util/MiscUtils.h"
#include "util/ResourceUtils.h"
#include <clanlib/core.h>
#include <Clanlib/display.h>
#include "PVRTexLib/PVRTexLib.h"
#include "util/RTFileFormat.h"
#include "ClanlibUtils.h"

class App
{
public:

	enum eOutput
	{
		RTTEX,
		PVR,
		PNG
	};

	App()
	{
		m_output = RTTEX;
		SetPixelType(pvrtexlib::PixelType(0));
		SetPixelTypeIfNotSquareOrTooBig(pvrtexlib::PixelType(0));
		SetMaxMipLevel(1);
		SetStretchImage(false);
		SetForceSquare(false);
		SetFlipV(false);
	}

	bool ParmExistsWithData(string parm, string *parmData);
	bool ParmExists(string parm); //not case sensitive
	vector<string> m_parms;
	string GetLastParm();
	eOutput GetOutput() {return m_output;}
	void SetOutput(eOutput o) {m_output = o;}
	void SetPixelType(pvrtexlib::PixelType ptype );
	pvrtexlib::PixelType GetPixelType() {return m_pixelType;}
	void SetPixelTypeText(string s);
	string GetPixelTypeText();
	
	void SetMaxMipLevel(int maxMip) {m_maxMipLevel = maxMip;}
	int GetMaxMipLevel() {return m_maxMipLevel;}

	void SetStretchImage(bool bNew) {m_stretchImage = bNew;}
	bool GetStretchImage() {return m_stretchImage;}

	void SetForceSquare(bool bNew) {m_forceSquare = bNew;}
	bool GetForceSquare() {return m_forceSquare;}

	void SetPixelTypeIfNotSquareOrTooBig(pvrtexlib::PixelType ptype) {m_pixelTypeIfNotSquareOrTooBig = ptype;}
	pvrtexlib::PixelType GetPixelTypeIfNotSquareOrTooBig() {return m_pixelTypeIfNotSquareOrTooBig;}

	void SetFlipV(bool bNew) {m_bFlipV = bNew;}
	bool GetFlipV() {return m_bFlipV;}
	
private:
	
	eOutput m_output;
	pvrtexlib::PixelType m_pixelType;
	pvrtexlib::PixelType m_pixelTypeIfNotSquareOrTooBig;
	string m_pixelTypeText;
	int m_maxMipLevel;
	bool m_stretchImage;
	bool m_forceSquare;
	bool m_bFlipV;

};

App * GetApp();

