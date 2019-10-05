#ifndef NETWORK_PLAYER_H_OEDALPHA
#define NETWORK_PLAYER_H_OEDALPHA

#include "Player.h"

// 서버 입장에서의 Opponent
class NetworkPlayer : public Player
{
    public:
	NetworkPlayer(IP*, SOCKET);	// 생성자
	~NetworkPlayer();  		// 소멸자
	int Input();			// 여기서 Input을 받을 때
	void SoundEffect(char);
	void Redraw(int which, Box &b);	
	// 여기에 Display정보 Update 할 때에
	int Main();	//이걸 Game에서 Thread로 돌려줘야 한다
	static bool bClientConnected;
	//static int nFlag;
	void GameOver(int );
	//void Attack(char);
	void SetScore(int which, int n);
	void DrawBound(int, int, int);

	void Next(int, Cardpair);
	void Attack(int, int);
	void GameStatus(int, int);
	
    private:
		void Send(CMD, void*, int);
	IP* m_ip;
	SOCKET m_socket;
	int bound[2][10][5];
	MsgQueue* keyQueue;
};

#endif
