#include "main.h"
#include "TexturePacker.h"
#include "FontPacker.h"
#include "FileSystem/FileManager.h"

App g_App;

App * GetApp() {return &g_App;}
void WaitForKey();

//dummy stuff now used but needed for so things will link when including game library stuff

int GetPrimaryGLX() {return 0;}
int GetPrimaryGLY() {return 0;}


void LogMsg ( const char* traceStr, ... )
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	memset ( (void*)buffer, 0, logSize );

	va_start ( argsVA, traceStr );
	vsnprintf_s( buffer, logSize, logSize, traceStr, argsVA );
	va_end( argsVA );
	OutputDebugString(buffer);
	OutputDebugString("\n");
	printf(buffer);
	printf("\n");
}


void LogError ( const char* traceStr, ... )
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	memset ( (void*)buffer, 0, logSize );

	va_start ( argsVA, traceStr );
	vsnprintf_s( buffer, logSize, logSize, traceStr, argsVA );
	va_end( argsVA );
	LogMsg("ERROR: %s\n", buffer);
	WaitForKey();
}

void WaitForKey()
{
	cout << "\nPlease press [RETURN] to continue: " << flush;
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail() , '\n');

	cin.get();
	cout << endl;
}



bool App::ParmExists( string parm )
{
	for (unsigned int i=0; i < m_parms.size(); i++)
	{
		if (ToLowerCaseString(parm) == ToLowerCaseString(m_parms[i]))
		{
			return true;
		}
	}

	return false;
}

bool App::ParmExistsWithData( string parm, string *parmData )
{
	for (unsigned int i=0; i < m_parms.size(); i++)
	{
		if (ToLowerCaseString(parm) == ToLowerCaseString(m_parms[i]))
		{
			if (i >= m_parms.size())
			{
				//no more parms to check, so no data for you
				return false;
			}
			*parmData = m_parms[i+1];
			return true;
		}
	}

	return false;
}

std::string App::GetLastParm()
{
	return m_parms[m_parms.size()-1];
}

string App::GetPixelTypeText()
{
	return m_pixelTypeText;
}

void App::SetPixelTypeText( string s )
{
 m_pixelTypeText = s;
}

void App::SetPixelType( pvrtexlib::PixelType ptype )
{
	m_pixelType = ptype;

	switch (ptype)
	{
	case pvrtexlib::OGL_PVRTC2:
		SetPixelTypeText("PVRTC2"); break;

	case pvrtexlib::OGL_PVRTC4:
		SetPixelTypeText("PVRTC4"); break;

	case pvrtexlib::OGL_RGBA_4444:
		SetPixelTypeText("RGBA 16 bit"); break;
	case pvrtexlib::OGL_RGBA_8888:
		SetPixelTypeText("RGBA 32 bit"); break;
	case pvrtexlib::OGL_RGB_565:
		SetPixelTypeText("RGB 16 bit (565)"); break;
	case pvrtexlib::OGL_RGB_888:
		SetPixelTypeText("RGB 24 bit"); break;

	default:

		SetPixelTypeText("Unknown");
	}
}
void ShowHelp()
{ 
	LogMsg("Help and examples\n");
	LogMsg("RTPack.exe <any file> (Compresses it as an rtpack without changing the name)");
	LogMsg("RTPack.exe -make_font <filename.txt> (Create a .rtfont)");
	
	LogMsg("RTPack.exe -pvrtc4 <image file> (Makes pvrtc .rttex)");
	LogMsg("");
	LogMsg("More options/flags for making textures:\n");
	LogMsg("RTPack.exe -pvrt4444 <image file> (Makes raw 16 bit 4444 or 565 if no alpha)");
	LogMsg("RTPack.exe -pvrt8888 <image file> (Creates raw 32 bit .rttex");
	LogMsg("RTPack.exe -pvrt4444 -o pvr <image file> (Writes a .pvr format output)");
	LogMsg("Extra flags you can use with texture generation:");
	LogMsg("-mipmaps (Causes mipmaps to be generated)");
	LogMsg("-stretch (Stretches instead of pads to reach power of 2)");
	LogMsg("-force_square (forces textures to be square in addition to being power of 2)");
	LogMsg("-4444_if_not_square_or_too_big (1024 width or height and non square will use -pvrt4444)");
	LogMsg("-8888_if_not_square_or_too_big (1024 width or height and non square will use -pvrt8888)");
	LogMsg("-flipv (vertical flip, applies to textures only)");

}


int main(int argc, char* argv[])
{
	LogMsg("\nRTPack V1.1 by Seth A. Robinson.  /h for help\n");

	
#ifdef _DEBUG
	//printf("Got %d parms:", argc-1);
#endif

	if (argc > 1)
	{
		for (int i=1; i < argc; i++)
		{
#ifdef _DEBUG
			printf(argv[i]);
			printf(" ");
#endif

			g_App.m_parms.push_back(argv[i]);
		}
	}
#ifdef _DEBUG
	
	printf("\n\n");
#endif
	string outputFormat;
	if (GetApp()->ParmExistsWithData("-o", &outputFormat))
	{
		if (outputFormat == "pvr")
		{
			GetApp()->SetOutput(App::PVR);
		}

		if (outputFormat == "png")
		{
			GetApp()->SetOutput(App::PNG);
		}
	}

	if (GetApp()->ParmExists("-mipmaps"))
	{
		GetApp()->SetMaxMipLevel(20);
	}
	
	if (GetApp()->ParmExists("-flipv"))
	{
		GetApp()->SetFlipV(true);
	}

	if (GetApp()->ParmExists("-stretch"))
	{
		GetApp()->SetStretchImage(true);
	}

	if (GetApp()->ParmExists("-force_square"))
	{
		GetApp()->SetForceSquare(true);
	}

	if (GetApp()->ParmExists("-8888_if_not_square_or_too_big"))
	{
		GetApp()->SetPixelTypeIfNotSquareOrTooBig(pvrtexlib::OGL_RGBA_8888);
	}

	if (GetApp()->ParmExists("-4444_if_not_square_or_too_big"))
	{
		GetApp()->SetPixelTypeIfNotSquareOrTooBig(pvrtexlib::OGL_RGBA_4444);
	}

	if (GetApp()->ParmExists("/h") || GetApp()->ParmExists("-h") || argc == 1)
	{
		ShowHelp();
	} else

	if (argc == 2)
	{
		//just compress whatever the hell this is
		CompressFile(GetApp()->m_parms[0]);
		return 0;
	}

	
	if (GetApp()->ParmExists("-pvrtc4"))
	{
		GetApp()->SetPixelType(pvrtexlib::OGL_PVRTC4);

	} 
	if (GetApp()->ParmExists("-pvrtc2"))
	{
		GetApp()->SetPixelType(pvrtexlib::OGL_PVRTC2);
	
	} 
	if (GetApp()->ParmExists("-pvrt4444"))
		{
			GetApp()->SetPixelType(pvrtexlib::OGL_RGBA_4444);
		}
		if (GetApp()->ParmExists("-pvrt8888"))
		{
			GetApp()->SetPixelType(pvrtexlib::OGL_RGBA_8888);
		}


		if (GetApp()->GetPixelType() != 0)
		{
			if (GetApp()->m_parms.size() < 2)
			{
				LogError("Aren't you missing the filename?");
			} else
			{
				TexturePacker packer;
				packer.SetCompressionType(GetApp()->GetPixelType());
				packer.ProcessTexture(GetApp()->GetLastParm());
			}

		}

			if (GetApp()->ParmExists("-make_font"))
			{
				if (GetApp()->m_parms.size() < 2)
				{
					LogError("Aren't you missing the filename of the .txt?");
				} else
				{
					FontPacker packer;
					packer.PackFont(GetApp()->GetLastParm());
				}
			}

	

#ifdef _DEBUG
	WaitForKey();
#endif
	return 0;
}


bool IsLargeScreen()
{
	return true; 
}


ePlatformID GetEmulatedPlatformID()
{
	return PLATFORM_ID_WINDOWS;
}

FileManager * GetFileManager()
{
	return NULL;
}

int GetScreenSizeX() {assert(!"Not used"); return 0;}
int GetScreenSizeY() {assert(!"Not used"); return 0;}
