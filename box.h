/* * Box
         Card 가 쌓여 있는 곳, 현재 몇 점이 났는가 등을 체크한다.
         check(), clear(), 점수난곳없애기() */
#ifndef _BOX_PUREAREA_H_
#define _BOX_PUREAREA_H_

#include "card.h"

class Box
{
private:
public:
	Card deck[10][5]; 

	Box();
	int dedeck(int row, int col);	//없애야 할 카드의 번호 return
	//void fall(int row, int col);
	void gravity();
};

#endif
