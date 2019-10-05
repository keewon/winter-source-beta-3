////////////////////////////////////////////////////////////
///
/// 채팅할 때에 서버 하는애
/// 
///  Server.h Server.cpp
///
////////////////////////////////////////////////////////////
#include "Server.h"
#include "resource.h"
#include "MsgQueue.h"

int Server::nFlag = 0;
int Server::nOpponentType = 0;
bool Server::bClientConnected = false;
extern HINSTANCE instance;

namespace ss {
	
const int MENU_GO = 1;
const int MENU_STOP = 2;
const int MENU_NEXT = 3;

Server* server;
HWND hWnd;
MsgQueue msgQueue(1024);

void AddToChatList(std::string s) {
	for (int i=0; i<7; i++) {
		server->strChatList[i] = server->strChatList[i+1];
	}
	server->strChatList[7] = s;
	RECT rect; rect.left = 40; rect.right = 262; rect.top = 68; rect.bottom = 348;
	InvalidateRect(hWnd, &rect, false);
}

int funcChat(const SOCKET);

// Socket 프로그래밍에 대한 책을 좀 읽어보는 것이 좋을 듯..
// 미안해.. 짧은 기간에 너무 많은 것을 요구해서..
// 서버 쪽에서는 일단 기다리고 있다가 접속 요구가 오면 
// Accept를 하거든 .. 그걸 funcAccept에서 처리하며
// 한번 접속을 허용한 다음에는 funcChat에서 처리하는 거지
// 이 게임은 서버하나에 클라이언트가 하나만 붙기 때문에
// 이렇게 짜도 되는 거지..

int 
funcAccept(const SOCKET nSocketID) {
    AddToChatList("Somebody wants to connect");
    if (Server::bClientConnected == false) 
    {
	if (nSocketID == server->m_nServerSocketID) return -1;
	Server::bClientConnected = true;
	server->m_nClientSocketID = nSocketID;
	char szTemp[100];
	sprintf(szTemp, "connected! socketID = %d", nSocketID);
	AddToChatList(szTemp);

	// CMD_NAME 보내기
	char szPacket[Protocol::MAX_CHAT_LENGTH+1];
	szPacket[0] = np_CMD::CMD_NAME;
	server->strMyName.resize(Protocol::MAX_CHAT_LENGTH, 0);
	memcpy(szPacket+1, server->strMyName.data(), Protocol::MAX_CHAT_LENGTH);
	// send에러처리하기 귀찮다.. -_- 뭐 별일 없겠지
	send(server->m_nClientSocketID, szPacket, Protocol::MAX_CHAT_LENGTH+1, 0);

	// ACCEPT끝내고 채팅모드로 들어간다
	// 메시지 핸들링을 funcChat으로 바꿔준다
	(*(server->m_lpIPServer))[nSocketID] = &funcChat;
    }
    else {
	//CMD_ERROR 보내기
	char szPacket[1];
	szPacket[0] = np_CMD::CMD_ERROR;
	// send에러처리하기 귀찮다.. -_- 뭐 별일 없겠지
	send(nSocketID, szPacket, 1, 0);
    }
    return 0;
}


int 
funcChat(const SOCKET nSocketID) {

	if ((int)nSocketID < 0) {
		Server::bClientConnected = false;
		server->m_lpIPServer->erase(server->m_nClientSocketID);
		ss::msgQueue.Empty();
		AddToChatList("Connection closed");
		return 0;
	}
	
	Server::bClientConnected = true;
	server->m_nServerSocketID = nSocketID;
	char szTemp[1000]; memset(szTemp, 0, 1000);
	int n = recv(nSocketID, szTemp, 1000, 0);
	if (n>0) msgQueue.Push(szTemp, n);

	return 0;
}

};	// namespace ss

LRESULT
Server::
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CHAR:
		// 사용자가 입력한 키는 전부 채팅 메시지가 된다.
		{
		    using namespace ss;
		    char cKey = (char)wParam;

		    // 엔터키가 입력되면 메시지를 보낸다.
		    if (cKey == 13) // VK_RETURN
		    {	
			if (bClientConnected) 
			{
			    if (!m_lpIPServer->empty() && m_nClientSocketID > 0) 
			    {
				char szPacket[Protocol::MAX_CHAT_LENGTH+1];
				szPacket[0] = np_CMD::CMD_CHAT;
				strMyMessage.resize(Protocol::MAX_CHAT_LENGTH,' ');
				memcpy(szPacket+1, strMyMessage.data(), Protocol::MAX_CHAT_LENGTH);
				if (-1 == send(m_nClientSocketID, szPacket, Protocol::MAX_CHAT_LENGTH+1, 0)) 
				{
				    ss::AddToChatList("Error : Can't connect to Client");
				    bClientConnected = false;
				    m_lpIPServer->erase(m_nClientSocketID);
				    ss::msgQueue.Empty();
				}

			    } else {
				ss::AddToChatList("Error : Can't connect to Client");
				bClientConnected = false;
				m_lpIPServer->erase(m_nClientSocketID);
				ss::msgQueue.Empty();
			    }
			}
			ss::AddToChatList(strMyMessage);
			RECT rect; rect.left = 40; rect.right = 262; rect.top = 68; rect.bottom = 348;
			InvalidateRect(hWnd, &rect, false);
			strMyMessage = "";
		    }
		    else if (cKey == 8) {	// VK_BACKSPACE
			if (!strMyMessage.empty())
			    strMyMessage.erase(strMyMessage.length()-1, 1);
		    }
		    else if (cKey == 9) {	// VK_TAB	네칸 띄운다
			strMyMessage += "    ";
		    }
		    else if (cKey == 27) {	// VK_ESCAPE : 메시지 취소
			strMyMessage = "";
		    }
		    else if (cKey >= 32 && cKey < 128){
			if (strMyMessage.length() < Protocol::MAX_CHAT_LENGTH-1) {	
			    // strMyMessage[MAX-1]는 항상 0으로 한다
			    strMyMessage += cKey;
			    //char babo[1];
			    //sprintf(babo, "%d", cKey);
			    //MessageBox(hWnd, babo, "HAJE", MB_OK);

			}
		    }
		    RECT rect; rect.left = 355; rect.top = 414; 
		    rect.right = 701, rect.bottom = 435;
		    InvalidateRect(hWnd, &rect, false);
		}
		break;
	case WM_KEYUP:
		{
			int nKey = (int)wParam;
			// Function Key 제어는 여기서 한다.
			if (nKey == VK_F4 || nKey == VK_F5 || nKey == VK_F6 || nKey == VK_F3) {
				//MessageBox(hWnd, "F1", "HAJE", MB_OK);
				switch (nKey) {
				case VK_F3:
					nOpponentType = 0;
					break;
				case VK_F4:
					nOpponentType = 1;
					break;
				case VK_F5:
					nOpponentType = 2;
					break;
				case VK_F6:
					nOpponentType = 3;
					break;

				}
					// 시작하는 버튼
				if (bClientConnected && nOpponentType != 3) {
					// 상대방은 차고 나는 시작
				    // CMD_QUIT 보내기
				    if (bClientConnected){
					char szPacket[1];
					szPacket[0] = np_CMD::CMD_QUIT;
					if (-1 == send(m_nClientSocketID, szPacket, 1, 0)) {
						ss::AddToChatList("Error : Can't connect to Client");
					}
					bClientConnected = false;
					m_lpIPServer->erase(m_nClientSocketID);
				    }
					nFlag = ss::MENU_GO;
				}
				else if (nOpponentType != 3) {
						nFlag = ss::MENU_GO;					
				}
					// 이걸 누르면 CMD_GO를 보내야지
					if (bClientConnected && nOpponentType == 3) {

						// send Go Message
						char szPacket[1];
						szPacket[0] = np_CMD::CMD_GO;
						if (-1 == send(m_nClientSocketID, szPacket, 1, 0))
						{
							m_lpIPServer->erase(m_nClientSocketID);
							ss::msgQueue.Empty();
							nFlag = -1;
						}
						nFlag = ss::MENU_GO;
					}
				return 0;
			}
			else if (nKey == VK_F2/* || nKey == VK_ESCAPE*/) {
				    // 끝내는 버튼
				    nFlag = ss::MENU_STOP;
				    // CMD_QUIT 보내기
				    if (bClientConnected){
					char szPacket[1];
					szPacket[0] = np_CMD::CMD_QUIT;
					if (-1 == send(m_nClientSocketID, szPacket, 1, 0)) {
						ss::AddToChatList("Error : Can't connect to Client");
					}
					bClientConnected = false;
					m_lpIPServer->erase(m_nClientSocketID);
				    }
				return 0;
			}
			/*
			else if (nKey == VK_F5) {
					DeleteObject(m_hbitmapBack);
					if (bClientConnected)
						Server::nOpponentType = 
							(Server::nOpponentType+1) % 4;
					else Server::nOpponentType =
						(Server::nOpponentType+1) % 3;
					int resource;
					switch(Server::nOpponentType) {
					case 0:
						resource = IDB_SERVER_NONE;
						break;
					case 1:
						resource = IDB_SERVER_KEY;
						break;
					case 2:
						resource = IDB_SERVER_COM;
						break;
					case 3:
						resource = IDB_SERVER_NET;
						break;
					}
					m_hbitmapBack = LoadBitmap(instance,
						MAKEINTRESOURCE(resource));
					RECT rect; rect.top = 0; rect.left = 0;
					rect.right = 250; rect.bottom = 480;
					InvalidateRect(hWnd, &rect, false);
			}*/

		}
		break;

	case WM_PAINT:
		// 그림으로 된 메뉴와 배경을 그려준다
		{
		    {
		    HDC hdc = GetDC(hWnd);
		    HDC memdc = CreateCompatibleDC(hdc);
		    SelectObject(memdc, m_hbitmapBack);
		    RECT r; GetClientRect(hWnd, &r);
		    BitBlt(hdc, 0,0, r.right, r.bottom, memdc, 0,0,SRCCOPY);
		    DeleteDC(memdc);

			HDC memdc2 = CreateCompatibleDC(hdc);
			SelectObject(memdc2, m_hbitmapX);
			if (bClientConnected == false) {
				BitBlt(hdc, 444,265,474,295, memdc2, 0,0, SRCCOPY);
			}
			BitBlt(hdc, 444, 400,474, 430, memdc2, 0,0, SRCCOPY);
			DeleteDC(memdc2);


		    }

		    {
		    HDC hdc = GetDC(hWnd);

		    // 글자들도 찍는다.
		    SetBkMode(hdc, TRANSPARENT);
		    Rectangle(hdc, 34, 410, 390, 440);
		    TextOut(hdc, 40,414, strMyMessage.c_str(), strMyMessage.length());

		    for (int i=0; i<8; i++) {
			    TextOut(hdc, 40, 68+20*i, strChatList[i].c_str(), strChatList[i].length());
		    }

		    //DeleteDC(memdc);
		    }

		}
		break;
	case WM_LBUTTONUP:
		// 메뉴선택 이벤트는 마우스를 눌렀다가 띄었을 때에 일어난다
		{
			POINT p;
			//RECT r; GetWindowRect(hWnd, &r);
			GetCursorPos(&p);
			ScreenToClient(hWnd, &p);

			if (p.x > 440 && p.x < 714) {
				if (p.y > 45 && p.y < 375) {
					int menu_num = (p.y-45) / 66;
					switch (menu_num) {
					case 0:
					case 1:
					case 2:
						// Send bye message
						if (bClientConnected) {
							char szPacket[1];
							szPacket[0] = np_CMD::CMD_QUIT;
							if (-1 == send(m_nClientSocketID, szPacket, 1, 0)) {
								ss::AddToChatList("Error : Can't connect to Client");
							}
							bClientConnected = false;
							m_lpIPServer->erase(m_nClientSocketID);
						
						}

						Server::nOpponentType = menu_num;
						nFlag = ss::MENU_GO;
						break;
					case 3:
						if (bClientConnected) {
							Server::nOpponentType = 3;
							
							char szPacket[1];
							szPacket[0] = np_CMD::CMD_GO;
							if (-1 == send(m_nClientSocketID, szPacket, 1, 0))
							{
								m_lpIPServer->erase(m_nClientSocketID);
								ss::msgQueue.Empty();
								nFlag = -1;
							}
							nFlag = ss::MENU_GO;
						}
						break;
					case 4:
						// 끝내는 버튼
						nFlag = ss::MENU_STOP;
						// CMD_QUIT 보내기
						if (bClientConnected){
							char szPacket[1];
							szPacket[0] = np_CMD::CMD_QUIT;
						if (-1 == send(m_nClientSocketID, szPacket, 1, 0)) {
							ss::AddToChatList("Error : Can't connect to Client");
						}
							bClientConnected = false;
							m_lpIPServer->erase(m_nClientSocketID);
						}

						break;
					}
				}
			}
		}
		break;
	case WM_CLOSE:
		nFlag = ss::MENU_STOP;
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);	
}

Server::
Server(std::string name, IP* ip, SOCKET s) {
    	strMyName = name;
	m_lpIPServer = ip;
	m_nServerSocketID = s;
	m_nClientSocketID = -1;
}

Server::
~Server() {
}

int
Server::Main(HWND hWnd, MSG message) {

	ss::msgQueue.Empty();
    ss::server = this;
    bClientConnected = false;
    nFlag = 0;
    nOpponentType = 0;

    ss::hWnd = m_hWnd = hWnd;
    m_hbitmapBack = LoadBitmap(instance, MAKEINTRESOURCE(IDB_SERVER_BACK));
	//m_hbitmapO = LoadBitmap(instance, MAKEINTRESOURCE(IDB_O));
	m_hbitmapX = LoadBitmap(instance, MAKEINTRESOURCE(IDB_X));

	ss::AddToChatList("네트워크 플레이 하는 방법");
	ss::AddToChatList("-------------------------");
	ss::AddToChatList("다른 사용자가 접속하면   ");
	ss::AddToChatList("Network의 x표가 사라진다.");
	ss::AddToChatList("그 때에 F6를 누르면 된다.");
	ss::AddToChatList("아직 버그가 많은 것 같다.");
    InvalidateRect(hWnd, NULL, false);

    (*m_lpIPServer)[m_nServerSocketID] = & ss::funcAccept;

    if (hWnd) 
    {
	while (!nFlag) 
	{
	    if (PeekMessage(& message, NULL, 0, 0, PM_REMOVE)) 
	    {
		if (message.message == WM_QUIT)
		    break;
		else {
		    TranslateMessage(& message);
		    DispatchMessage(& message);
		}
	    } 
	    else 
	    {
		struct timeval tv;
		tv.tv_sec = 0; tv.tv_usec = 100;
		(*m_lpIPServer)(&tv);

		CMD cmd = ss::msgQueue.ReadCmd();
		char cTemp;

		switch(cmd) {
		case np_CMD::CMD_0_BOUND:
		case np_CMD::CMD_1_BOUND:
		case np_CMD::CMD_GAME_SERVER:
		{
		    //잘못된 패킷 : 그냥 날려버리기
		    struct Protocol::GameServerPacket temp;
		    // sizeof() + 1인 것은 CMD까지 계산한 값이다
		    if (ss::msgQueue.Size() >= sizeof(Protocol::GameServerPacket)+1) {
			ss::msgQueue.Pop(&cTemp,1);
			ss::msgQueue.Pop(&temp, sizeof(Protocol::GameServerPacket));
			ss::AddToChatList("Invalid Packet :: ServerPacket");
		    }
		}
		break;
		case np_CMD::CMD_GAME_CLIENT:
		{
		    //잘못된 패킷 : 그냥 날려버리기
		    struct Protocol::GameClientPacket temp;
		    // sizeof() + 1인 것은 CMD까지 계산한 값이다
		    if (ss::msgQueue.Size() >= sizeof(Protocol::GameClientPacket)+1) {
			ss::msgQueue.Pop(&cTemp,1);
			ss::msgQueue.Pop(&temp, sizeof(Protocol::GameClientPacket));
			ss::AddToChatList("Invalid Packet :: ClientPacket");
		    }
		}
		break;
		case np_CMD::CMD_ERROR:
		{
		    ss::msgQueue.Pop(&cTemp,1);
		    //ss::AddToChatList("Error : connection Lost");
		    MessageBox(m_hWnd, "Connection Lost", "Network Error", MB_OK);
			bClientConnected = false;
			ss::msgQueue.Empty();
		    // 다시 listen하기
		    (*m_lpIPServer)[m_nServerSocketID] = & ss::funcAccept;
		}
		break;
		case np_CMD::CMD_QUIT:
		{
		    ss::msgQueue.Pop(&cTemp,1);
		    MessageBox(m_hWnd, "Opponent Quits", "End of Game", MB_OK);
			bClientConnected = false;
			ss::msgQueue.Empty();
		    // 다시 listen하기
		    (*m_lpIPServer)[m_nServerSocketID] = & ss::funcAccept;
		}
		break;
		case np_CMD::CMD_GO:
		{	//서버가 채팅모드일 때에 GO가 날라오는 건 잘못된 패킷
		    //명령만 뽑아내고 무시하기
		    ss::msgQueue.Pop(&cTemp,1);
		    ss::AddToChatList("Invalid Packet :: GO");
		}
		break;
		case np_CMD::CMD_NAME:
		{
		    //상대방의 이름이 날라온다
		    char szName[Protocol::MAX_CHAT_LENGTH+1];
		    if (ss::msgQueue.Size() >= Protocol::MAX_CHAT_LENGTH+1) {
			ss::msgQueue.Pop(&cTemp,1);
			ss::msgQueue.Pop(szName, Protocol::MAX_CHAT_LENGTH);
			szName[Protocol::MAX_CHAT_LENGTH] = 0;
			strOpponentName = szName;
			ss::AddToChatList("Client's Name : " + strOpponentName);

		    }
		}
		break;
		case np_CMD::CMD_NIL:
		{// 아무것도 하지 않는다 
		}
		break;
		case np_CMD::CMD_CHAT:
		{
		    // 채팅 메시지
		    char szMessage[Protocol::MAX_CHAT_LENGTH+1];
		    // CMD까지 합쳐서 계산하는 거라서 MAX_CHAT_LENGTH+1이다
		    if (ss::msgQueue.Size() >= Protocol::MAX_CHAT_LENGTH+1) {
			ss::msgQueue.Pop(&cTemp,1);
			ss::msgQueue.Pop(szMessage, Protocol::MAX_CHAT_LENGTH);
			szMessage[Protocol::MAX_CHAT_LENGTH] = 0;
			std::string s = szMessage;
			s = strOpponentName + " : " + s;
			ss::AddToChatList(s);
		    }
		}
		break;
		// +4 byte 명령어 무시하기
		case np_CMD::CMD_0_SCORE:
		case np_CMD::CMD_1_SCORE:
			{
				char szMessage[5];
				if (ss::msgQueue.Size() >= 5) {
					ss::msgQueue.Pop(szMessage, 5);
				}
			}
			break;
		// +2 byte 명령어 무시하기
		case np_CMD::CMD_0_NEXT:
		case np_CMD::CMD_1_NEXT:
			{
				char szMessage[3];
				if (ss::msgQueue.Size() >= 3) {
					ss::msgQueue.Pop(szMessage, 3);
				}
			}
			break;
		// +1 byte 명령어 무시하기
		case np_CMD::CMD_0_ATTACK:
		case np_CMD::CMD_1_ATTACK:
		case np_CMD::CMD_SOUND:
		{
		    // Sound Message
		    // 뽑아낸 다음에 무시한다.
		    char szMessage[2];
		    if (ss::msgQueue.Size() >= 2) {
			ss::msgQueue.Pop(szMessage, 2);
		    }
		}
		break;
		// +0byte 명령어 무시하기
		case np_CMD::CMD_WIN:
		case np_CMD::CMD_LOSE:
			{
				char szMessage[1];
				if (ss::msgQueue.Size() >= 1) {
					ss::msgQueue.Pop(szMessage, 1);
				}
			}
			break;
		default:
			ss::msgQueue.Empty();

		} //switch
	    }
	}
    }
    (*m_lpIPServer)[m_nServerSocketID] = 0;
    DeleteObject(m_hbitmapBack);
	DeleteObject(m_hbitmapX);
	//DeleteObject(m_hbitmapO);
    //DeleteObject(m_hbitmapOpponent);

    //CloseHandle(hListenThread);
    return nFlag;
}
