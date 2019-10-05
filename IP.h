#ifndef _IP_h_
#define _IP_h_

#include <windows.h>
#include "ISO.h"
#include <map>

class IP
{
private:
    static unsigned int counter;
protected:
    typedef std::map <SOCKET, int (*)(const SOCKET)> map;

    map server, client;
public:
    SOCKET UDP(int (*const function)(const SOCKET), const u_short port);
    SOCKET TCP(int (*const function)(const SOCKET), const u_short port);
    SOCKET TCP(int (*const function)(const SOCKET), const u_short port, const char *const server);
    bool empty(void);
    void erase(const SOCKET socketID);
    inline void syslog(const int priority, const char *const message) { };
    int operator ()(const struct timeval *const timeout = 0);
    map::referent_type & operator [](const SOCKET socketID);

    IP(HWND hWnd = 0);
    virtual ~IP(void);
};

#endif//_IP_h_