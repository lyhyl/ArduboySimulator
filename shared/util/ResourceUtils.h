#pragma once
#include "RTFileFormat.h"

#ifndef CLANLIB_1
	#include "math/rtRect.h"
	#include "MathUtils.h"
	bool LoadFromFile(CL_Vec2f &num, FILE *fp);
	bool LoadFromFile(CL_Vec3f &num, FILE *fp);
	bool LoadFromFile(CL_Rectf &num, FILE *fp);
#endif

byte * LoadFileIntoMemory(string fileName, unsigned int *p_ui_size, bool bUseSavePath = false); //automatically decompresses if needed
byte * LoadFileIntoMemoryBasic(string fileName, unsigned int *length, bool bUseSavePath = false, bool bAddBasePath = true); //won't try to automatically decompress

bool FileExists(const string &fName);

string SeparateStringSTL(string input, int index, char delimiter);
bool SeparateString (const char str[], int num, char delimiter, char *return1);
void StringReplace(const std::string& what, const std::string& with, std::string& in);
bool CompressFile(string fName);
bool IsAPackedFile(byte *pFile);
bool IsARTFile(byte *pFile); //not full proof, but helps with catching errors
rtpack_header BuildRTPackHeader(int size, int compressedSize);

byte * zlibDeflateToMemory(byte *pInput, int sizeBytes, int *pSizeCompressedOut);
byte * zLibInflateToMemory(byte *pInput, unsigned int compressedSize, unsigned int decompressedSize);

//some helpers with file handling
bool LoadFromFile(string &str, FILE *fp);
bool LoadFromFile(float &num, FILE *fp);
bool LoadFromFile(int &num, FILE *fp);
bool LoadFromFile(uint32 &num, FILE *fp);
bool LoadFromFile(bool &num, FILE *fp);
bool SaveToFile(float num, FILE *fp);
bool SaveToFile(int num, FILE *fp);
bool SaveToFile(uint32 num, FILE *fp);
bool SaveToFile(const string &str, FILE *fp);

bool IsPowerOf2(int n);
byte * DecompressRTPackToMemory(byte *pMem, unsigned int *pDecompressedSize=NULL);
int GetFileSize(const string &fName);
string AddIPADToFileName(string file); //appends _ipad to a file name if we are indeed running on an ipad (or large screen)
string ReplaceWithDeviceNameInFileName(const string &fName); //replace "iphone" with "ipad" in filename, if on ipad
string ReplaceWithLargeInFileName(const string &fName); //replace "iphone" with "ipad" in filename, if on ipad
string ReplaceWithLargeInFileNameAndOSSpecific(const string &fName); //like above, but also changes to "win" if Windows is detected