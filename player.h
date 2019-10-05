#ifndef _PLAYER_OEDALPHA_H_
#define _PLAYER_OEDALPHA_H_

#include "card.h"
#include "box.h"
#include "sprite.h"
#include "sound.h"
#include "box.h"
#include "msgqueue.h"
#include "protocol.h"
#include "ip.h"

//namespace Checker {
//class Party;
//}

class Player
{
private:

public:
    //enum WaveName {SE_BACK = 0, SE_CLEAR = 1, SE_ATTACK = 2, 
    //SE_ATTACKED = 3, SE_GAMEOVER =4 };
	virtual ~Player();
    virtual int Input() = 0; // 입력을 받는다.
    virtual void Redraw(int, Box &b); // 출력한다.
	virtual void Next(int, Cardpair);
    virtual void SoundEffect(char);
	virtual void GameOver(int winner);
	virtual void Attack(int which, int n);
	virtual void GameStatus(int code, int num);
	virtual void SetScore(int which, int n);
	virtual void DrawBound(int, int, int);
	//virtual void DrawBound(int, Checker::Party*);

	virtual int Main();

};

#endif
