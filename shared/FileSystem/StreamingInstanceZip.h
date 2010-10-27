//  ***************************************************************
//  StreamingInstanceZip - Creation date: 10/01/2010
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2010 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef StreamingInstanceZip_h__
#define StreamingInstanceZip_h__

#include "StreamingInstance.h"
#include "util/unzip/unzip.h"

class StreamingInstanceZip : public StreamingInstance
{
public:
	StreamingInstanceZip();
	virtual ~StreamingInstanceZip();
	
	virtual bool Open(string fName);
	virtual bool IsFinished();
	virtual int Read(byte * pBufferOut, int maxBytesToRead);
	virtual void Close();
	bool Init(string zipFileName);
	void SetRootDirectory(string rootDir);

protected:

private:

	static const int BUFFER_SIZE = 1024*8;

	int FillBufferWithCachedBytes(byte *pBufferOut, int maxBytesToRead);

	unzFile m_uf;
	unz_file_info m_file_info;

	byte m_buffer[BUFFER_SIZE];
	int m_bufferCurIndex;
	int m_bufferBytesLeft;
	bool m_bIsFinished;
	string m_rootDir;
};

#endif // StreamingInstanceZip_h__