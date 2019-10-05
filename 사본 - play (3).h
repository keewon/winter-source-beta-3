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
	int checked[10][5];
	int partynum;
	int bound[50][50];

	void clear();
	void check(int row, int col);
	int calculate(int part, int ss);
	void checkclear();
	void unify(int num1, int num2);
	int score;
	int ssak;
	int how_many_go;
	int point[50];

public:
	Play(int nID, Game*, Player*, Player*);
	Play(int nID);
	~Play();
	void StartTurn();
	void EndTurn();
	void MoveLeft();
	void MoveRight();
	void MoveDown();
//	void GiveIce(int icepoint);
	void RotateClock();
	void RotateNonClock();
	void RecieveIce(int icepoint);
	int LoseCheck();
	void Redraw();
	void Init();

	void Delay(int);
	int Main();
	Cardpair cardpair;

	//bool delaying;

};

#endif //_PLAY_H_COJETTE
