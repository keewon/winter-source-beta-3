//////////////////////////////////////////////////////////////////
//
//  Client 
//  채팅할 때 클라이언트 쪽...
//
///////////////////////////////////////////////////////////////////
#include "Client.h"
#include "resource.h"
#include "MsgQueue.h"

// static variable 다룰 때에 특히 조심!
int Client::nFlag = 0;
int Client::nOpponentType = 2;
bool Client::bConnected = false;
extern HINSTANCE instance;

namespace cc {
	
const int MENU_GO = 1;
const int MENU_STOP = 2;
const int MENU_NEXT = 3;

Client* client;
HWND hWnd;	    // Handle of window
		    // 핸들의 개념에 대해 알아둘 필요가 있음
		    // Visual C++책이나 API 책을 볼 것!
		    // Visual Basic 책이나 C++ Builder책에도 나옴
MsgQueue msgQueue(1024);

// 채팅한 메시지가 담겨있는 리스트
// 사실 이것도 좀 구리구리 한데.. 그래도 이건 쓸만한 편일지도..
void AddToChatList(std::string s) {
	for (int i=0; i<7; i++) {
		client->strChatList[i] = client->strChatList[i+1];
	}
	client->strChatList[7] = s;
	RECT rect; rect.left = 40; rect.right = 262; rect.top = 68; rect.bottom = 348;
	//RECT rect; rect.left = 253; rect.right = 400; rect.top = 60; rect.bottom = 250;
	InvalidateRect(hWnd, &rect, false);
}


// 네트워크 메시지가 올 때 불리는 함수..
int 
funcChat(const SOCKET nSocketID) {
	if ((int)nSocketID < 0) {
		Client::bConnected = false;
		AddToChatList("Connection closed");
		return 0;
	}
	
	char szTemp[1000]; memset(szTemp, 0, 1000);
	int n = recv(nSocketID, szTemp, 1000, 0);
	
	if (n > 0) msgQueue.Push(szTemp, n);

	return 0;
}

};	// namespace cc


LRESULT
Client::
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CHAR:
		// 사용자가 입력한 키는 전부 채팅 메시지가 된다.
		{
			using namespace cc;
			char cKey = (char)wParam;

			// 엔터키가 입력되면 메시지를 보낸다.
			if (cKey == 13) {	// VK_RETURN
				if (m_nClientSocketID > 0) {
				    char szPacket[Protocol::MAX_CHAT_LENGTH+1];
				    szPacket[0] = np_CMD::CMD_CHAT;
				    strMyMessage.resize(Protocol::MAX_CHAT_LENGTH,0);
				    memcpy(szPacket+1, strMyMessage.data(), Protocol::MAX_CHAT_LENGTH);
				    if (0 > send(m_nClientSocketID, szPacket, Protocol::MAX_CHAT_LENGTH+1, 0)) {
					    MessageBox(hWnd, "Can't send", "Client", MB_OK);
				    }
				} else {
					MessageBox(hWnd, "Client is not connected?", "babo", MB_OK);
				}
				cc::AddToChatList(strMyMessage);
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
			// 창기반 프로그램에서 다시 그리라고 하는 명령
		}
		break;
	case WM_KEYUP:
		{
			int nKey = (int)wParam;
			// Function Key 제어는 여기서 한다.
			// 예를 들면 F5를 누를 때에 Go가 된다거나 하게 만들 수 있다.
			//if (nKey == VK_F1) MessageBox(hWnd, "F1", "HAJE", MB_OK);
			if (nKey == VK_F2) {

						// 끝내는 버튼
						nFlag = cc::MENU_STOP;
						// CMD_QUIT 보내기
						if (bConnected){
						char szPacket[1];
						szPacket[0] = np_CMD::CMD_QUIT;
						if (-1 == send(m_nClientSocketID, szPacket, 1, 0)) {
							cc::AddToChatList("Error : Can't connect to Server");
						}
						bConnected = false;
						}
			}
		}
		break;

	case WM_PAINT:
		// 그림으로 된 메뉴와 배경을 그려준다
		{
		    // DC의 개념을 알아야 함.. 책 찾아볼 것.
		    {
			HDC hdc = GetDC(hWnd);
			HDC memdc = CreateCompatibleDC(hdc);
			SelectObject(memdc, m_hbitmapBack);
			RECT r; GetClientRect(hWnd, &r);
			BitBlt(hdc, 0,0, r.right, r.bottom, memdc, 0,0,SRCCOPY);
			DeleteDC(memdc);

			HDC memdc2 = CreateCompatibleDC(hdc);
			SelectObject(memdc2, m_hbitmapX);
			BitBlt(hdc, 444, 60,474, 90, memdc2, 0,0,SRCCOPY);
			BitBlt(hdc, 444, 130,474, 160, memdc2, 0,0,SRCCOPY);
			BitBlt(hdc, 444, 196,474, 226, memdc2, 0,0,SRCCOPY);
			BitBlt(hdc, 444, 263,474, 293, memdc2, 0,0,SRCCOPY);
			BitBlt(hdc, 444, 400,474, 430, memdc2, 0,0, SRCCOPY);
			DeleteDC(memdc2);
		    }

		    {
			
			HDC hdc = GetDC(hWnd);
			HDC memdc = CreateCompatibleDC(hdc);
			/*
			SelectObject(memdc, m_hbitmapOpponent);
			BitBlt(hdc, 335,0, 410,75, memdc, 0,0,SRCAND);
			*/

			// 글자들도 찍는다.
			SetBkMode(hdc, TRANSPARENT);
		    Rectangle(hdc, 34, 410, 390, 440);
		    TextOut(hdc, 40,414, strMyMessage.c_str(), strMyMessage.length());

		    for (int i=0; i<8; i++) {
			    TextOut(hdc, 40, 68+20*i, strChatList[i].c_str(), strChatList[i].length());
		    }

		    DeleteDC(memdc);
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
					case 3:
						//nFlag = cc::MENU_GO;

						break;
					case 4:
						// 끝내는 버튼
						nFlag = cc::MENU_STOP;
						// CMD_QUIT 보내기
						if (bConnected){
						char szPacket[1];
						szPacket[0] = np_CMD::CMD_QUIT;
						if (-1 == send(m_nClientSocketID, szPacket, 1, 0)) {
							cc::AddToChatList("Error : Can't connect to Server");
						}
						bConnected = false;

						break;
						}
					}
				}
			}			
		}
		break;
	case WM_CLOSE:
		{
			nFlag = cc::MENU_STOP;
			// CMD_QUIT 보내기
			if (bConnected){
				char szPacket[1];
				szPacket[0] = np_CMD::CMD_QUIT;
				if (-1 == send(m_nClientSocketID, szPacket, 1, 0)) {
					cc::AddToChatList("Error : Can't connect to Server");
				}
			bConnected = false;
			}
		}
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);	
}


Client::
Client(std::string name, IP* ip, SOCKET socketID) : 
	strMyName(name), m_lpIPClient(ip), m_nClientSocketID(socketID) {
}

Client::
~Client() {
}

int
Client::Main(HWND hWnd, MSG message) {

	cc::msgQueue.Empty();
    cc::client = this;
    bConnected = true;
    nFlag = 0;
    cc::hWnd = m_hWnd = hWnd;
    m_hbitmapBack = LoadBitmap(instance, MAKEINTRESOURCE(IDB_SERVER_BACK));
	m_hbitmapX = LoadBitmap(instance, MAKEINTRESOURCE(IDB_X) );
    // IDB_SERVER_NET 등은 Bitmap 파일로서 resource.rc 파일에 있다.
    // 그림 파일 바꿀 때에 Client랑 Server에 쓰이는 파일은 
    // Resource에 등록되어있고 실제 게임할 때 필요한 그림파일은 
    // 등록이 안되어 있다.

//    m_hbitmapOpponent = LoadBitmap(instance, MAKEINTRESOURCE(IDB_SERVER_NET));
    //SetFocus(m_hTextChat);

	cc::AddToChatList("네트워크 플레이 하는 방법");
	cc::AddToChatList("-------------------------");
	cc::AddToChatList("서버가 게임을 시작할 때  ");
	cc::AddToChatList("까지 기다린다            ");
	cc::AddToChatList("                         ");
	cc::AddToChatList("아직 버그가 많은 것 같다.");
    InvalidateRect(hWnd, NULL, false);

    (*m_lpIPClient)[m_nClientSocketID] = & cc::funcChat;
    //SetTimer(hWnd, 1, 1000, NULL);
    if (hWnd) 
    {
	while (!nFlag) 
	{
	    if (PeekMessage(& message, NULL, 0, 0, PM_REMOVE)) {
		if (message.message == WM_QUIT) break;
		else {
		    TranslateMessage(& message);
		    DispatchMessage(& message);
		}
	    } 
	    else 
	    {
		timeval tv;
		tv.tv_sec = 0; tv.tv_usec = 100;
		(*m_lpIPClient)(&tv);
		CMD cmd = cc::msgQueue.ReadCmd();
		char cTemp; 
		switch(cmd) 
		{
		    case np_CMD::CMD_GAME_SERVER:
		    {
			//잘못된 패킷 : 그냥 날려버리기
			struct Protocol::GameServerPacket temp;
			// sizeof() + 1인 것은 CMD까지 계산한 값이다
			if (cc::msgQueue.Size() >= sizeof(Protocol::GameServerPacket)+1) {
			    cc::msgQueue.Pop(&cTemp,1);
			    cc::msgQueue.Pop(&temp, sizeof(Protocol::GameServerPacket));
			    cc::AddToChatList("Invalid Packet :: ServerPacket");
			}
		    }
		    break;
		    case np_CMD::CMD_GAME_CLIENT:
		    {
			//잘못된 패킷 : 그냥 날려버리기
			struct Protocol::GameClientPacket temp;
			// sizeof() + 1인 것은 CMD까지 계산한 값이다
			if (cc::msgQueue.Size() >= sizeof(Protocol::GameClientPacket)+1) {
			    cc::msgQueue.Pop(&cTemp,1);
			    cc::msgQueue.Pop(&temp, sizeof(Protocol::GameClientPacket));
			    cc::AddToChatList("Invalid Packet :: ClientPacket");
			}
		    }
		    break;
		    case np_CMD::CMD_ERROR:
		    {
			cc::msgQueue.Pop(&cTemp,1);
			cc::AddToChatList("Error : connection Lost");
			bConnected = false;
			m_lpIPClient->erase(m_nClientSocketID);
			cc::msgQueue.Empty();
			Client::nFlag = cc::MENU_STOP;
		    }
		    break;
		    case np_CMD::CMD_QUIT:
		    {
			cc::msgQueue.Pop(&cTemp,1);
			cc::AddToChatList("Server quits");
			bConnected = false;
			m_lpIPClient->erase(m_nClientSocketID);
			cc::msgQueue.Empty();
		    }
		    break;
		    case np_CMD::CMD_GO:
		    {	//클라이언트가 채팅모드일 때에 GO가 날라오면 게임화면으로 전환한다
				nFlag = cc::MENU_GO;
		    }
		    break;
		    case np_CMD::CMD_NAME:
		    {
			//상대방의 이름이 날라온다
			char szName[Protocol::MAX_CHAT_LENGTH+1];
			if (cc::msgQueue.Size() >= Protocol::MAX_CHAT_LENGTH+1) {
			    cc::msgQueue.Pop(&cTemp,1);
			    cc::msgQueue.Pop(szName, Protocol::MAX_CHAT_LENGTH);
			    szName[Protocol::MAX_CHAT_LENGTH] = 0;
			    strOpponentName = szName;
			    cc::AddToChatList("Server's Name : " + strOpponentName);

			    // 자기 이름도 보낸다
			    char szPacket[Protocol::MAX_CHAT_LENGTH+1];
			    szPacket[0] = np_CMD::CMD_NAME;
			    strMyMessage.resize(Protocol::MAX_CHAT_LENGTH, 0);
			    memcpy(szPacket+1, strMyName.data(), Protocol::MAX_CHAT_LENGTH);
			    // send에러처리하기 귀찮다.. -_- 뭐 별일 없겠지
			    send(m_nClientSocketID, szPacket, Protocol::MAX_CHAT_LENGTH+1, 0);
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
			if (cc::msgQueue.Size() >= Protocol::MAX_CHAT_LENGTH+1) {
			    cc::msgQueue.Pop(&cTemp,1);
			    cc::msgQueue.Pop(szMessage, Protocol::MAX_CHAT_LENGTH);
			    szMessage[Protocol::MAX_CHAT_LENGTH] = 0;
			    std::string s = szMessage;
				s = strOpponentName + " : " + s;
			    cc::AddToChatList(s);
			}
		    }
		    break;
		// +4 byte 명령어 무시하기
		case np_CMD::CMD_0_SCORE:
		case np_CMD::CMD_1_SCORE:
			{
				char szMessage[5];
				if (cc::msgQueue.Size() >= 5) {
					cc::msgQueue.Pop(szMessage, 5);
				}
			}
			break;
		// +2 byte 명령어 무시하기
		case np_CMD::CMD_0_NEXT:
		case np_CMD::CMD_1_NEXT:
			{
				char szMessage[3];
				if (cc::msgQueue.Size() >= 3) {
					cc::msgQueue.Pop(szMessage, 3);
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
			if (cc::msgQueue.Size() >= 2) {
				cc::msgQueue.Pop(szMessage, 2);
		    }
		}
		break;
		// +0byte 명령어 무시하기
		case np_CMD::CMD_WIN:
		case np_CMD::CMD_LOSE:
			{
				char szMessage[1];
				if (cc::msgQueue.Size() >= 1) {
					cc::msgQueue.Pop(szMessage, 1);
				}
			}
			break;
		default:
			cc::msgQueue.Empty();

		}

	    }
	}
    }
    DeleteObject(m_hbitmapBack);
	DeleteObject(m_hbitmapX);
//    DeleteObject(m_hbitmapOpponent);
    (*m_lpIPClient)[m_nClientSocketID] = 0;

    //CloseHandle(hListenThread);
    return nFlag;
}
