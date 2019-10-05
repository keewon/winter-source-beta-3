//game.cpp
//

// Local 변수는 꼭 초기화 해주자!
// -_- 저것 땜에 게임이 한번만 되고 그담부터 안되는 거였다

#include "box.h"
#include "game.h"
#include "util.h"

//int Game::nFlag = 0;
int Game::nDownFlag0 = 0;
int Game::nDownFlag1 = 0;

namespace gg {
    const int PLAYER1 = 0;
    const int PLAYER2 = 1;
    Play* play[2];
    bool bGameOver = false;
	bool bP1Over = true;
	bool bP2Over = true;
}

DWORD WINAPI PlayThread(LPVOID v) {

    int nPlayer = (int)v;

    if (nPlayer == 0 || nPlayer == 1) 
	{
		if (gg::play[nPlayer]) 
		{
			gg::play[nPlayer]->Init(1);
			gg::play[nPlayer]->StartTurn();
			while(gg::bGameOver == false) 
			{
					if (gg::bGameOver == false) 
					{
						gg::play[nPlayer]->Main();
					}
					if (nPlayer == 0 && Game::nDownFlag0 >= 20) 
					{
						gg::play[nPlayer]->MoveDown();
						//Game::nDownFlag0 = 0;
					}
					else if (nPlayer == 1 && Game::nDownFlag1 >= 20) 
					{
						gg::play[nPlayer]->MoveDown();
						//Game::nDownFlag1 = 0;
					}
					Sleep(0);
			}
		}
    }

	Wait(2000);

	if (nPlayer == 0) gg::bP1Over = true;
	else if (nPlayer == 1) gg::bP2Over = true;
    return 0;
}

Game::Game(Player* p1, Player* p2, LocalPlayer* lp) 
: nNagari(1), nAttackStatus(0)
{
    InitializeCriticalSection(&lockOfCardSet);
    InitializeCriticalSection(&lockOfAttackStatus);

	//nAttackStatus=0;
    player[0] = p1;
    player[1] = p2;
	if (player[0])
	{
		play[0] = new Play(0, this, player[0], player[1]);
	}
	else
	{
		play[0] = 0;
	}
    if (player[1]) 
    {
		play[1] = new Play(1, this, player[1], player[0]);
    }
    else 
    {
		play[1] = 0;
    }

    local = lp;
}

Game::~Game() {
    DeleteCriticalSection(&lockOfCardSet);
    DeleteCriticalSection(&lockOfAttackStatus);

    if (play[0]) { delete play[0]; play[0] = 0; }
    if (play[1]) { delete play[1]; play[1] = 0; }
}

// 카드 빼내기
Cardpair Game::Decard() {
    Cardpair c;
    __try {
	    EnterCriticalSection(&lockOfCardSet);
	    c = cardSet.decard();
    }
    __finally {
	    LeaveCriticalSection(&lockOfCardSet);
    }
    return c;
}

// 카드 집어넣기
void Game::Incard(int nCard) {
    __try {
	    EnterCriticalSection(&lockOfCardSet);
	    cardSet.incard(nCard);
    }
    __finally {
	    LeaveCriticalSection(&lockOfCardSet);
    }
}

int Game::GetAttackStatus(/*int id*/) {
    int n;
    __try {
	    EnterCriticalSection(&lockOfAttackStatus);
	    n = nAttackStatus;
		//nAttackStatus=0;
    }
    __finally {
	    LeaveCriticalSection(&lockOfAttackStatus);
    }

    //if (n > 0) {		return (id==gg::PLAYER1)? n:0;	}
    //else if (n < 0) {	return (id==gg::PLAYER1)? 0:n;	}
    //	else return 0;
	return n;
}

void Game::AttackClear() {
	__try {
		EnterCriticalSection(&lockOfAttackStatus);
		nAttackStatus=0;
	} __finally {
		LeaveCriticalSection(&lockOfAttackStatus);
	}
	
}
void Game::Attack(int id, int score) {

    if (score <= 0 || !(id == 0 || id == 1 ) ) /*return 0*/;
	else{
		//score = score * ((id==gg::PLAYER1)? 1:-1);
		//int n;
		__try {
			EnterCriticalSection(&lockOfAttackStatus);
			if((nAttackStatus<0 && id==0) || (nAttackStatus>0 && id==1))
				nAttackStatus=-nAttackStatus;
			if(nAttackStatus>0) nAttackStatus+=score;
			else if(nAttackStatus<0) nAttackStatus-=score;
			else if(nAttackStatus==0) nAttackStatus=(id==0)?score:-score;
			//nAttackStatus += score;
			//n = nAttackStatus;

		}
		__finally {
			LeaveCriticalSection(&lockOfAttackStatus);
		}
	}

	//if (player[id]) player[id]->Attack(score);
	//if (player[1-id]) player[1-id]->Attack(-score);

    //return n;
}

int Game::GetRemainedCard() {
	int n;
	__try {
		EnterCriticalSection(&lockOfCardSet);
		n = cardSet.number();
	}
	__finally {
		LeaveCriticalSection(&lockOfCardSet);
	}
	return n;
}

void Game::Lose(int id) {
	if (id == -1) {
		// 나가리

		nNagari*=2;

	    __try {
		    EnterCriticalSection(&lockOfCardSet);
			for (int i=1; i<=50; i++) {
				cardSet.incard(i);
			}
		}
		__finally {
		    LeaveCriticalSection(&lockOfCardSet);
	    }

		__try {
			EnterCriticalSection(&lockOfAttackStatus);
			nAttackStatus = 0;
		} 
		__finally {
			LeaveCriticalSection(&lockOfAttackStatus);
		}

		if (play[0]) play[0]->Init(nNagari);
		if (play[1]) play[1]->Init(nNagari);

		if (play[0]) play[0]->StartTurn();
		if (play[1]) play[1]->StartTurn();
	} else {
		if (player[id]) {
			// 상대편이 없으면 승패를 표시하지 않는다
			if (player[1-id]) player[id]->GameOver(0);
			else player[id]->GameOver(-1);
		}
		if (player[1-id]) player[1-id]->GameOver(1);
		gg::bGameOver = true;
	}
}

int Game::
Main(HWND hWnd, MSG message) {
//    nFlag = 0;

    nDownFlag0 = 0;
    nDownFlag1 = 0;
    gg::bGameOver = false;
	gg::bP1Over = true;
	gg::bP2Over = true;
	gg::play[0] = play[0];
	gg::play[1] = play[1];
	//int nDownTick = timeGetTime();
	int nDrawTick = timeGetTime();

    if (hWnd) {
	DirectX::Sprite::view(400,300);
	//SetTimer(hWnd, 1, 50, NULL);

	// 함수포인터를 인자로 넘기는 것에 대해 알고 있나?
	// 재미있고도 신기하고도 복잡한 것들이지..
	if (player[0]) {
		gg::bP1Over = false;
		hPlayer[0] = CreateThread(NULL, 0, PlayThread, (LPVOID)0, 0, &(dwPlayer[0]));
	}
	if (player[1]) {
		gg::bP2Over = false;
	    hPlayer[1] = CreateThread(NULL, 0, PlayThread, (LPVOID)1, 0, &(dwPlayer[1]));
	}
	
	while (!(gg::bP1Over && gg::bP2Over) ) 
	{
	    if (PeekMessage(& message, NULL, 0, 0, PM_REMOVE)) 
	    {
			if (message.message == WM_QUIT)
				break;
			else {
				TranslateMessage(& message);
				DispatchMessage(& message);
			}
	    }
		else {
			int newTick = timeGetTime();

			/*
			if (newTick - nDownTick > 50) {
				nDownFlag0++;
				nDownFlag1++;
				nDownTick = newTick;
			}
			*/
			if (newTick - nDrawTick > 100) {
				local->keyDown[0].n2++;
				local->keyDown[0].n4++;
				local->keyDown[0].n6++;
				local->keyDown[1].n2++;
				local->keyDown[1].n4++;
				local->keyDown[1].n6++;

				nDownFlag0++;
				nDownFlag1++;

				//local->SetScore(1, nDownFlag1 + 1000);
				local->Draw();

				nDrawTick = newTick;
			}
		}
	}
    }
    // Thread 끝내기
    if (player[0]) CloseHandle(hPlayer[0]);
    if (player[1]) CloseHandle(hPlayer[1]);

    if (play[1]) { delete play[1]; play[1] = 0; gg::play[1] = 0;}
    if (play[0]) { delete play[0]; play[0] = 0; gg::play[0] = 0;}

    return 0;
}

LRESULT 
Game::
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	/*
    switch(message) {
	// 일정시간마다 패 내려주는 곳
	// 실제로 내리는 것은 PlayThread에서 한다
	case WM_TIMER:
	    switch(wParam) {
		case 1:
		{

		    local->Draw();
		}
		break;
	    }
	break;
    }*/
    return DefWindowProc(hWnd, message, wParam, lParam);
}
