#include "Menu.h"
///////////////////////////////////////////
// 게임 처음 시작하면 나오는 메뉴 Menu.cpp
////////////////////////////////////////
int Menu::nFlag = 0;
std::string Menu::strIP = "";
std::string Menu::strName = "Player1";

extern HINSTANCE instance;

namespace mm {
const int MENU_SERVER = 1;
const int MENU_CLIENT = 2;
const int MENU_OPTION = 3;
const int MENU_EXIT = 4;

HWND hMenuDialog;
BOOL CALLBACK MenuDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
switch(iMessage)
	{
	case WM_INITDIALOG:
		switch(Menu::nFlag) {
		case MENU_SERVER:
			SetWindowText(hDlg, "Go/Stop Puyo :: Server");
			SetDlgItemText(hDlg, IDC_EDIT_NAME, "왕건");
			EnableWindow(GetDlgItem(hDlg,IDC_EDIT_IP),false);
			break;
		case MENU_CLIENT:
			SetDlgItemText(hDlg, IDC_EDIT_NAME, "견훤");
			SetWindowText(hDlg, "Go/Stop Puyo :: Client");
			break;
		}
		return true;
	case WM_COMMAND:
		{
			char szTemp[16];
			switch (wParam)
			{
			case IDC_OK:
				GetDlgItemText(hDlg, IDC_EDIT_NAME, szTemp, 16);
				if (strlen(szTemp) <= 0) {
					MessageBox(hDlg, "Input Your Name", "Error", MB_OK);
					return true;
				}else {
					Menu::strName = szTemp;
					GetDlgItemText(hDlg, IDC_EDIT_IP, szTemp, 16);
					if (strlen(szTemp) <= 0 && 
						Menu::nFlag == MENU_CLIENT) {
						MessageBox(hDlg, "Invalid IP Address", 
							"Error", MB_OK);
						return true;
					} else {
						Menu::strIP = szTemp;
					}
					EndDialog(hDlg,0);
					return true;
				}
			case IDC_CANCEL:
				Menu::nFlag = -1;
				EndDialog(hDlg,0);
				return true;
			}
		}
		break;
	}
return false;
}

} // namespace mm;

LRESULT
Menu::
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CLOSE:
		nFlag = mm::MENU_EXIT;
		break;
	case WM_PAINT:
		{
			{
			HDC hdc = GetDC(hWnd);
			HDC memdc = CreateCompatibleDC(hdc);
			SelectObject(memdc, m_hbitmapBack);
			RECT r; GetClientRect(hWnd, &r);
			BitBlt(hdc, 0,0, r.right, r.bottom, memdc, 0,0,SRCCOPY);
			DeleteDC(memdc);
			}
		}
		
		break;
	case WM_KEYUP:
		{
			int nKey = (int)wParam;
			// Function Key 제어는 여기서 한다.
			if (nKey == VK_F1) {
				//MessageBox(hWnd, "F1", "HAJE", MB_OK);
				nFlag= mm::MENU_SERVER;
					DialogBox(instance, MAKEINTRESOURCE(IDD_MENU_DIALOG),
						hWnd, mm::MenuDlgProc);
				return 0;
			}
			else if (nKey == VK_F2) {
				nFlag = mm::MENU_CLIENT;
					DialogBox(instance, MAKEINTRESOURCE(IDD_MENU_DIALOG),
						hWnd, mm::MenuDlgProc);
					return 0;

			}
			else if (nKey == VK_F4) {//VK_F10/* || nKey == VK_ESCAPE*/) {
				nFlag = mm::MENU_EXIT;
				return 0;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT p;
			//RECT r; GetWindowRect(hWnd, &r);
			GetCursorPos(&p);
			ScreenToClient(hWnd, &p);

			if (p.x > 211 && p.x < 522) {
				if (p.y > 160 && p.y < 230) {
					nFlag = mm::MENU_SERVER;
					DialogBox(instance, MAKEINTRESOURCE(IDD_MENU_DIALOG),
						hWnd, mm::MenuDlgProc);
					return 0;
				}
				
				else if (p.y > 253 && p.y < 323) {
					nFlag = mm::MENU_CLIENT;
					//nFlag = mm::MENU_EXIT;
					DialogBox(instance, MAKEINTRESOURCE(IDD_MENU_DIALOG),
						hWnd, mm::MenuDlgProc);
					return 0;
				}
				
				/*
				else if (p.y > 00 && p.y < 230) {
					nFlag = mm::MENU_OPTION;
				}*/
				
				else if (p.y > 346 && p.y < 416) {
					nFlag = mm::MENU_EXIT;
				}
				
			}
		}
		break;

	}
	return DefWindowProc(hWnd, message, wParam, lParam);	
}

Menu::Menu() {
}

Menu::~Menu() {
}

int Menu::Main(HWND hWnd, MSG message) {
	using namespace mm;

	nFlag = 0;
	m_hWnd = hWnd;
	m_hbitmapBack = LoadBitmap(instance, MAKEINTRESOURCE(IDB_MENU_BACK));
	InvalidateRect(hWnd,NULL, false);

	if (hWnd) {
		while (!nFlag) {
			if (PeekMessage(& message, NULL, 0, 0, PM_REMOVE)) {
				if (message.message == WM_QUIT)
					break;
				else {
					TranslateMessage(& message);
					DispatchMessage(& message);
				}
			} 
			else {
			}
		}
	}

	DeleteObject(m_hbitmapBack);
	return nFlag;
}
