#ifndef _GAME_OEDALPHA_H_
#define _GAME_OEDALPHA_H

#include "unit.h"
#include "play.h"
#include "player.h"
#include "card.h"
#include "LocalPlayer.h"

using namespace DirectX;
class Game : public Unit{
private:
	Cardset cardSet;
	int nAttackStatus;
	int nNagari;
	CRITICAL_SECTION lockOfCardSet;
	CRITICAL_SECTION lockOfAttackStatus;
	HANDLE hPlayer[2];
	DWORD dwPlayer[2];

	Play* play[2];			// Play of each Player
	Player* player[2];
	LocalPlayer* local;

public:
	Game(Player*, Player*, LocalPlayer*);
	~Game();
	Cardpair Decard();				// Play에서 Cardpair를 빼낼 때
	void Incard(int );				// Play에서 Card를 다시 집어넣을 때
	int GetAttackStatus(/*int id*/);	// 현재 자기가 얼마나 공격당했는지 알아본다
	void AttackClear();				//nAttackStatus를 초기화.
	void Attack(int id, int score);	// 상대를 공격한다.
	void Lose(int id);				// 자신이 졌음을 알린다.
	int GetRemainedCard();
	LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
	int Main(HWND, MSG);
	//static int nFlag;
	static int nDownFlag0;
	static int nDownFlag1;
};

#endif
