/** 
 @file  win32.c
 @brief ENet Win32 system specific functions
*/
#ifdef _WIN32


#define ENET_BUILDING_LIB 1

#include <stdio.h>

//#include <assert.h>
//#include <WinSock2.h>
  

#include "enet/enet.h"
#include <mmsystem.h>

#define HAS_GETADDRINFO
//#define HAS_INET_PTON

typedef unsigned __int32 uint32_t;

static enet_uint32 timeBase = 0;

int
enet_initialize (void)
{

    WORD versionRequested = MAKEWORD (1, 1);
    WSADATA wsaData;
   
    if (WSAStartup (versionRequested, & wsaData))
       return -1;

    if (LOBYTE (wsaData.wVersion) != 1||
        HIBYTE (wsaData.wVersion) != 1)
    {
       WSACleanup ();
       
       return -1;
    }

    timeBeginPeriod (1);

	

    return 0;
}

void
enet_deinitialize (void)
{
    timeEndPeriod (1);

    WSACleanup ();
}

enet_uint32
enet_host_random_seed (void)
{
    return (enet_uint32) timeGetTime ();
}

enet_uint32
enet_time_get (void)
{
    return (enet_uint32) timeGetTime () - timeBase;
}

void
enet_time_set (enet_uint32 newTimeBase)
{
    timeBase = (enet_uint32) timeGetTime () - newTimeBase;
}

int enet_address_set_host (ENetAddress * address, const char * name)
{
	//Seth copied over from linux

	


#ifdef HAS_GETADDRINFO
	struct addrinfo hints, * resultList = NULL, * result = NULL;

	memset (& hints, 0, sizeof (hints));
	hints.ai_family = AF_UNSPEC;

	if (getaddrinfo (name, NULL, & hints, & resultList) != 0)
		return -1;

	for (result = resultList; result != NULL; result = result -> ai_next)
	{
		if (result -> ai_addr != NULL && result -> ai_addrlen >= sizeof (struct sockaddr_in))
		{
			if (result -> ai_family == AF_INET)
			{
#ifdef _DEBUG

//OutputDebugStringA("protocol: %d sock type: %d", result->ai_protocol, result->ai_socktype);
			//	OutputDebugStringA("hey");

				//OutputDebugString("yo");
				


	

#endif

				struct sockaddr_in * sin = (struct sockaddr_in *) result -> ai_addr;

				((uint32_t *) & address -> host.s6_addr)[0] = 0;
				((uint32_t *) & address -> host.s6_addr)[1] = 0;
				((uint32_t *) & address -> host.s6_addr)[2] = htonl(0xffff);
				((uint32_t *) & address -> host.s6_addr)[3] = sin->sin_addr.s_addr;

				freeaddrinfo (resultList);

				return 0;
			}
			else if(result -> ai_family == AF_INET6)
			{
				struct sockaddr_in6 * sin = (struct sockaddr_in6 *) result -> ai_addr;

				address -> host = sin -> sin6_addr;
				address -> sin6_scope_id = sin -> sin6_scope_id;

				freeaddrinfo (resultList);

				return 0;
			}
		}
	}

	if (resultList != NULL)
		freeaddrinfo (resultList);
return -1; //something wrong
	
#else
	//old crap

	/*
	struct hostent * hostEntry = NULL;
#ifdef HAS_GETHOSTBYNAME_R
    struct hostent hostData;
    char buffer [2048];
    int errnum;

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
    gethostbyname_r (name, & hostData, buffer, sizeof (buffer), & hostEntry, & errnum);
#else
    hostEntry = gethostbyname_r (name, & hostData, buffer, sizeof (buffer), & errnum);
#endif
#else
    hostEntry = gethostbyname (name);
#endif

    if (hostEntry == NULL ||
        hostEntry -> h_addrtype != AF_INET)
    {
#ifdef HAS_INET_PTON
        if (! inet_pton (AF_INET6, name, & address -> host))
#elif _MSC_VER
		assert(!"Need to define HAS_INET_PTON?");
	// TODO FIXME
#else
        if (! inet_aton (name, (struct in_addr *) & address -> host))
#endif
            return -1;
        return 0;
    }

//    address -> host = * (enet_uint32 *) hostEntry -> h_addr_list [0];

    return 0;
	*/
#endif
}

int
enet_address_get_host_ip (const ENetAddress * address, char * name, size_t nameLength)
{
#ifdef HAS_INET_NTOP
    if (inet_ntop (AF_INET6, & address -> host, name, nameLength) == NULL)
#else
    char * addr = inet_ntoa (* (struct in_addr *) & address -> host);
    if (addr != NULL)
    {
        size_t addrLen = strlen(addr);
        if (addrLen >= nameLength)
          return -1;
        memcpy (name, addr, addrLen + 1);
    } 
    else
#endif
        return -1;
    return 0;
}

int
enet_address_get_host (const ENetAddress * address, char * name, size_t nameLength)
{
    struct in6_addr in;
    struct hostent * hostEntry = NULL;
#ifdef HAS_GETHOSTBYADDR_R
    struct hostent hostData;
    char buffer [2048];
    int errnum;

    in = address -> host;

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
    gethostbyaddr_r ((char *) & in, sizeof (struct in6_addr), AF_INET6, & hostData, buffer, sizeof (buffer), & hostEntry, & errnum);
#else
    hostEntry = gethostbyaddr_r ((char *) & in, sizeof (struct in6_addr), AF_INET6, & hostData, buffer, sizeof (buffer), & errnum);
#endif
#else
    in = address -> host;

    hostEntry = gethostbyaddr ((char *) & in, sizeof (struct in6_addr), AF_INET6);
#endif

    if (hostEntry == NULL)
      return enet_address_get_host_ip (address, name, nameLength);
    else
    {
       size_t hostLen = strlen (hostEntry -> h_name);
       if (hostLen >= nameLength)
         return -1;
       memcpy (name, hostEntry -> h_name, hostLen + 1);
    }

    return 0;
}

int
enet_socket_bind (ENetSocket socket, const ENetAddress * address)
{
    struct sockaddr_in6 sin;

    memset (& sin, 0, sizeof (struct sockaddr_in6));

    sin.sin6_family = AF_INET6;

    if (address != NULL)
    {
       sin.sin6_port = ENET_HOST_TO_NET_16 (address -> port);
       sin.sin6_addr = address -> host;
       sin.sin6_scope_id = address -> sin6_scope_id;
    }
    else
    {
       sin.sin6_port = 0;
       sin.sin6_addr = in6addr_any;
       sin.sin6_scope_id = 0;
    }

    return bind (socket,
                 (struct sockaddr *) & sin,
                 sizeof (struct sockaddr_in6)) == SOCKET_ERROR ? -1 : 0;
}

int
enet_socket_get_address (ENetSocket socket, ENetAddress * address)
{
    struct sockaddr_in6 sin;
    int sinLength = sizeof (struct sockaddr_in6);

    if (getsockname (socket, (struct sockaddr *) & sin, & sinLength) == -1)
      return -1;

    address -> host = sin.sin6_addr;
    address -> port = ENET_NET_TO_HOST_16 (sin.sin6_port);
    address -> sin6_scope_id = sin.sin6_scope_id;

    return 0;
}

int
enet_socket_listen (ENetSocket socket, int backlog)
{
    return listen (socket, backlog < 0 ? SOMAXCONN : backlog) == SOCKET_ERROR ? -1 : 0;
}

ENetSocket
enet_socket_create (ENetSocketType type)
{
    return socket (PF_INET6, type == ENET_SOCKET_TYPE_DATAGRAM ? SOCK_DGRAM : SOCK_STREAM, 0);
}

int
enet_socket_set_option (ENetSocket socket, ENetSocketOption option, int value)
{
    int result = SOCKET_ERROR;
    switch (option)
    {
        case ENET_SOCKOPT_NONBLOCK:
        {
            u_long nonBlocking = (u_long) value;
            result = ioctlsocket (socket, FIONBIO, & nonBlocking);
            break;
        }

        case ENET_SOCKOPT_BROADCAST:
            result = setsockopt (socket, SOL_SOCKET, SO_BROADCAST, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_REUSEADDR:
            result = setsockopt (socket, SOL_SOCKET, SO_REUSEADDR, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_RCVBUF:
            result = setsockopt (socket, SOL_SOCKET, SO_RCVBUF, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_SNDBUF:
            result = setsockopt (socket, SOL_SOCKET, SO_SNDBUF, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_RCVTIMEO:
            result = setsockopt (socket, SOL_SOCKET, SO_RCVTIMEO, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_SNDTIMEO:
            result = setsockopt (socket, SOL_SOCKET, SO_SNDTIMEO, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_NODELAY:
            result = setsockopt (socket, IPPROTO_TCP, TCP_NODELAY, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_IPV6_V6ONLY:
            result = setsockopt (socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *) & value, sizeof (int));
            break;

        default:
            break;
    }
    return result == SOCKET_ERROR ? -1 : 0;
}

int
enet_socket_get_option (ENetSocket socket, ENetSocketOption option, int * value)
{
    int result = SOCKET_ERROR, len;
    switch (option)
    {
        case ENET_SOCKOPT_ERROR:
            len = sizeof(int);
            result = getsockopt (socket, SOL_SOCKET, SO_ERROR, (char *) value, & len);
            break;

        default:
            break;
    }
    return result == SOCKET_ERROR ? -1 : 0;
}

int
enet_socket_connect (ENetSocket socket, const ENetAddress * address)
{
    struct sockaddr_in6 sin;
    int result;

    memset (& sin, 0, sizeof (struct sockaddr_in6));

    sin.sin6_family = AF_INET6;
    sin.sin6_port = ENET_HOST_TO_NET_16 (address -> port);
    sin.sin6_addr = address -> host;
    sin.sin6_scope_id = address -> sin6_scope_id;

    result = connect (socket, (struct sockaddr *) & sin, sizeof (struct sockaddr_in6));
    if (result == SOCKET_ERROR && WSAGetLastError () != WSAEWOULDBLOCK)
      return -1;

    return 0;
}

ENetSocket
enet_socket_accept (ENetSocket socket, ENetAddress * address)
{
    SOCKET result;
    struct sockaddr_in6 sin;
    int sinLength = sizeof (struct sockaddr_in6);

    result = accept (socket, 
                     address != NULL ? (struct sockaddr *) & sin : NULL, 
                     address != NULL ? & sinLength : NULL);

    if (result == INVALID_SOCKET)
      return ENET_SOCKET_NULL;

    if (address != NULL)
    {
        address -> host = sin.sin6_addr;
        address -> port = ENET_NET_TO_HOST_16 (sin.sin6_port);
        address -> sin6_scope_id = sin.sin6_scope_id;
    }

    return result;
}

int
enet_socket_shutdown (ENetSocket socket, ENetSocketShutdown how)
{
    return shutdown (socket, (int) how) == SOCKET_ERROR ? -1 : 0;
}

void
enet_socket_destroy (ENetSocket socket)
{
    if (socket != INVALID_SOCKET)
      closesocket (socket);
	//OutputDebugStringA ("Error sending outgoing packets");

}


int
enet_socket_send (ENetSocket socket,
                  const ENetAddress * address,
                  const ENetBuffer * buffers,
                  size_t bufferCount)
{
 
	
	struct sockaddr_in6 sin;
	
	
	DWORD sentLength;
	
    if (address != NULL)
    {
        memset (& sin, 0, sizeof (struct sockaddr_in6));

        sin.sin6_family = AF_INET6;
        sin.sin6_port = ENET_HOST_TO_NET_16 (address -> port);
        sin.sin6_addr = address -> host;
        sin.sin6_scope_id = address -> sin6_scope_id;
    }

    if (WSASendTo (socket, 
                   (LPWSABUF) buffers,
                   (DWORD) bufferCount,
                   & sentLength,
                   0,
                   address != NULL ? (struct sockaddr *) & sin : NULL,
                   address != NULL ? sizeof (struct sockaddr_in6) : 0,
                   NULL,
                   NULL) == SOCKET_ERROR)
    {
       if (WSAGetLastError () == WSAEWOULDBLOCK)
         return 0;

       return -1;
    }

    return (int) sentLength;
}

int
enet_socket_receive (ENetSocket socket,
                     ENetAddress * address,
                     ENetBuffer * buffers,
                     size_t bufferCount)
{

    INT sinLength = sizeof (struct sockaddr_in6);
   
	
	DWORD flags = 0,
          recvLength;
    struct sockaddr_in6 sin;

    if (WSARecvFrom (socket,
                     (LPWSABUF) buffers,
                     (DWORD) bufferCount,
                     & recvLength,
                     & flags,
                     address != NULL ? (struct sockaddr *) & sin : NULL,
                     address != NULL ? & sinLength : NULL,
                     NULL,
                     NULL) == SOCKET_ERROR)
    {
       switch (WSAGetLastError ())
       {
       case WSAEWOULDBLOCK:
       case WSAECONNRESET:
          return 0;
       }

       return -1;
    }

    if (flags & MSG_PARTIAL)
      return -1;

    if (address != NULL)
    {
        address -> host = sin.sin6_addr;
        address -> port = ENET_NET_TO_HOST_16 (sin.sin6_port);
        address -> sin6_scope_id = sin.sin6_scope_id;
    }


    return (int) recvLength;
}

int
enet_socketset_select (ENetSocket maxSocket, ENetSocketSet * readSet, ENetSocketSet * writeSet, enet_uint32 timeout)
{
    struct timeval timeVal;

    timeVal.tv_sec = timeout / 1000;
    timeVal.tv_usec = (timeout % 1000) * 1000;

	
    return select (maxSocket + 1, readSet, writeSet, NULL, & timeVal);
}

int
enet_socket_wait (ENetSocket socket, enet_uint32 * condition, enet_uint32 timeout)
{
    fd_set readSet, writeSet;
    struct timeval timeVal;
    int selectCount;
    
    timeVal.tv_sec = timeout / 1000;
    timeVal.tv_usec = (timeout % 1000) * 1000;
    
    FD_ZERO (& readSet);
    FD_ZERO (& writeSet);

    if (* condition & ENET_SOCKET_WAIT_SEND)
      FD_SET (socket, & writeSet);

    if (* condition & ENET_SOCKET_WAIT_RECEIVE)
      FD_SET (socket, & readSet);

    selectCount = select (socket + 1, & readSet, & writeSet, NULL, & timeVal);

    if (selectCount < 0)
      return -1;

    * condition = ENET_SOCKET_WAIT_NONE;

    if (selectCount == 0)
      return 0;

    if (FD_ISSET (socket, & writeSet))
      * condition |= ENET_SOCKET_WAIT_SEND;
    
    if (FD_ISSET (socket, & readSet))
      * condition |= ENET_SOCKET_WAIT_RECEIVE;

	
    return 0;
} 

#endif

