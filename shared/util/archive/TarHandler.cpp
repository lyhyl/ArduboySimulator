#include "PlatformPrecomp.h"
#include "TarHandler.h"

TarHandler::TarHandler()
{
	m_fp = NULL;
	m_fpOut = NULL;
	m_state = STATE_NONE;
	m_pBzipBuffer = NULL;
	
}

TarHandler::~TarHandler()
{
	Kill();
	SAFE_DELETE_ARRAY(m_pBzipBuffer);
}

void TarHandler::Kill()
{
	if (m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;
	}

	if (m_fpOut)
	{
		fclose(m_fpOut);
		m_fpOut = NULL;
	}

	m_state = STATE_NONE;
	memset(&m_tarHeader, 0, sizeof(tar_header));
	m_tarHeaderBytesRead = 0;
	m_tarState = TAR_STATE_FILLING_HEADER;
	m_totalBytesWritten = 0;
	m_bytesNeededToReachBlock = 0;
	m_bForceLowerCaseFileNames = true;
}

void TarHandler::panic(char *pMessage)
{
	LogMsg(pMessage);
}

bool myfeof ( FILE* f )
{
	int c = fgetc ( f );
	if (c == EOF) return true;
	ungetc ( c, f );
	return false;
}

void TarHandler::OnBZIPError(int error)
{
	m_state = STATE_ERROR;
	m_error = error;
	LogError("Error %d", error);
}


/* string-octal to integer conversion */
int oct_to_int(char *oct)
{
	int i;
	sscanf(oct, "%o", &i);
	return i;
}


bool TarHandler::WriteBZipStream(byte *pData, int size)
{
	if (size == 0) return true;

	int headerSize = sizeof(tar_header);
	//LogMsg("Writing %d..", size);
	switch(m_tarState)
	{
		case TAR_STATE_FILLING_HEADER:
			{
				int amountToRead = rt_min(size, headerSize-m_tarHeaderBytesRead);
				memcpy( &m_tarHeader.name[0]+m_tarHeaderBytesRead, pData, amountToRead);

				m_tarHeaderBytesRead += amountToRead;
				m_totalBytesWritten += amountToRead;

				assert(m_tarHeaderBytesRead <= sizeof(tar_header));
				if (m_tarHeaderBytesRead == sizeof(tar_header))
				{
					m_tarFileOutBytesLeft = oct_to_int(m_tarHeader.size);

					//note:  files can be 0 size, so we can't check by that..
					if ( /*m_tarFileOutBytesLeft == 0 ||*/ m_tarHeader.name[0] == 0)
					{
						m_tarState = TAR_STATE_FINISHED;
						if (m_fpOut)
						{
							fclose(m_fpOut);
							m_fpOut = NULL;
						}
						return true;
					}

					//finished reading header, setup to switch to file writing
					m_tarHeaderBytesRead = 0;
					m_tarState = TAR_STATE_WRITING_FILE;
					if (m_bForceLowerCaseFileNames)
					{
						ToLowerCase(m_tarHeader.name);
					}
					
					CreateDirectoryRecursively(m_destPath, GetPathFromString(m_tarHeader.name));
					
					//this is some extra info Dink might want
					if (m_firstDirCreated.empty())
					{
						m_firstDirCreated = m_tarHeader.name;

						//isolate just the first directory


						for (unsigned int i=0; i < m_firstDirCreated.length(); i++)
						{
							if (m_firstDirCreated[i] == '/' || m_firstDirCreated[i] == '\\')
							{
								if (m_firstDirCreated[i+1] != 0)
								{
									//well, this must be the cutoff point
									m_firstDirCreated = m_firstDirCreated.substr(0,i);
								}
								break;
							}

							if (i == m_firstDirCreated.length()-1)
							{
								//couldn't find it. ignore this file, they shouldn't be writing in the base.
								m_firstDirCreated.clear();
							}
						}
					}
					
					//open the file for writing
					m_fpOut = fopen( (m_destPath+m_tarHeader.name).c_str(), "wb");
#ifdef _DEBUG
					LogMsg("Writing %s...", (m_destPath+m_tarHeader.name).c_str());

					/*
					if (string(m_tarHeader.name).find("ds-i8-03.bmp") != string::npos)
					{
						assert(!"Woah!");


					}
					*/
#endif				
					if (!m_fpOut)
					{
						OnBZIPError(BZ_IO_ERROR);
						return true;
					}
					WriteBZipStream(pData+amountToRead, size-amountToRead);

				}
			}
		break;

		case TAR_STATE_WRITING_FILE:
			{
				assert(m_fpOut);

				int amountToRead = rt_min(size, m_tarFileOutBytesLeft);
				int bytesRead = fwrite(pData, 1, amountToRead,  m_fpOut);
				m_totalBytesWritten += amountToRead;

				if (bytesRead != amountToRead)
				{
					OnBZIPError(BZ_IO_ERROR);
					return true;
				}

				m_tarFileOutBytesLeft -= amountToRead;

				if (m_tarFileOutBytesLeft <= 0)
				{
					fclose(m_fpOut);
					m_fpOut = NULL;
					
					m_bytesNeededToReachBlock = headerSize-(m_totalBytesWritten%headerSize);
					
					if (m_bytesNeededToReachBlock == 0 || m_bytesNeededToReachBlock == 512)
					{
						m_tarState = TAR_STATE_FILLING_HEADER;
					} else
					{
						m_tarState = TAR_STATE_READING_BLANK_PART;
					}
					
					WriteBZipStream(pData+amountToRead, size-amountToRead);
				}
			}
			
		break;

		case TAR_STATE_READING_BLANK_PART:
			{

				int amountToRead = rt_min(size, m_bytesNeededToReachBlock);
				m_totalBytesWritten += amountToRead;

				m_bytesNeededToReachBlock -= amountToRead;
				if (m_bytesNeededToReachBlock == 0) m_tarState = TAR_STATE_FILLING_HEADER;
				WriteBZipStream(pData+amountToRead, size-amountToRead);
			}

		break;

		case TAR_STATE_FINISHED:
			assert(size != 0);

			m_totalBytesWritten += size;

			return true;
			
			break;

		default:
			assert(!"Error");
	}
	return true; //success
}

bool TarHandler::uncompressStream ( FILE *zStream)
{
	
		if (!m_bzf)
		{
			if (!m_pBzipBuffer)
			{
				m_pBzipBuffer = new byte[C_BZIP_BUFFER_SIZE];
				if (!m_pBzipBuffer)
				{
				 
					OnBZIPError(BZ_MEM_ERROR);
					return false;
				}
			}
			
			if (!zStream)
			{
				OnBZIPError(BZ_IO_ERROR);
				return false;
			}
			if (ferror(zStream)) goto errhandler_io;
		
			m_bzf = BZ2_bzReadOpen (&m_bzerr, zStream, 0, (int)true, m_bzipReservedBuffer, m_bzipnUnused);
			if (m_bzf == NULL || m_bzerr != BZ_OK) goto errhandler;
			m_streamNo++;
		}

			m_nread = BZ2_bzRead ( &m_bzerr, m_bzf, m_pBzipBuffer, C_BZIP_BUFFER_SIZE );
			if (m_bzerr == BZ_DATA_ERROR_MAGIC) goto trycat;
			if ((m_bzerr == BZ_OK || m_bzerr == BZ_STREAM_END) && m_nread > 0)
			{
				if (!WriteBZipStream(m_pBzipBuffer, m_nread)) goto errhandler_io;
			}
	
		if (m_bzerr == BZ_OK) return true; //no error yet, completed this cycle

		if (m_bzerr != BZ_STREAM_END) goto errhandler;

		BZ2_bzReadGetUnused ( &m_bzerr, m_bzf, &m_bzipunusedTmpV, &m_bzipnUnused );
		if (m_bzerr != BZ_OK) panic ( "decompress:bzReadGetUnused" );

		m_bzipunusedTmp = (byte*)m_bzipunusedTmpV;
		for (m_i = 0; m_i < m_bzipnUnused; m_i++) m_bzipReservedBuffer[m_i] = m_bzipunusedTmp[m_i];

		BZ2_bzReadClose ( &m_bzerr, m_bzf );
		if (m_bzerr != BZ_OK) panic ( "decompress:bzReadGetUnused" );

		//if (m_bzipnUnused == 0 && myfeof(zStream)) break;
	
	m_state = STATE_DONE;
	if (ferror(zStream)) goto errhandler_io;

	m_ret = fclose ( zStream );
	m_fp = NULL; //zstream is actually m_fp that we passed in.. yeah, ugly.
	if (m_ret == EOF) goto errhandler_io;

	return true;

trycat: 
	
errhandler:
	BZ2_bzReadClose ( &m_bzerr_dummy, m_bzf );
	switch (m_bzerr) 
	{
	  case BZ_CONFIG_ERROR:
		  OnBZIPError(m_bzerr); break;
	  case BZ_IO_ERROR:
errhandler_io:
		  OnBZIPError(BZ_IO_ERROR); break;
	  case BZ_DATA_ERROR:
		  OnBZIPError(m_bzerr);
	  case BZ_MEM_ERROR:
		  OnBZIPError(m_bzerr);
	  case BZ_UNEXPECTED_EOF:
		  OnBZIPError(m_bzerr);
	  case BZ_DATA_ERROR_MAGIC:
		  if (zStream != stdin) fclose(zStream);
		  if (m_streamNo == 1) 
		  {
			  return false;
		  } else 
		  {
			  return true;       
		  }
	  default:
		  panic ( "decompress:unexpected error" );
	}

	panic ( "decompress:end" );
	return true; /*notreached*/
}


bool TarHandler::OpenFile( const string &fName, const string &destPath )
{
	Kill();

	m_destPath = destPath;
	int headerSize = sizeof(tar_header);

	m_fp = fopen(fName.c_str(), "rb");
	m_fpOut = NULL;
	m_bzf = NULL;
	m_bzipnUnused = 0;
	m_streamNo = 0;
	m_state = STATE_BZIPPING;
	return (m_fp != 0);
}

bool TarHandler::ProcessChunk()
{
	switch (m_state)
	{
	case STATE_BZIPPING:
		uncompressStream ( m_fp);
		break;

	default:
		return false; //done I guess
	}

	return true; //still processing
}