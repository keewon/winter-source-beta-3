#include "IP.h"
#include <set>

#define LOG_ERR 3

unsigned int IP::counter(0);

SOCKET
IP::
UDP(int (*const function)(const SOCKET), const u_short port)
{
    struct sockaddr_in address;
    size_t length(sizeof(struct sockaddr));
    SOCKET socketID(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));

    if (socketID == INVALID_SOCKET)
    {
	syslog(LOG_ERR, "socket in IP::UDP\n");
	return INVALID_SOCKET;
    }
    memset((void *) & address, 0, length);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    if (bind(socketID, (struct sockaddr *) & address, length) == SOCKET_ERROR)
    {
	syslog(LOG_ERR, "bind in IP::UDP\n");
	closesocket(socketID);
	return INVALID_SOCKET;
    }
    client[socketID] = function;

    return socketID;
}

SOCKET
IP::
TCP(int (*const function)(const SOCKET), const u_short port)
{
    struct sockaddr_in address;
    size_t length(sizeof(struct sockaddr));
    SOCKET socketID(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

    if (socketID == INVALID_SOCKET)
    {
	syslog(LOG_ERR, "socket in IP::TCP\n");
	return INVALID_SOCKET;
    }
    memset((void *) & address, 0, length);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    if (bind(socketID, (struct sockaddr *) & address, length) == SOCKET_ERROR)
    {
	syslog(LOG_ERR, "bind in IP::TCP\n");
	closesocket(socketID);
	return INVALID_SOCKET;
    }
    if (listen(socketID, SOMAXCONN) == SOCKET_ERROR)
    {
	syslog(LOG_ERR, "listen in IP::TCP\n");
	closesocket(socketID);
	return INVALID_SOCKET;
    }
    server[socketID] = function;

    return socketID;
}

SOCKET
IP::
TCP(int (*const function)(const SOCKET), const u_short port, const char *const server)
{
    struct sockaddr_in address;
    size_t length(sizeof(struct sockaddr));
    SOCKET socketID(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

    if (socketID == INVALID_SOCKET)
    {
	syslog(LOG_ERR, "socket in IP::TCP\n");
	return INVALID_SOCKET;
    }
    memset((void *) & address, 0, length);
    address.sin_family = AF_INET;           
    address.sin_addr.s_addr = inet_addr(server); 
    address.sin_port = htons(port);
    if (connect(socketID, (struct sockaddr *) & address, length) == SOCKET_ERROR)
    {
	syslog(LOG_ERR, "connect in IP::TCP\n");
	closesocket(socketID);
	return INVALID_SOCKET;
    }
    client[socketID] = function;

    return socketID;
}

bool
IP::
empty(void)
{
    if (client.find(INVALID_SOCKET) != client.end())
	client.erase(INVALID_SOCKET);

    return server.empty() && client.empty();
}

void
IP::
erase(const SOCKET socketID)
{
    if (server.find(socketID) != server.end())
	server.erase(socketID);
    if (client.find(socketID) != client.end())
	client.erase(socketID);
    closesocket(socketID);
}

int
IP::
operator ()(const struct timeval *const timeout)
{
    SOCKET maxID(0);
    fd_set FileDescriptors;

    FD_ZERO(& FileDescriptors);
    if (client.find(INVALID_SOCKET) != client.end())
	client.erase(INVALID_SOCKET);
    for (map::const_iterator i(server.begin()); i != server.end(); ++i)
    {
	FD_SET(i->first, & FileDescriptors);
	if (maxID < i->first)
	    maxID = i->first;
    }
    for (map::const_iterator i(client.begin()); i != client.end(); ++i)
    {
	FD_SET(i->first, & FileDescriptors);
	if (maxID < i->first)
	    maxID = i->first;
    }
    if (select(maxID + 1, & FileDescriptors, 0, 0, const_cast <struct timeval *>(timeout)) == SOCKET_ERROR)
    {
	syslog(LOG_ERR, "select in IP::operator ()\n");
	return SOCKET_ERROR;
    }

    std::set <SOCKET> sockets;

    for (map::const_iterator i(client.begin()); i != client.end(); ++i)
	if (FD_ISSET(i->first, & FileDescriptors) && i->second != 0)
	    if ((* i->second)(i->first) == SOCKET_ERROR)
		sockets.insert(i->first);
    for (map::const_iterator i(server.begin()); i != server.end(); ++i)
	if (FD_ISSET(i->first, & FileDescriptors))
	{
	    struct sockaddr_in address;
	    size_t length(sizeof(address));
	    SOCKET socketID(accept(i->first, (struct sockaddr *) & address, (int *) & length));

	    if (socketID == INVALID_SOCKET)
	    {
		syslog(LOG_ERR, "accept in IP::operator ()\n");
		continue;
	    }
	    if (maxID < socketID)
		maxID = socketID;
	    client[socketID] = i->second;
	    if (i->second != 0 && (* i->second)(socketID) == SOCKET_ERROR)
		sockets.insert(socketID);
	}
    for (std::set <SOCKET>::const_iterator i(sockets.begin()); i != sockets.end(); ++i)
	erase(* i);
    FD_ZERO(& FileDescriptors);

    return 0;
}

IP::map::referent_type &
IP::
operator [](const SOCKET socketID)
{
    if (server.find(socketID) != server.end())
	return server[socketID];
    if (client.find(socketID) != client.end())
	return client[socketID];
    return client[INVALID_SOCKET];
}

IP::
IP(HWND hWnd)
{
    if (counter++ == 0)
    {
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
	{
	    if (hWnd != 0)
		MessageBox(hWnd, "WSAStartup", "Winsock", MB_OK);
	    WSACleanup();
	}
    }
}

IP::
~IP(void)
{
    std::set <SOCKET> sockets;

    for (map::const_iterator i(server.begin()); i != server.end(); ++i)
    {
	closesocket(i->first);
	sockets.insert(i->first);
    }
    for (std::set <SOCKET>::const_iterator i(sockets.begin()); i != sockets.end(); ++i)
	server.erase(* i);
    sockets.clear();
    for (map::const_iterator i(client.begin()); i != client.end(); ++i)
    {
	closesocket(i->first);
	sockets.insert(i->first);
    }
    for (std::set <SOCKET>::const_iterator i(sockets.begin()); i != sockets.end(); ++i)
	client.erase(* i);
    if (--counter == 0)
	WSACleanup();
}