#include "PlatformPrecomp.h"
#include "StreamingInstanceZip.h"

#define CASESENSITIVITY (0)

StreamingInstanceZip::StreamingInstanceZip()
{
	m_uf = NULL;
	m_bIsFinished = true;
}

StreamingInstanceZip::~StreamingInstanceZip()
{
	Close();

	if (m_uf)
		unzClose(m_uf);
}

bool StreamingInstanceZip::Init(string zipFileName)
{
	assert(!m_uf && "We don't handle calling this twice yet");
	m_uf = unzOpen(zipFileName.c_str());
	if (!m_uf)
	{
		return false;
	}

	return true; //success
}

bool StreamingInstanceZip::Open( string fName)
{
	int err = UNZ_OK;
	
	if (unzLocateFile(m_uf,(m_rootDir+fName).c_str(),CASESENSITIVITY)!=UNZ_OK)
	{
		return false;
	}
	
	char st_filename_inzip[512];

	err = unzGetCurrentFileInfo(m_uf,&m_file_info,st_filename_inzip,sizeof(st_filename_inzip),NULL,0,NULL,0);

	if (err!=UNZ_OK)
	{
		LogError("error %d with zipfile in unzGetCurrentFileInfo",err);
		return false;
	}

	m_bufferCurIndex = 0;
	m_bufferBytesLeft = 0;

	err = unzOpenCurrentFile(m_uf);

	if (err!=UNZ_OK)
	{
		LogError("error %d with zipfile in unzOpenCurrentFile",err);
		return false;
	}

	m_bIsFinished = false;
	
	return true;
}

bool StreamingInstanceZip::IsFinished()
{
	return m_bIsFinished;
}

int StreamingInstanceZip::FillBufferWithCachedBytes(byte *pBufferOut, int maxBytesToRead)
{
	maxBytesToRead = rt_min(maxBytesToRead, m_bufferBytesLeft);

	memcpy(pBufferOut, m_buffer+m_bufferCurIndex, maxBytesToRead);
	m_bufferCurIndex += maxBytesToRead;
	m_bufferBytesLeft -= maxBytesToRead;
	return maxBytesToRead;
}

int StreamingInstanceZip::Read( byte * pBufferOut, int maxBytesToRead )
{
	if (m_bIsFinished) return 0;

	int totalBytesWritten = 0;

	while(maxBytesToRead >= 0)
	{
		if (m_bufferBytesLeft > 0)
		{
			int bytesRead = FillBufferWithCachedBytes(pBufferOut, maxBytesToRead);
			pBufferOut += bytesRead;
			maxBytesToRead -= bytesRead;
			totalBytesWritten += bytesRead;
			if (maxBytesToRead == 0) return totalBytesWritten;
		}

		//if we got here, it means we need to fill our buffer again
		int err = unzReadCurrentFile(m_uf,m_buffer,BUFFER_SIZE);
		if (err<0)	
		{
			LogError("error %d with zipfile in unzReadCurrentFile",err);
			return false;
		}

		if (err == 0)
		{
			//EOF reached
			Close();
			break;
		}

		m_bufferBytesLeft = err;
		m_bufferCurIndex = 0;
	}

	return totalBytesWritten;
}

void StreamingInstanceZip::SetRootDirectory( string rootDir )
{
	m_rootDir = rootDir+"/";
}


void StreamingInstanceZip::Close()
{
	if (!m_bIsFinished)
	{
		int err = unzCloseCurrentFile(m_uf);
		if (err!=UNZ_OK)
		{
			LogError("error %d with zipfile in unzCloseCurrentFile",err);
			return;
		}
		m_bIsFinished = true;

	}
}