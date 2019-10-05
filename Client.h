#ifndef _CLIENT_OEDALPHA_H_
#define _CLIENT_OEDALPHA_H_

//////////////////////////////////////////////////////////////////
//
//  Client.h
//  채팅할 때 클라이언트 쪽...
//  Server.h 랑 비슷한 계급이다
//
///////////////////////////////////////////////////////////////////

#include "protocol.h"
#include <string>	
#include "unit.h"

#include "IP.h"

class Client : public Unit {
private:

	std::string strMyMessage;
	HWND m_hWnd;
	HBITMAP m_hbitmapBack;
	HBITMAP m_hbitmapX;
	//HBITMAP m_hbitmapOpponent;
	SOCKET m_nClientSocketID;

public:
	Client(std::string, IP*, SOCKET);
	~Client();
	int Main(HWND, MSG);
	//HRESULT Restore(void);
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	
	static int nFlag;				// 메뉴 선택의 결과
	static bool bConnected;	// 네트웍클라이언트가 연결되었는지의 여부
	static int nOpponentType;		// 상대방의 종류
	std::string strMyName;			// 자기 이름
	std::string strOpponentName;		// 상대방의 이름
	std::string strChatList[8];
	IP* m_lpIPClient;
};

#endif

