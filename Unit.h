#ifndef _Unit_
#define _Unit_

#include <windows.h>

class Unit
{
public:
	//virtual HRESULT Restore(void) = 0; // 각 버퍼들의 잃어버린 이미지들을 다시 연결시켜준다.
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0; // 실행 단위에서 쓸 WndProc
	virtual int Main(HWND hWnd, MSG message) = 0; // 실행 단위의 main function
	virtual ~Unit();
};

#endif