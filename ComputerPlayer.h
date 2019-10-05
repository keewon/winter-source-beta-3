// 아래 두 줄은 헤더를 만들 때에 중복 include되지 않게 하는 macro입니다.
#ifndef _COMPUTER_PLAYER_HANEULI_H
#define _COMPUTER_PLAYER_HANEULI_H

#include "Player.h"
#include "Box.h"	// Added by ClassView
//#include "fstream.h"

class ComputerPlayer : public Player
{
private:
    // 추가해야할 것들을 추가해 넣으세요
	MsgQueue comQ;
	int nID;
	Box box;

	bool processed;

	int clock;
public:
    ComputerPlayer(int n);
    ~ComputerPlayer();
    int Input();
    void Redraw(int, Box &b);
    //void GameOver(bool winner);
    void Attack(char);

    int Main();

	bool Danger();
	//HWND hhh;
    // public 쪽에는 웬만하면 함수나 변수를 추가하지 마세요
};


// 아래에 있는 #endif 는 제일 윗줄의 #ifndef와 쌍이 되는 애 입니다.
#endif