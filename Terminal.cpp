// Terminal  - Client behavior
// 다른 놈들을 헤쳐서 다시 모으면 얘가 된다..
// (하루만에 짰음..)
#include "Terminal.h"
#include "MsgQueue.h"
#include "util.h"
#include <fstream>

int Terminal::nFlag = 0;
namespace tt {
	MsgQueue msgQueue(16384);
	std::ofstream ofs;
    int
    func(const SOCKET nSocketID) {
	if ((int)nSocketID < 0) {
	    Terminal::nFlag = -1;
	    return 0;
	}

	char szTemp[1000]; memset(szTemp, 0, 1000);
	int n = recv(nSocketID, szTemp, 1000, 0);
	
	if (n > 0) msgQueue.Push(szTemp, n);

	return 0;
    }

}

Terminal::
Terminal(LocalPlayer* p1, IP* arg_ip, SOCKET arg_s ) : 
	m_player(p1),m_ip(arg_ip), m_socket(arg_s)
{ }

Terminal::
~Terminal()
{ }

int
Terminal::
Main(HWND hWnd, MSG message) {
    nFlag = 0;
	tt::ofs.open("terminal.log");
    int nDrawTick = timeGetTime();

    DirectX::Sprite::view(400,300);

    if (hWnd) 
    {
	//SetTimer(hWnd, 1, 100, NULL);
		//{
		//Box b;
		//m_player->Redraw(1, b); 
		//m_player->Redraw(0, b);
		//}

	    (*m_ip)[m_socket] = & tt::func;

		// send Go Message
		char szPacket[1];
		szPacket[0] = np_CMD::CMD_GO;
		if (-1 == send(m_socket, szPacket, 1, 0))
		{
			m_ip->erase(m_socket);
			tt::msgQueue.Empty();
			nFlag = -1;
		}

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
		(*m_ip)(&tv);

		CMD cmd = tt::msgQueue.ReadCmd();
		if ( (int)cmd != 0 && (int)cmd != np_CMD::CMD_GAME_SERVER
			&& (int)cmd != np_CMD::CMD_NIL) {
			tt::ofs << (int)cmd << std::endl;
		}
		char cTemp;

		switch(cmd) {
		case np_CMD::CMD_GAME_SERVER:
		{
		    // 처리해서 이대로 화면을 보여줘야 한다
		    struct Protocol::GameServerPacket temp;
		    if (tt::msgQueue.Size() >= sizeof(Protocol::GameServerPacket)+1) {
			tt::msgQueue.Pop(&cTemp,1);
			tt::msgQueue.Pop(&temp, sizeof(Protocol::GameServerPacket));

			// 처리는 다 localplayer에서 하도록 하자
			m_player->Redraw(temp);

		    }
		}
		break;
		case np_CMD::CMD_GAME_CLIENT:
		{
		    //잘못된 패킷 : 그냥 날려버리기
		    struct Protocol::GameClientPacket temp;
		    // sizeof() + 1인 것은 CMD까지 계산한 값이다
		    if (tt::msgQueue.Size() >= sizeof(Protocol::GameClientPacket)+1) {
			tt::msgQueue.Pop(&cTemp,1);
			tt::msgQueue.Pop(&temp, sizeof(Protocol::GameClientPacket));
		    }
		}
		break;
		case np_CMD::CMD_ERROR:
		{
		    tt::msgQueue.Pop(&cTemp,1);
		    //tt::AddToChatList("Error : connection Lost");
		    MessageBox(hWnd, "Connection Lost", "Network Error", MB_OK);
		    nFlag = -1;
		}
		break;
		case np_CMD::CMD_QUIT:
		{
		    tt::msgQueue.Pop(&cTemp,1);
		    MessageBox(hWnd, "Opponent Quits", "End of Game", MB_OK);
		    // 게임을 마쳐야 하는데.. 
		    nFlag = -1;
		}
		break;
		case np_CMD::CMD_GO:
		{	//클라이언트가 게임모드일때 GO가 날라오는 건 잘못된 패킷
		    	//명령만 뽑아내고 무시하기
		    tt::msgQueue.Pop(&cTemp,1);
		}
		break;
		case np_CMD::CMD_NAME:
		{
		    //상대방의 이름이 날라온다 - 무시한다
		    char szName[Protocol::MAX_CHAT_LENGTH+1];
		    if (tt::msgQueue.Size() >= Protocol::MAX_CHAT_LENGTH+1) {
			tt::msgQueue.Pop(&cTemp,1);
			tt::msgQueue.Pop(szName, Protocol::MAX_CHAT_LENGTH);
		    }
		}
		break;
		case np_CMD::CMD_NIL:
		{// 아무것도 하지 않는다 
		}
		break;
		case np_CMD::CMD_CHAT:
		{
		    // 채팅 메시지 - 무시한다
		    char szMessage[Protocol::MAX_CHAT_LENGTH+1];
		    // CMD까지 합쳐서 계산하는 거라서 MAX_CHAT_LENGTH+1이다
		    if (tt::msgQueue.Size() >= Protocol::MAX_CHAT_LENGTH+1) {
			tt::msgQueue.Pop(&cTemp,1);
			tt::msgQueue.Pop(szMessage, Protocol::MAX_CHAT_LENGTH);
		    }
		}
		break;
		case np_CMD::CMD_SOUND:
		{
		    // Sound Message
		    // 연주한다
		    char szMessage[1];
		    if (tt::msgQueue.Size() >= 2) {
			tt::msgQueue.Pop(szMessage, 1);
			tt::msgQueue.Pop(szMessage, 1);
			m_player->SoundEffect(szMessage[0]);
		    }
		}
		break;
		case np_CMD::CMD_WIN:
			{
				char szMessage[1];
				if (tt::msgQueue.Size() >= 1) {
				tt::msgQueue.Pop(szMessage, 1);
				// 일반적인 경우와는 반대로 그림을 보여줄 것
				m_player->GameOver(0);
				m_player->Draw();
				nFlag = -1;
				}

			}
			break;
		case np_CMD::CMD_LOSE:
			{
				char szMessage[1];
				if (tt::msgQueue.Size() >= 1) {
				tt::msgQueue.Pop(szMessage, 1);
				// 평상시와 반대로 그림을 보여줄 것
				m_player->GameOver(1);
				m_player->Draw();
				nFlag = -1;
				}

			}
			break;
		case np_CMD::CMD_0_ATTACK:
		    {
			char szMessage[1];
			if (tt::msgQueue.Size() >= 2) {
			    tt::msgQueue.Pop(&cTemp, 1);
			    tt::msgQueue.Pop(szMessage, 1);
				//tt::ofs << "ATTACK0 : " << (int)(szMessage[0]) << std::endl;
			    m_player->Attack(0, (int)(szMessage[0]));
			}
		    }
		    break;
		case np_CMD::CMD_1_ATTACK:
		    {
			char szMessage[1];
			if (tt::msgQueue.Size() >= 2) {
			    tt::msgQueue.Pop(&cTemp, 1);
			    tt::msgQueue.Pop(szMessage, 1);
				//tt::ofs << "ATTACK1 : " << (int)(szMessage[0]) << std::endl;
			    m_player->Attack(1, (int)(szMessage[0]));
			}
		    }
		    break;
		case np_CMD::CMD_0_BOUND:
		    {
		    // 처리해서 이대로 화면을 보여줘야 한다
		    struct Protocol::GameServerPacket temp;
		    if (tt::msgQueue.Size() >= sizeof(Protocol::GameServerPacket)+1) {
			tt::msgQueue.Pop(&cTemp,1);
			tt::msgQueue.Pop(&temp, sizeof(Protocol::GameServerPacket));

			//tt::ofs << (int)(temp.player) << std::endl;
			//for (int k=0; k<10; k++) {
			//	for (int l=0; l<5; l++) {
			//		tt::ofs << (int)(temp.deck[k][l]) << " ";
			//	}
			//	tt::ofs << std::endl;
			//}

			// 처리는 다 localplayer에서 하도록 하자
			m_player->DrawBound(temp);
		    }
		    }
		    break;
		case np_CMD::CMD_1_BOUND:
		    {
		    // 처리해서 이대로 화면을 보여줘야 한다
		    struct Protocol::GameServerPacket temp;
		    if (tt::msgQueue.Size() >= sizeof(Protocol::GameServerPacket)+1) {
			tt::msgQueue.Pop(&cTemp,1);
			tt::msgQueue.Pop(&temp, sizeof(Protocol::GameServerPacket));

			//tt::ofs << (int)(temp.player) << std::endl;
			//for (int k=0; k<10; k++) {
			//	for (int l=0; l<5; l++) {
			//		tt::ofs << (int)(temp.deck[k][l]) << " ";
			//	}
			//	tt::ofs << std::endl;
			//}
			// 처리는 다 localplayer에서 하도록 하자
			m_player->DrawBound(temp);
		    }
		    }
		    break;
		case np_CMD::CMD_0_NEXT:
		    {
			char szMessage[2];
			if (tt::msgQueue.Size() >= 3) {
			    tt::msgQueue.Pop(&cTemp, 1);
			    tt::msgQueue.Pop(szMessage, 2);
			    Cardpair c( (int)(szMessage[0]), (int)(szMessage[1]) );
			    m_player->Next(0, c);
			}
		    }
		    break;
		case np_CMD::CMD_1_NEXT:
		    {
			char szMessage[2];
			if (tt::msgQueue.Size() >= 3) {
			    tt::msgQueue.Pop(&cTemp, 1);
			    tt::msgQueue.Pop(szMessage, 2);
			    Cardpair c( (int)(szMessage[0]), (int)(szMessage[1]) );
			    m_player->Next(1, c);
			}
		    }
		    break;
		case np_CMD::CMD_0_SCORE:
		    {
			char szMessage[4];
			if (tt::msgQueue.Size() >= 5) {
			    tt::msgQueue.Pop(&cTemp, 1);
			    tt::msgQueue.Pop(szMessage, 4);

			    int score = 0;
				//tt::ofs << "SCORE0 : ";
			    for (int i=0; i<4; i++) {
				score = score * 256;
				score = score + (unsigned int)(szMessage[i]);

				//tt::ofs << (unsigned int)(szMessage[i]);
			    }
				//tt::ofs << std::endl;

			    m_player->SetScore(0, score);
			}
		    }
		    break;
		case np_CMD::CMD_1_SCORE:
		    {
			char szMessage[4];
			if (tt::msgQueue.Size() >= 5) {
			    tt::msgQueue.Pop(&cTemp, 1);
			    tt::msgQueue.Pop(szMessage, 4);

			    int score = 0;
				//tt::ofs << "SCORE1 : ";
			    for (int i=0; i<4; i++) {
				score = score * 256;
				score = score + (unsigned int)(szMessage[i]);

				//tt::ofs << (unsigned int)(szMessage[i]);
			    }
				//tt::ofs << std::endl;

			    m_player->SetScore(1, score);
			}
		    }
		    break;
		case np_CMD::CMD_STATUS:
		    {
			char szMessage[2];
			if (tt::msgQueue.Size() >= 3) {
			    tt::msgQueue.Pop(&cTemp, 1);
			    tt::msgQueue.Pop(szMessage, 2);
				int n1 = szMessage[0];
				int n2 = szMessage[1];

				if (128 <= n2 && n2 <= 255) {
					n2 = n2-256;
				}
			    m_player->GameStatus(n1, n2 );
    			}
		    }
		    break;
		default:
			tt::msgQueue.Empty(); // 에러가 있는 듯.. 다 지워버려

		} //switch

		// Key input을 받아서 보내기
		int nKey = (char)(m_player->Input());
		if (nKey != 0) {
		    Protocol::GameClientPacket temp;
		    temp.key = (char)nKey;
		    char szPacket[sizeof(Protocol::GameClientPacket)+1];
		    szPacket[0] = np_CMD::CMD_GAME_CLIENT;
		    memcpy(szPacket+1, &temp, sizeof(Protocol::GameClientPacket));

		    if (-1 == send(m_socket, szPacket, 
				sizeof(Protocol::GameClientPacket)+1, 0))
		    {
			m_ip->erase(m_socket);
			tt::msgQueue.Empty();
			nFlag = -1;
		    }
		}

			int newTick = timeGetTime();

			/*
			if (newTick - nDownTick > 50) {
				nDownFlag0++;
				nDownFlag1++;
				nDownTick = newTick;
			}
			*/
		
			if (newTick - nDrawTick > 100) {
				m_player->keyDown[0].n2++;
				m_player->keyDown[0].n4++;
				m_player->keyDown[0].n6++;
				m_player->keyDown[1].n2++;
				m_player->keyDown[1].n4++;
				m_player->keyDown[1].n6++;

				//local->SetScore(1, nDownFlag1 + 1000);
				m_player->Draw();

				nDrawTick = newTick;
			}
			
		
	    }
	}
    }
    (*m_ip)[m_socket] = 0;
    nFlag = 0;

	Wait(2000);
    return 0;
}

LRESULT 
Terminal::
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	/*
    switch(message) {
	case WM_TIMER:
	    switch(wParam) {
		case 1:
		{
		    m_player->keyDown[0].n2++;
		    m_player->keyDown[0].n4++;
		    m_player->keyDown[0].n6++;
		    m_player->keyDown[1].n2++;
		    m_player->keyDown[1].n4++;
		    m_player->keyDown[1].n6++;
		    m_player->Draw();
		}
		break;
	    }

	break;
    }*/
	
    return DefWindowProc(hWnd, message, wParam, lParam);
}
