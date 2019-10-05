#include "ui.h"

using namespace DirectX;

int UI::nFlag = 0;
UI::UI() {

    sprBackground("Resource/background.bmp");

    for (int i=0; i<2; i++) {
	for (int j=0; j<10; j++) {
	    for (int k=0; k<5; k++) {
		// 공백그림으로 채워넣기
		sprCardbox[i][j][k]("Resource/00.bmp");
		// 위치 맞춰주기
		sprCardbox[i][j][k](50*(i*9+k+1), 50*(10-j));
	    }
	}
    }

    sprBackground(0,0);
    bKeyDown5 = bKeyDown2 = bKeyDown4 = bKeyDown6 = false;
}

UI::~UI() {
}

int
UI::
GetInput(void)
{
//	Input::keyboard();
/*
	if (Input::KeyDown[DIK_ESCAPE]) {	return -1;	}
	if (Input::KeyDown[DIK_NUMPAD5]) {	return 5;	}
	if (Input::KeyState[DIK_NUMPAD4]) {	return 4;	}
	if (Input::KeyState[DIK_NUMPAD6]) {	return 6;	}
	if (Input::KeyState[DIK_NUMPAD2]) {	return 2;	}
*/
	if (GetAsyncKeyState(VK_ESCAPE)) {
		return -1;
	}
	if (GetAsyncKeyState(VK_NUMPAD5)) {
		if (bKeyDown5 == false) {
			bKeyDown5 = true;
			return 5;
		}
	} else bKeyDown5 = false;
	if (GetAsyncKeyState(VK_NUMPAD4)) {
		if (bKeyDown4 == false) {
			bKeyDown4 = true;
			return 4;
		}
	} else bKeyDown4 = false;
	if (GetAsyncKeyState(VK_NUMPAD6)) {
		if (bKeyDown6 == false) {
			bKeyDown6 = true;
			return 6;
		}
	} else bKeyDown6 = false;
	if (GetAsyncKeyState(VK_NUMPAD2)) {
		if (bKeyDown2 == false) {
			bKeyDown2 = true;
			return 2;
		}
	} else bKeyDown2 = false;


	return 0;

}

void 
UI::
Redraw(int which, Box &b)
{

    if (which > 1 || which < 0) return;

    //which 가 0이면 자기편이고 왼쪽에 그린다, 
    //         1이면 상대편이면 오른쪽에 그린다.
    sprBackground(DDBLTFAST_NOCOLORKEY, 256, 0);
    char szFilename[80];

    for (int i=0; i<=10; i++) {
	for (int j=0; j<5; j++) {
	    if (b.deck[i][j].num < 0 || b.deck[i][j].num > 51) {
		// 혹시 모를 에러를 막기 위해
		sprintf(szFilename, "Resource/error.bmp");
	    }
	    else {
		sprintf(szFilename, "Resource/%02d.bmp", b.deck[i][j].num);
	    }
	    sprCardbox[which][i][j](szFilename);
	    if (b.deck[i][j].num != 0)
		sprCardbox[which][i][j](DDBLTFAST_NOCOLORKEY,256,0);
	}
    }
    DirectX::DirectDraw::flip();
}


LRESULT 
UI::
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, message, wParam, lParam);
}

UI::
Main(HWND hWnd, MSG message) {
    nFlag = 0;
    //m_hWnd = hWnd;
    if (hWnd) {
	DirectX::Sprite::view(400,300);

	while (!nFlag) {
	    if (PeekMessage(& message, NULL, 0, 0, PM_REMOVE)) {
		if (message.message == WM_QUIT)
			break;
		else {
		    TranslateMessage(& message);
		    DispatchMessage(& message);
		}
	    } 
	}
    }

    return 0;
}
