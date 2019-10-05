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

	void clear();
	int score;
	int ssak;
	int nagari;
	int how_many_go;

	//int first_delay;

public:
	Play(int nID, Game*, Player*, Player*);
	Play(int nID);
	~Play();
	void StartTurn();
	void EndTurn();
	bool MoveLeft();
	bool MoveRight();
	void MoveDown();
//	void GiveIce(int icepoint);
	bool Rotate(int direction, int status, int next_row, int next_col);
	bool RotateClock();
	bool RotateCounterClock();
	void ReceiveIce(int icepoint);
	int LoseCheck();
	void Redraw();
	void Init(int nagari_);

	//void Delay(int);
	int Main();

	Cardpair cardpair;
	Cardpair nextpair;

	//bool delaying;

};

#endif //_PLAY_H_COJETTE
