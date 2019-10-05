// 아래 두 줄은 헤더를 만들 때에 중복 include되지 않게 하는 macro입니다.
#ifndef _COMPUTER_PLAYER_HANEULI_H
#define _COMPUTER_PLAYER_HANEULI_H

#include "Player.h"
#include "Box.h"	// Added by ClassView
#include "fstream.h"

class ComputerPlayer : public Player
{
private:
	ofstream out;
	Box tempBox;
	int Rotated_state;
	virtual int visit(int x, int y);
	virtual int getcurrent(int,int);
	int compos[2][2];
	int humanpos[2][2];
	virtual void Infer_decide();
	virtual void Seperator(int);
	Box comBox, comDrop;
	Box humanBox, humanDrop;
    // 추가해야할 것들을 추가해 넣으세요
	MsgQueue comQ;
	int nID;
	Card comIng[2], humanIng[2]; // 배열

public:
	int Decide();
	
    //enum WaveName {SE_BACK = 0, SE_CLEAR = 1, SE_ATTACK = 2, 
    //SE_ATTACKED = 3, SE_GAMEOVER =4 };
    ComputerPlayer(int n);
    ~ComputerPlayer();
    int Input();
    void Redraw(int, Box &b);
    void SoundEffect(char);
    void GameOver(bool winner);
    void Attack(char);
    void RemainedCard(int);
	void SetScore(int, int);
	void DrawBound(int, int, int);

    int Main();

	HWND hhh;
    // public 쪽에는 웬만하면 함수나 변수를 추가하지 마세요
};


// 아래에 있는 #endif 는 제일 윗줄의 #ifndef와 쌍이 되는 애 입니다.
#endif