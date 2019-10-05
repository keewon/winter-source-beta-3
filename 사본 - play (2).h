#ifndef _PLAY_H_COJETTE_
#define _PLAY_H_COJETTE_

#include "player.h"
#include "box.h"

class Game;

class Play {
private:
	Game* game;
	Player* player;
	Player* opponent;
	Box box;
	int nID;

	// for check
	int checked[10][5][2];
	int partynum;
	int bound[50][50][2];

	void clear();
	int check(int row, int col);
	int calculate(int part, int sec);
	void checkclear();
	void unify(int num1, int num2);

public:
	Play(int nID, Game*, Player*, Player*);
	Play(int nID);
	~Play();
	void StartTurn();
	void EndTurn();
	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void GiveIce(int icepoint);
	void RotateClock();
	void RotateNonClock();
	void RecieveIce(int icepoint);
	int LoseCheck();
	void Redraw();
	void Init();

	void Delay(int);
	int Main();
	Cardpair cardpair;

};

#endif //_PLAY_H_COJETTE
