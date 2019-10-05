//game.cpp
//

// Local 변수는 꼭 초기화 해주자!
// -_- 저것 땜에 게임이 한번만 되고 그담부터 안되는 거였다

#include "box.h"
#include "game.h"

int Game::nFlag = 0;
int Game::nDownFlag0 = 0;
int Game::nDownFlag1 = 0;

namespace gg {
    const int PLAYER1 = 0;
    const int PLAYER2 = 1;
    Play* play[2];
    bool bGameOver = false;
}

DWORD WINAPI PlayThread(LPVOID v) {

    int nFlag1 = 0;
    int nPlayer = (int)v;

    if (nPlayer == 0 || nPlayer == 1) {
	if (gg::play[nPlayer]) {
	    gg::play[nPlayer]->Init();
	    gg::play[nPlayer]->StartTurn();

	    while(nFlag1 != -1 ) 
	    {

		if (gg::play[nPlayer] == 0) {
		    nFlag1 = -1;
		}
		else 
	    	{
		    nFlag1 = gg::play[nPlayer]->Main();
		    Sleep(0);
		}

		if (gg::bGameOver == true) {
			nFlag1 = -1;
			gg::play[nPlayer]->Delay(2000);
		}

	    }
	    Game::nFlag = 1;
	}
    }
    return 0;
}

Game::Game(Player* p1, Player* p2) {
    InitializeCriticalSection(&lockOfCardSet);
    InitializeCriticalSection(&lockOfAttackStatus);

    player[0] = p1;
    player[1] = p2;
    play[0] = new Play(0, this, player[0], player[1]);
    if (player[1]) 
    {
	play[1] = new Play(1, this, player[1], player[0]);
    }
    else 
    {
	play[1] = 0;
    }

    gg::play[0] = play[0];
    gg::play[1] = play[1];
}

Game::~Game() {
    DeleteCriticalSection(&lockOfCardSet);
    DeleteCriticalSection(&lockOfAttackStatus);

    delete play[0];
    if (play[1]) delete play[1];
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

int Game::GetAttackStatus(int id) {
    int n;
    __try {
	    EnterCriticalSection(&lockOfAttackStatus);
	    n = nAttackStatus;
    }
    __finally {
	    LeaveCriticalSection(&lockOfAttackStatus);
    }

    if (n > 0) {		return (id==gg::PLAYER1)? n:0;	}
    else if (n < 0) {	return (id==gg::PLAYER1)? 0:n;	}
    else return 0;
}

int Game::Attack(int id, int score) {
    if (score <= 0 || !(id == gg::PLAYER1 || id == gg::PLAYER2 ) ) return 0;
    score = score * (id==gg::PLAYER1)? 1:-1;
    int n;
    __try {
	    EnterCriticalSection(&lockOfAttackStatus);
	    nAttackStatus += score;
	    n = nAttackStatus;
    }
    __finally {
	    LeaveCriticalSection(&lockOfAttackStatus);
    }
	if (player[id]) player[id]->Attack(score);
	if (player[1-id]) player[1-id]->Attack(-score);

    return n;
}

void Game::Lose(int id) {
	if (player[id]) player[id]->GameOver(false);
	if (player[1-id]) player[1-id]->GameOver(true);
	gg::bGameOver = true;
}

int Game::
Main(HWND hWnd, MSG message) {
    nFlag = 0;
    nDownFlag0 = 0;
    nDownFlag1 = 0;
    gg::bGameOver = false;

    if (hWnd) {
	DirectX::Sprite::view(400,300);
	SetTimer(hWnd, 1, 200, NULL);

	// 함수포인터를 인자로 넘기는 것에 대해 알고 있나?
	// 재미있고도 신기하고도 복잡한 것들이지..
	hPlayer[0] = CreateThread(NULL, 0, PlayThread, (LPVOID)0, 0, &(dwPlayer[0]));
	if (player[1])
	    hPlayer[1] = CreateThread(NULL, 0, PlayThread, (LPVOID)1, 0, &(dwPlayer[1]));
	
	while (!nFlag) 
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
	}
    }
    // Thread 끝내기
    CloseHandle(hPlayer[0]);
    if (player[1]) CloseHandle(hPlayer[1]);

    return 0;
}

LRESULT 
Game::
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
	// 일정시간마다 패 내려주는 곳
	case WM_TIMER:
	    switch(wParam) {
		case 1:
		{
		    nDownFlag0++;
		    nDownFlag1++;
		    if (play[0]) {
			if (nDownFlag0 >= 10) {
			    play[0]->MoveDown();
			    play[0]->Redraw();
			    nDownFlag0 = 0;
			}
		    }
		    if (play[1]) {
			if (nDownFlag1 >= 10) {			
			    play[1]->MoveDown();
			    play[1]->Redraw();
			    nDownFlag1 = 0;
			}
		    }

		}
		break;
	    }
	break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
