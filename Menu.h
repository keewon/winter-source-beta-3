#ifndef _MENU_H_OEDALPHA_
#define _MENU_H_OEDALPHA_

#include "unit.h"
#include "resource.h"
//#include "sprite.h"
#include <string>

class Menu : public Unit {
private:
	HWND m_hWnd;
	HBITMAP m_hbitmapBack;
public:
	Menu();
	~Menu();
	int Main(HWND , MSG);
	//HRESULT Restore(void);
	LRESULT WndProc(HWND  hWnd, UINT message, WPARAM wParam, LPARAM lParam);	
	static int nFlag;	// 0:Server, 1:Client, 2:Option, 3:Exit, -1:loop
	static std::string strIP;
	static std::string strName;
};

#endif