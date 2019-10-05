/////////////////////////////////////////////////////////////////////////////
//  AsdxPlayer
//                               하제 겨울프로젝트 가/서 
//                               서기원
/////////////////////////////////////////////////////////////////////////////
#include "AsdxPlayer.h"
#include "stdio.h"

//using namespace DirectX;
AsdxPlayer::
AsdxPlayer(LocalPlayer* l)
{
    local = l;
    // Key 초기화 : Key가 계속 눌러져 있을 때 한번만 처리하게 하는
    // Flag이다. 이건 새로 고칠 때에는 사용하지 않겠죠?
    // 누르고 있으면 계속 왼쪽이나 오른쪽으로 가게 해야할테니..
    //bKeyDown5 = bKeyDown2 = bKeyDown4 = bKeyDown6 = false;

    // deck 초기화 : deck은 플레이어 각각의 상자정보를 담고 있는 Array이다.

}

AsdxPlayer::
~AsdxPlayer() {
}

int
AsdxPlayer::
Input(void)
{
	
	int& keyDown2 = local->keyDown[1].n2;
	int& keyDown4 = local->keyDown[1].n4;
	int& keyDown5 = local->keyDown[1].n5;
	int& keyDown6 = local->keyDown[1].n6;
	if (GetAsyncKeyState(VK_ESCAPE)) {
		return -1;
	}
	if (GetAsyncKeyState('X') 
		|| GetAsyncKeyState('G') ) {
	    // 바꾸는 키는 계속 누르고 있어도 한번만 됨.
	    if (keyDown5 >= 0) {
		keyDown5 = -1;
		return 5;
	    }
	}
	else if (GetAsyncKeyState('Z')) {
	    // 바꾸는 키는 계속 누르고 있어도 한번만 됨.
	    if (keyDown5 >= 0) {
		keyDown5 = -1;
		return 8;	// 반시계
	    }
	}
	else keyDown5 = 0;

	if (GetAsyncKeyState('F') ) {
	    // 약간 천천히 그러나 빠르게 움직임
	    // 여러번 누르면 더 빠르게 움직임
	    if (keyDown4 >= 0) {
		keyDown4 = -2;
		return 4;
	    }
	}
	else keyDown4 = 0;

	if (GetAsyncKeyState('H') ) {
	    // 약간 천천히 그러나 빠르게 움직임
	    // 여러번 누르면 더 빠르게 움직임
	    if (keyDown6 >= 0) {
		keyDown6 = -2;
		return 6;
	    }
	}
	else keyDown6 = 0;

	if (GetAsyncKeyState(' ') || GetAsyncKeyState('V') ||
		GetAsyncKeyState('B') ) {
	    // 빠르게 내려감
	    if (keyDown2 >= 0) {
		keyDown2 = -1;
	 	return 2;
	    }
	}
	else keyDown2 = 0;
	return 0;
}

void 
AsdxPlayer::SoundEffect(char data) {
    local->SoundEffect(data);
}



void AsdxPlayer::Attack(char c) {	
    // Attack했다 또는 당했다는 표시를 모니터와 음향으로 해주어야 함
}
