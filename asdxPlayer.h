#ifndef ASDX_PLAYER_H_OEDALPHA
#define ASDX_PLAYER_H_OEDALPHA

#include "player.h"
#include "LocalPlayer.h"

class AsdxPlayer : public Player
{
    public:
	AsdxPlayer(LocalPlayer*);	// 생성자
	~AsdxPlayer();  // 소멸자
	int Input();	// 여기서 Input을 받을 때
	void SoundEffect(char);
	// 여기에 Display정보 Update 할 때에
	//void GameOver(int );
	void Attack(char c);
    private:
	/*
	int deck[2][10][5]; 	// 상자[너?나][세로][가로]
	int bound[2][10][5];
	bool bKeyDown4;
	bool bKeyDown6;
	bool bKeyDown2;
	bool bKeyDown5;
	int nScore[2];
	char szTemp[80];
	*/
	LocalPlayer* local;
};

#endif
