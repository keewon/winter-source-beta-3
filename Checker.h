#ifndef CHECKER_WINTER_OEDALPHA_H
#define CHECKER_WINTER_OEDALPHA_H

#include "card.h"
#include "box.h"
#include "player.h"

class Game;

namespace Checker {
	const int MAX_NUM_CARD=50;
	typedef enum { NONE=0, PI=1, TI=2, YEOL=3, KWANG=4} method;

	class CheckBox {
	public:
		CheckBox();
		bool box[10][5];
	};

	class Party {
	public:
		Party();
		~Party();
		//int Type();
		int Heuristic();
		int Score();
		int Number();
		bool MeongTeong();
		bool Add(int x_,int y_,int n_);
		bool Add(int x_,int y_,int n_, method m);
		void Incard(Game* game, Box& box, Player* p1, Player* p2, int nID);

		//void DrawBound(Player*, Player*, int);
		//int Size();
	//private:
		method type;
		int number;
		int *x;
		int *y;
		int *c;
		bool IsIn(int);

	};

	bool CheckGrid(Box&, CheckBox&, int, int, Party&);
}

#endif