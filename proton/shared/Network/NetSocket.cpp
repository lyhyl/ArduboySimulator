#include "PlatformPrecomp.h"

#include "NetSocket.h"
#include "BaseApp.h"

#ifndef WIN32
	#include <sys/types.h> 
	#include <sys/socket.h>
	#include <sys/wait.h> 
	#include <netinet/in.h> 
	#include <netdb.h> 

#ifdef ANDROID_NDK
#include <fcntl.h>

#else
	#include <sys/fcntl.h>
#endif


#include <sys/ioctl.h>
#define INVALID_SOCKET  (~0)
#define rt_closesocket(x) close(x)

#if defined(RT_WEBOS_ARM) || defined(ANDROID_NDK)
	#include <linux/sockios.h>
	#include <errno.h>
#else
	#include <sys/sockio.h>
#endif



#else
	
	#include "winsock.h"
#ifndef ECONNREFUSED
	#define ECONNREFUSED            WSAECONNREFUSED
#endif

	#define rt_closesocket(x) closesocket(x)

#endif

NetSocket::NetSocket()
{
	m_socket = INVALID_SOCKET;
}

NetSocket::~NetSocket()
{
	Kill();
}

#define C_NET_MAXHOSTNAME 254

void NetSocket::Kill()
{
	if (m_socket != INVALID_SOCKET)
	{
		#ifdef _DEBUG
		//LogMsg("Killed socket %d", m_socket);
		#endif
		rt_closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}

	m_readBuffer.clear();
	m_writeBuffer.clear();

}
bool NetSocket::Init( string url, int port )
{
	Kill();
	//connect to another one

	m_idleTimer = GetBaseApp()->GetTick();
	struct sockaddr_in sa;
	struct hostent     *hp;
	
	if ((hp= gethostbyname(url.c_str())) == NULL) 
	{
		/* do we know the host's */
		errno= ECONNREFUSED;                       /* address? */
		return false;                                /* no */
	}

	memset(&sa,0,sizeof(sa));
	memcpy((char *)&sa.sin_addr,hp->h_addr,hp->h_length);     /* set address */
	sa.sin_family= hp->h_addrtype;
	sa.sin_port= htons((u_short)port);

	if ((m_socket= socket(hp->h_addrtype,SOCK_STREAM,0)) < 0)     /* get socket */
		return false;

#ifdef WIN32

	//u_long iMode = 0;
	//ioctlsocket(m_socket, FIOASYNC, &iMode);

	WSAAsyncSelect(m_socket, GetForegroundWindow(), WM_USER+1, FD_READ); 
	WSAAsyncSelect(m_socket, GetForegroundWindow(), WM_USER+1, FD_WRITE); 
	WSAAsyncSelect(m_socket, GetForegroundWindow(), WM_USER+1, FD_CONNECT); 
	WSAAsyncSelect(m_socket, GetForegroundWindow(), WM_USER+1, FD_OOB); 

#else
		fcntl (m_socket, F_SETFL, O_NONBLOCK);
	
#endif
	
	int ret = connect(m_socket,(struct sockaddr *)&sa,sizeof sa);
#ifdef _DEBUG
	//LogMsg("Setup socket (%d)", ret);
#endif


	return true;
}

void NetSocket::Update()
{	
	UpdateRead();
	UpdateWrite();
}

void NetSocket::UpdateRead()
{
	if (m_socket == INVALID_SOCKET) return;
		
	vector <char> buff;
	buff.resize(1024);
	int bytesRead;

	do
	{
		bytesRead = ::recv (m_socket, &buff[0], buff.size(), 0);
	
		if (bytesRead <= 0)
		{
			//all done
			return;
		}

		//copy it into our real lbuffer
		m_readBuffer.insert(m_readBuffer.end(), buff.begin(), buff.begin()+bytesRead);
		#ifdef _DEBUG
		//LogMsg("Read %d bytes", bytesRead);
#ifdef WIN32
		//LogMsg(&buff[0]);  //can't really do this, because %'s will crash it
		//OutputDebugString(&buff[0]);
		//OutputDebugString("\n");
#endif
		#endif
		m_idleTimer = GetBaseApp()->GetTick();

	} while (bytesRead >= int(buff.size()));

}

void NetSocket::UpdateWrite()
{
	
	if (m_socket == INVALID_SOCKET || m_writeBuffer.empty()) return;

	int bytesWritten = ::send (m_socket, &m_writeBuffer[0], m_writeBuffer.size(), 0);

	if (bytesWritten <= 0)
	{
		//socket probably hasn't connected yet
		return;
	}
	m_writeBuffer.erase(m_writeBuffer.begin(), m_writeBuffer.begin()+bytesWritten);
	m_idleTimer = GetBaseApp()->GetTick();

#ifdef _DEBUG
	//LogMsg("wrote %d, %d left", bytesWritten, m_writeBuffer.size());
#endif
}

void NetSocket::Write( const string &msg )
{
	if (msg.empty()) return;
	m_writeBuffer.insert(m_writeBuffer.end(), msg.begin(), msg.end());
	UpdateWrite();
}

int NetSocket::GetIdleTimeMS()
{
	return GetBaseApp()->GetTick()-m_idleTimer;
}