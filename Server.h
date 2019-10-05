#ifndef _SERVER_OEDALPHA_H_
#define _SERVER_OEDALPHA_H_

////////////////////////////////////////////////////////////
///
/// 채팅할 때에 서버 하는애
/// 
///  Server.h Server.cpp
///
////////////////////////////////////////////////////////////
#include "protocol.h"
#include <string>	
#include "unit.h"

#include "IP.h"

class Server : public Unit {
private:

	std::string strMyMessage;
	HWND m_hWnd;
	HBITMAP m_hbitmapBack;
	//HBITMAP m_hbitmapOpponent;
	HBITMAP m_hbitmapO;
	HBITMAP m_hbitmapX;

public:
	Server(std::string, IP*, SOCKET);
	~Server();
	int Main(HWND, MSG);
	//HRESULT Restore(void);
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	
	static int nFlag;				// 메뉴 선택의 결과
	static bool bClientConnected;	// 네트웍클라이언트가 연결되었는지의 여부
	static int nOpponentType;		// 상대방의 종류
	std::string strMyName;			// 자기 이름
	std::string strOpponentName;		// 상대방의 이름
	std::string strChatList[8];
	IP* m_lpIPServer;
	SOCKET m_nServerSocketID;
	SOCKET m_nClientSocketID;
};

#endif

