//  ***************************************************************
//  FileSystemZip - Creation date: 09/25/2010
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2010 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef FileSystemZip_h__
#define FileSystemZip_h__

#include "FileSystem.h"
#include "util/unzip/unzip.h"

class FileSystemZip : public FileSystem
{
public:
	FileSystemZip();
	virtual ~FileSystemZip();
	bool Init(string apkFileName);
	vector<string> GetContents();
	void SetRootDirectory(string rootDir);

	virtual byte * Get( string fileName, int *pSizeOut);
	virtual StreamingInstance * GetStreaming(string fileName, int *pSizeOut); //pSizeOut currently always set to 0.  Returns null on fail. You must DELETE !
	virtual bool FileExists(string fileName);

protected:
	

private:

	unzFile m_uf;
	string m_rootDir;
	string m_zipFileName;
};

#endif // FileSystemZip_h__