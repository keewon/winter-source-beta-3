#ifndef _TERMINAL_H_OEDALPHA_
#define _TERMINAL_H_OEDALPHA_

// 서버에 접속해서 게임하는 경우 실행되는 놈
// 서버의 Game과 대응되는 놈이어야 한다

#include "unit.h"
#include "localplayer.h"
#include "ip.h"

class Terminal : public Unit {
    private:
	LocalPlayer* m_player;
	IP* m_ip;
	SOCKET m_socket;

    public:
	Terminal(LocalPlayer*, IP*, SOCKET);
	~Terminal();
	int Main(HWND hWnd, MSG message);
	LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
	static int nFlag;
};
#endif
