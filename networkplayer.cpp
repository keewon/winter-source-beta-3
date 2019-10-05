// Network Player by oedalpha
#include "NetworkPlayer.h"
#include "msgqueue.h"
#include <fstream>

bool NetworkPlayer::bClientConnected = false;
//int NetworkPlayer::nFlag = 0;

namespace np {
    // 얘는 new가 아니므로 delete하지 않는다!
    MsgQueue msgQueue(1024);
	std::ofstream ofs;
    // 네트워크로 메시지가 전달될 때마다 불리게 될 함수
    // 하지만 여기서 하는 일은 그저 Queue에 집어넣는 것 뿐이다
    int 
    func(const SOCKET nSocketID) {

	if ((int)nSocketID < 0) {
	    // nSocketID는 이 함수가 불릴 때에 넘어오는 인자인데
	    // 이게 0보다 작으면 비정상적인 경우이다
	    NetworkPlayer::bClientConnected = false;
	    msgQueue.Empty();
	    return 0;
	}
	
	NetworkPlayer::bClientConnected = true;
	char szTemp[1000]; memset(szTemp, 0, 1000);
	int n = recv(nSocketID, szTemp, 1000, 0);
	if (n>0) msgQueue.Push(szTemp, n);

	return 0;
    }


}
NetworkPlayer::
NetworkPlayer(IP* arg_ip, SOCKET arg_socket)	// 생성자
{
 //   nFlag = 0;
	np::msgQueue.Empty();
    bClientConnected = true;
    m_ip = arg_ip;
    m_socket = arg_socket;
    keyQueue = new MsgQueue(256);   // new 했다. 그러면? delete해야죠.

    // 네트웍으로 메시지가 올 때 여기로 가라고 지정하여 주는 문장
    (*m_ip)[m_socket] = & np::func; 

	for (int which=0; which <=1; which++) {
		for (int i=0; i<10; i++) {
			for (int j=0; j<5; j++) {
				bound[which][i][j] = 0; //reset
			}
		}
	}


}
NetworkPlayer::
~NetworkPlayer()  		// 소멸자
{ 
    delete keyQueue; // new 한 것은 항상 delete!!
		    // c++이 없어질 때까지 기억하자!
}

int 
NetworkPlayer::
Input()
{
    // client packet에는 key정보가 담겨있다
    // packet을 받아서 미리 keyqueue에 넣어 놓는다
    // 이것을 이용해서 Data를 받아간다

    if (keyQueue->Size() > 0) 
    {
	char cTemp;
	keyQueue->Pop(&cTemp, 1);

	switch (cTemp) {
	    case -1:
	    case 5:
	    case 2:
	    case 4:
	    case 6:
		return cTemp;
		break;
	    default:
		// invalid한 key 입력
		return 0;
	}
    }
    return 0;
}

void 
NetworkPlayer::
Redraw(int which, Box &b)	// 여기에 Display정보 Update 할 때에
{
	Protocol::GameServerPacket data;
	data.player = (char)which;
	for (int i=0; i<10; i++) {
		for (int j=0; j<5; j++) {
		    data.deck[i][j] = (char)(b.deck[i][j].num);
		}
	}

	Send(np_CMD::CMD_GAME_SERVER, &data, sizeof(Protocol::GameServerPacket) );
}

void 
NetworkPlayer::
Send(CMD cmd, void* data, int length) {

    if (bClientConnected) 
    {
		if (length < 0) return;
		if (!m_ip->empty() && m_socket > 0) 
		{
			// Packet을 만든다
			char* szPacket = new char[length+1];
			szPacket[0] = cmd;
			if (length > 0) memcpy(szPacket+1, data, length);
			// 보낸다
			if (-1 == send(m_socket, szPacket, length+1, 0)) 
			{
				bClientConnected = false;
				m_ip->erase(m_socket);
				np::msgQueue.Empty();
			}
			delete [] szPacket;

		} else {
			bClientConnected = false;
			m_ip->erase(m_socket);
			np::msgQueue.Empty();
		}
    }
}

void
NetworkPlayer::
SoundEffect(char data) 
{
    // 위에서 효과음 연주하라고 명령하면 
    // 그 명령을 client쪽으로 보내주는 함수이다.
	Send(np_CMD::CMD_SOUND, &data, 1);
}

void NetworkPlayer::Next(int which, Cardpair c) {
	char data[2];
	data[0] = (char)(c.Pair[0].num);
	data[1] = (char)(c.Pair[1].num);
	switch (which) {
	case 0:
		Send(np_CMD::CMD_0_NEXT, data,2);
		break;
	case 1:
		Send(np_CMD::CMD_1_NEXT, data,2);
		break;
	}
}

void NetworkPlayer::Attack(int which, int n) {
	char c = (char)n;
	switch (which) {
	case 0:
		Send(np_CMD::CMD_0_ATTACK, &c,1);
		break;
	case 1:
		Send(np_CMD::CMD_1_ATTACK, &c, 1);
		break;
	}
}

void NetworkPlayer::GameStatus(int c, int n) {
	char data[2]; data[0]=(char)c; data[1] = (char)n;
	Send(np_CMD::CMD_STATUS, data, 2);
}



int
NetworkPlayer::
Main()
{
	if (bClientConnected == false) {
		//nFlag = -1;
		char key = -1;
		keyQueue->Push(&key, 1);
	}

    // 아래 세줄은 네트워크 메시지를 얼마나 자주 처리할 것
    // 인가를 정한다.

	struct timeval tv;
	tv.tv_sec = 0; tv.tv_usec = 100;
	(*m_ip)(&tv);


	// 큐에 메시지가 왔는지 왔으면 어떤 메시지가 왔는지 검사
	// 메시지가 안왔으면 CMD_NIL 이라는 메시지가 return 된다
	CMD cmd = np::msgQueue.ReadCmd();
	char cTemp;

	switch(cmd) 
	{
		case np_CMD::CMD_0_BOUND:
		case np_CMD::CMD_1_BOUND:
	    case np_CMD::CMD_GAME_SERVER:
	    {
		// 아니 서버한테 누가 서버 패킷을 보내지?
		// 이건 무시하자
		struct Protocol::GameServerPacket temp;
		// sizeof() + 1인 것은 CMD까지 계산한 값이다
		if (np::msgQueue.Size() >= 
			sizeof(Protocol::GameServerPacket)+1) 
		{
		    np::msgQueue.Pop(&cTemp,1);
		    np::msgQueue.Pop(&temp, sizeof(Protocol::GameServerPacket));
		}
	    }
	    break;
	    case np_CMD::CMD_GAME_CLIENT:
	    {
		// 상대방의 Key를 받아서 거시기 하자.
		struct Protocol::GameClientPacket temp;
		// sizeof() + 1인 것은 CMD까지 계산한 값이다
		if (np::msgQueue.Size() >= sizeof(Protocol::GameClientPacket)+1) {
		    np::msgQueue.Pop(&cTemp,1);
		    np::msgQueue.Pop(&temp, sizeof(Protocol::GameClientPacket));
		    // 여기서 temp가지고 거시기를 해야한다
		    keyQueue->Push(&(temp.key),1);
		}
	    }
	    break;
	    case np_CMD::CMD_ERROR:
	    {
		//에러나면 그냥 튕겨버려 
		// 가 아니고 gentle하게 튕길 것!
		np::msgQueue.Pop(&cTemp,1);
		bClientConnected = false;
		//MessageBox(m_hWnd, "Connection Lost", "Network Error", MB_OK);
		//NetworkPlayer::nFlag = -1;

	    }
	    break;
	    case np_CMD::CMD_QUIT:
	    {
		//에러나면 그냥 튕겨버려
		// 가 아니고 gentle하게 튕길 것!
		np::msgQueue.Pop(&cTemp,1);
		bClientConnected = false;
		//MessageBox(m_hWnd, "Opponent Quits", "End of Game", MB_OK);
		//NetworkPlayer::nFlag = -1;
	    }
	    break;
	    case np_CMD::CMD_GO:
	    {	
		//게임을 이제 시작하면 된다
		//일단은 이걸 받기 전에 시작하기로 하자.
		//Condition Variable을 써야하는데 원래는 -_-
		// (Condition Variable은 OS때 배우는 개념임..)
		np::msgQueue.Pop(&cTemp,1);
	    }
	    break;
	    case np_CMD::CMD_NAME:
	    {
		//상대방의 이름이 날라온다
		//무시하자
		// 이 메시지는 채팅할 때 유효한 메시지이다.
		char szName[Protocol::MAX_CHAT_LENGTH+1];
		if (np::msgQueue.Size() >= Protocol::MAX_CHAT_LENGTH+1) {
		    np::msgQueue.Pop(&cTemp,1);
		    np::msgQueue.Pop(szName, Protocol::MAX_CHAT_LENGTH);
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
		// 뽑아낸 다음에 무시한다
		char szMessage[Protocol::MAX_CHAT_LENGTH+1];
		// CMD까지 합쳐서 계산하는 거라서 MAX_CHAT_LENGTH+1이다
		if (np::msgQueue.Size() >= Protocol::MAX_CHAT_LENGTH+1) {
		    np::msgQueue.Pop(&cTemp,1);
		    np::msgQueue.Pop(szMessage, Protocol::MAX_CHAT_LENGTH);
		}
	    }
	    break;
		// +4byte 명령어 무시하기
		case np_CMD::CMD_0_SCORE:
		case np_CMD::CMD_1_SCORE:
			{
				char szMessage[5];
				if (np::msgQueue.Size() >= 5) {
					np::msgQueue.Pop(szMessage, 5);
				}
			}
			break;
		// +2byte 명령어 무시하기
		case np_CMD::CMD_0_NEXT:
		case np_CMD::CMD_1_NEXT:
			{
				char szMessage[3];
				if (np::msgQueue.Size() >= 3) {
					np::msgQueue.Pop(szMessage, 3);
				}
			}
			break;
		// +1byte 명령어 무시하기
		case np_CMD::CMD_0_ATTACK:
		case np_CMD::CMD_1_ATTACK:
	    case np_CMD::CMD_SOUND:
	    {
		// Sound Message
		// 뽑아낸 다음에 무시한다.
		// 서버한테 Sound MEssage를 보내는 일은 없겠지.. 설마..
		char szMessage[2];
		if (np::msgQueue.Size() >= 2) {
		    np::msgQueue.Pop(szMessage, 2);
		}
	    }
		break;
		// +0byte 명령어 무시하기
		case np_CMD::CMD_WIN:
		case np_CMD::CMD_LOSE:
			{
				char szMessage[1];
				if (np::msgQueue.Size() >= 1) {
					np::msgQueue.Pop(szMessage, 1);
				}
			}
			break;
		default:
			np::msgQueue.Empty();
	}
	return 0;
}

void NetworkPlayer::GameOver(int winner) {

	if (winner == 1) {
		Send(np_CMD::CMD_WIN, 0, 0);
		//char szPacket = np_CMD::CMD_WIN;
		//send(m_socket, &szPacket, 1, 0);
	}
	else {
		//char szPacket = np_CMD::CMD_LOSE;
		//send(m_socket, &szPacket, 1, 0);
		Send(np_CMD::CMD_LOSE, 0, 0);
	}

}

void NetworkPlayer::SetScore(int which, int n2) {
	int n = n2;
	char score[4];
	for (int i=3; i>=0; i--) {
		score[i] = n % 256;
		n -= (n%256);
		n = n/256;
		//n = n >> 8;
	}
	switch (which) {
	case 0: 
		Send(np_CMD::CMD_0_SCORE, score, 4);
		break;
	case 1:
		Send(np_CMD::CMD_1_SCORE, score, 4);
		break;
	}
}

void NetworkPlayer::DrawBound(int which, int x, int y) {
    if (x < 0) { 
		Protocol::GameServerPacket data;
		data.player = which;
		for (int i=0; i<10; i++) {
			for (int j=0; j<5; j++) {
				data.deck[i][j] = bound[which][i][j];
				bound[which][i][j] = 0; //reset
			}
		}

		switch(which) {
		case 0:
			Send(np_CMD::CMD_0_BOUND, &data, sizeof(Protocol::GameServerPacket) );
			break;
		case 1:
			Send(np_CMD::CMD_1_BOUND, &data, sizeof(Protocol::GameServerPacket) );
			break;
		}
		
    }
    else {
	bound[which][x][y] = -1;
	}
}