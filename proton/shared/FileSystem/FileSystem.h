//  ***************************************************************
//  FileSystem - Creation date: 09/25/2010
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2010 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef FileSystem_h__
#define FileSystem_h__

class StreamingInstance;

class FileSystem
{
public:
	FileSystem();
	virtual ~FileSystem();

	virtual byte *Get( string fileName, int *pSizeOut) = 0;
	virtual StreamingInstance * GetStreaming(string fileName, int *pSizeOut) = 0; //pSizeOut currently always set to 0.  Returns null on fail. You must DELETE !
	virtual bool FileExists(string fileName) = 0;

protected:
	

private:


};

#endif // FileSystem_h__