#include "FontPacker.h"
#include "FileSystem/FileManager.h"
#include "Clanlib/core.h"
#include "TexturePacker.h"

FontPacker::FontPacker()
{

}

FontPacker::~FontPacker()
{
}


bool FontPacker::WriteHeaderBitMapFontGenerator(FILE *fp, string fntFile, rtfont_header &header)
{
	TextScanner t(fntFile);

	memcpy(header.rtFileHeader.fileTypeID, C_RTFILE_FONT_HEADER, 6);
	
	vector<string> parms = CL_String::tokenize(t.m_lines[1], "=");
	header.charSpacing = 0;
	header.lineHeight = CL_String::to_int(parms[1]);
	header.lineSpacing = CL_String::to_int(parms[1]); //2 would be "base", whatever that is
	header.shadowXOffset = 0;
	header.shadowYOffset = 0;

	string line;
	header.kerningPairCount = 0;

	while ( string(line = t.GetMultipleLineStrings("kerning first", "=")).length() > 0)
	{
		header.kerningPairCount++;
	}

	parms = CL_String::tokenize(t.m_lines[3], "=");
	int charCount = CL_String::to_int(parms[1]);

	header.firstChar = 32;
	header.lastChar = 255;

	fwrite(&header, 1, sizeof(rtfont_header), fp);
	//add the character data
	rtfont_charData charData;

	int skipped = 0;

	for (int i=0; i < header.lastChar-header.firstChar; i++)
	{
		parms = CL_String::tokenize(t.m_lines[ (4+i)-skipped], "=");
		
		int index = CL_String::to_int(parms[1]);
		

		if (index == header.firstChar+i)
		{
			//actually use this
			charData.bmpPosX = CL_String::to_int(parms[2]);
			charData.bmpPosY = CL_String::to_int(parms[3]);
			charData.charSizeX = CL_String::to_int(parms[4]);
			charData.charSizeY = CL_String::to_int(parms[5]);
			charData.charBmpOffsetX = CL_String::to_int(parms[6]);
			charData.charBmpOffsetY = CL_String::to_int(parms[7]);

			charData.charBmpPosU = 0;
			charData.charBmpPosV = 0;
			charData.charBmpPosU2 = 0;
			charData.charBmpPosV2 = 0;
			charData.xadvance = CL_String::to_int(parms[8]);

		} else
		{
			//filler padding, we don't support this char
			
			charData.bmpPosX =0;
			charData.bmpPosY = 0;
			charData.charSizeX = 0;
			charData.charSizeY = 0;
			charData.charBmpOffsetX = 0;
			charData.charBmpOffsetY = 0;

			charData.charBmpPosU = 0;
			charData.charBmpPosV = 0;
			charData.charBmpPosU2 = 0;
			charData.charBmpPosV2 = 0;
			charData.xadvance = 0;
			skipped++;
		}
	
		fwrite(&charData, 1, sizeof(rtfont_charData), fp);
	}

	KerningPair k;

	//also add kerning data, if applicable
	while ( string(line = t.GetMultipleLineStrings("kerning first", "=")).length() > 0)
	{
		
		parms = CL_String::tokenize(line, "=");
		k.first = CL_String::to_int(parms[1]);
		k.second = CL_String::to_int(parms[2]);
		k.amount = CL_String::to_int(parms[3]);
		
		fwrite(&k, 1, sizeof(KerningPair), fp);
	}


	return true;
}


bool FontPacker::WriteHeader(FILE *fp, string fntFile, rtfont_header &header)
{
	TextScanner t(fntFile);

	if (t.m_lines[0][0] == 'i' && t.m_lines[0][1] == 'n')
	{
		//"info" at the front means its a BitMap Font Generator fnt file
		return WriteHeaderBitMapFontGenerator(fp, fntFile, header);
	}

	//assume it's a Well Oiled Font file
	string fntVersion = t.m_lines[1];
	if (fntVersion != "1")
	{
		LogError("Warning: Unknown version of the .fnt file, this will probably explode");
	}
	
	vector<string> parms = CL_String::tokenize(t.m_lines[3], ",");
	memcpy(header.rtFileHeader.fileTypeID, C_RTFILE_FONT_HEADER, 6);

	header.charSpacing = CL_String::to_int(parms[0]);
	header.lineHeight = CL_String::to_int(parms[1]);
	header.lineSpacing = CL_String::to_int(parms[2]);
	header.shadowXOffset = CL_String::to_int(parms[3]);
	header.shadowYOffset = CL_String::to_int(parms[4]);
	header.kerningPairCount = 0;
	parms = CL_String::tokenize(t.m_lines[5], ",");
	header.firstChar = CL_String::to_int(parms[0]);
	header.lastChar = CL_String::to_int(parms[1]);

	fwrite(&header, 1, sizeof(rtfont_header), fp);

	//add the character data
	rtfont_charData charData;

	for (int i=0; i < header.lastChar-header.firstChar; i++)
	{
		parms = CL_String::tokenize(t.m_lines[6+i], ",");
		charData.bmpPosX = CL_String::to_int(parms[0]);
		charData.bmpPosY = CL_String::to_int(parms[1]);
		charData.charSizeX = CL_String::to_int(parms[2]);
		charData.charSizeY = CL_String::to_int(parms[3]);
		charData.charBmpOffsetY = CL_String::to_int(parms[4]);
		charData.charBmpOffsetX = 0;

		charData.charBmpPosU = CL_String::to_float(parms[5]);
		charData.charBmpPosV = CL_String::to_float(parms[6]);
		charData.charBmpPosU2 = CL_String::to_float(parms[7]);
		charData.charBmpPosV2 = CL_String::to_float(parms[8]);
		charData.xadvance = 0;

		fwrite(&charData, 1, sizeof(rtfont_charData), fp);
	}
	
	return true;
}

bool FontPacker::WriteFontStates(FILE *fp, TextScanner &t)
{
	string line;
	while ( string(line = t.GetMultipleLineStrings("add_format_color")).length() > 0)
	{
		FontState s;
		s.m_triggerChar = SeparateStringSTL(line, 1, '|')[0];
		string color = SeparateStringSTL(line, 2, '|');

		byte r = CL_String::to_int(SeparateStringSTL(color, 0, ','));
		byte g = CL_String::to_int(SeparateStringSTL(color, 1, ','));
		byte b = CL_String::to_int(SeparateStringSTL(color, 2, ','));
		s.m_color = MAKE_RGB(r,g,b);
		m_fontStates.push_back(s);
	}

	//now let's write them to the file
	for (unsigned int i=0; i < m_fontStates.size(); i++)
	{
		fwrite(&m_fontStates[i].m_color, 4, 1, fp);
		fwrite(&m_fontStates[i].m_triggerChar, 1, 1, fp);
		fwrite("\0\0\0", 3, 1, fp); //blank reserved
	}

	return true;
}

bool FontPacker::PackFont( string fileName )
{
	TextScanner t(fileName);

	rtfont_header header;
	ZeroMemory(&header, sizeof(rtfont_header));

	string fontImage =  t.GetParmString("image", 1);
	string fntFile = t.GetParmString("fnt_file", 1);
	header.blankCharWidth = CL_String::to_int(t.GetParmString("blank_space_width", 1));
	
	//count how many font states we're going to have
	while ( t.GetMultipleLineStrings("add_format_color").length() > 0) { header.fontStateCount++;}
		
	string path = RemoveLastPartOfDir(fileName);

	//first, let's compress the image file..?
	if (CL_String::get_extension(fontImage) != "rttex")
	{
		
		//this works, but cleaner to just do it inline
		//RunRTPackByCommandLine("-pvrt4444 "+RemoveLastPartOfDir(fileName)+fontImage);

		TexturePacker packer;
		GetApp()->SetMaxMipLevel(1);
		GetApp()->SetPixelType(pvrtexlib::OGL_RGBA_8888);
		packer.SetCompressionType(pvrtexlib::OGL_RGBA_8888);
		packer.ProcessTexture(path+fontImage);
	}

	//first we'll write the header and main data
	string outputFileName = ModifyFileExtension(fileName, "rtfont");
	FILE *fp = fopen(outputFileName.c_str(), "wb");
	WriteHeader(fp, path+fntFile, header);

	WriteFontStates(fp, t);
	//append the BMP to use next
	unsigned int size;
	byte *pData = LoadFileIntoMemory(path + ModifyFileExtension(fontImage, "rttex"), &size);

	if (!pData)
	{
		LogError("Unable to open %s", path + ModifyFileExtension(fontImage, "rttex").c_str());
	} else
	{
		fwrite(pData, size, 1, fp);
	}
	
	SAFE_DELETE_ARRAY(pData);

	fclose(fp);

	//let's compress the entire file
	//RunRTPackByCommandLine(outputFileName);
	CompressFile(outputFileName);
	
	LogMsg("Finished creating %s.  %d chars, %d font state definitions.",outputFileName.c_str(), header.lastChar-header.firstChar, header.fontStateCount);

	return true;
}