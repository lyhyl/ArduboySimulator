#include "PlatformPrecomp.h"
#include "FileCopier.h"
#include "FileManager.h"
#include "StreamingInstance.h"


FileCopier::FileCopier()
{
	m_destFP = NULL;
	m_pSrcStreamInstance = NULL;
	m_status = STATUS_OK;
}

FileCopier::~FileCopier()
{
	if (m_destFP)
	{
		fclose(m_destFP);
	}

	if (m_status != STATUS_FINISHED)
	{
		RemoveFile(m_destFileName, false);
	}
}

bool FileCopier::Init( string fileNameSource, string fileNameDest )
{
	//open source
#ifdef _DEBUG
	if (fileNameDest == fileNameSource) 
	{
		LogMsg("Huh?");
		return false;
	}
#endif
	m_pSrcStreamInstance = GetFileManager()->GetStreaming(fileNameSource, &m_sourceSizeBytes, false);

	if (!m_pSrcStreamInstance)
	{
		LogError("Unable to open %s for reading", fileNameSource.c_str());
		m_status = STATUS_ERROR_GENERIC;
		return false;
	}

	//open dest
	m_destFP = fopen(fileNameDest.c_str(), "wb");
	
	if (!m_destFP)
	{
		LogError("Unable to open %s for write", fileNameDest.c_str());
		m_status = STATUS_ERROR_GENERIC;
		return false;
	}

	m_destFileName = fileNameDest;
	return true;
}

bool FileCopier::Update( int maxBytesToWrite )
{
	
	int bytesRead = 0;
	
	while (maxBytesToWrite > 0)
	{
		bytesRead = m_pSrcStreamInstance->Read(m_buffer,rt_min(FILE_COPY_BUFFER_SIZE, maxBytesToWrite));

		//write it out
		if (bytesRead > 0)
		{
			int bytesWritten = fwrite(m_buffer, 1, bytesRead, m_destFP);
			if (bytesWritten != bytesRead)
			{
				m_status = STATUS_ERROR_LOW_SPACE;
				return false;
			}
		} else
		{
			//done
			m_status = STATUS_FINISHED;
			fclose(m_destFP);
			m_destFP = NULL;
			return false;
		}

		maxBytesToWrite -= bytesRead;
	}

	return true; //signal to keep calling us, we're not finished
}

bool FileCopier::InitAndCopy( string fileNameSource, string fileNameDest )
{
	if (!Init(fileNameSource, fileNameDest))
	{
		return false;
	}

	while(1)
	{
		if (!Update(FILE_COPY_BUFFER_SIZE*2))
		{
			return m_status == STATUS_FINISHED;
		}
	}

	return false;

}