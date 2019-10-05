#ifndef _UI_H_OEDALPHA_
#define _UI_H_OEDALPHA_

// 게임 개발 노트 4월 7일...
// 동아리 경진대회는 일주일 남았다.
// 나는 이 게임의 설계가 이상하다는 것을 파악했다.
// 음냐.. 일단은 어쨌거나 돌아가게 해야겠다.

#include "Unit.h"
#include "sprite.h"
#include "sound.h"
#include "box.h"

using namespace DirectX;

class UI : public Unit {
private:
	Sprite sprBackground;		// 배경그림
	Sprite sprCardbox[2][10][5];		// 상자[너?나][세로][가로]
	bool bKeyDown4;
	bool bKeyDown6;
	bool bKeyDown2;
	bool bKeyDown5;
public:
	UI();
	~UI();
	int GetInput();					// 여기서 Input을 받을 때
	void Redraw(int which, Box &b);	// 여기에 Display정보 Update 할 때에
	//HRESULT Restore(void); 			
	// 각 버퍼들의 잃어버린 이미지들을 다시 연결시켜준다.
	LRESULT WndProc
		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// 실행 단위에서 쓸 WndProc
	int Main(HWND hWnd, MSG message);
	static int nFlag;
	char szTemp[80];
};

#endif
