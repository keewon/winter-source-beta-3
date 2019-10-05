#include "ISO.h"
#include <windows.h>
#include <fstream>

#include "Sprite.h"
#include "Sound.h"
#include "resource.h"
#include "Menu.h"
#include "Server.h"
#include "Client.h"
#include "game.h"
#include "IP.h"
#include "player.h"
#include "terminal.h"
#include "localplayer.h"
#include "networkPlayer.h"
#include "computerplayer.h"
#include "AsdxPlayer.h"

// global variables definition.

Unit * program = 0;
HINSTANCE instance;
bool exitFlag = false;

// 윈도우로 입력되는 메세지들을 처리하는 부분
LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{

	case WM_ACTIVATE:
		break;

	case WM_SYSKEYDOWN:
		return 0;
		break;
	case WM_SYSKEYUP:
		return 0;
		break;

	case WM_SETCURSOR:
		SetCursor(NULL);
		break;

	case WM_KEYDOWN:
		break;
	case WM_CLOSE:
		Sound::end();

		if (program) delete program;
		program = 0;
		exitFlag = true;

		DestroyWindow(hWnd);


		UnregisterClass("가서", instance);
		break;
	case WM_DESTROY:

		PostQuitMessage(0);
		break;

	}
	if (program)
		return program->WndProc(hWnd, message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// 윈도우의 생성시 필요한 값들을 초기화한다.
HWND
AppInit(HINSTANCE hInstance, int CmdShow)
{
	HWND hWnd;
	WNDCLASS wc;

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.lpszMenuName = 0;
	wc.lpszClassName = "가서";
	wc.hbrBackground = 0;
	wc.hInstance = hInstance;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClass(& wc))
		return NULL;

	// 창을 만든다
	hWnd = CreateWindow("가서", "GoStop+Puyo", 
			WS_POPUP|WS_BORDER|WS_SYSMENU|WS_MINIMIZEBOX,
			0,0,737,480,
			NULL, NULL, hInstance, NULL);

	if (!hWnd) return NULL;

	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

	if (!Sound::init(hWnd))
	{
		//Sound::end();
		//DestroyWindow(hWnd);
		return NULL;
	}

	return hWnd;
}

// 윈도우즈의 메인함수.
int PASCAL
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int CmdShow)
{

    instance = hInstance;
    HWND hWnd = AppInit(instance, CmdShow);
    MSG message;

    message.wParam = 0;
	if (hWnd == NULL) {
		return message.wParam;
	}


/*
    {
	std::ifstream InputStream("Opening.script");
	program = new Class(InputStream);
    }
*/
    // 메뉴화면
    program = new Menu();
    int	nFlag = program->Main(hWnd, message);

    // 여기서 리턴값을 바로 4랑비교하는 건 좀 거시기하지만.. 그냥 그렇게 했는데
    // 별로 보기는 안좋다..
    while(4 !=nFlag && !exitFlag) 
    {
	switch(nFlag) {
	case 1:	// Menu에서 나온 결과 -> 서버로 실행하라
	{
	    IP* ip = new IP(hWnd);
	    SOCKET socketID = ip->TCP(0, 3598);

	    if ((int)socketID < 0) {
		    MessageBox(hWnd, "Network Server Initialization Failed", "Warning", MB_OK);
		    Server::bClientConnected = false;
	    }
	    //else {// for debugging
		    //char szTemp[100];
		    //sprintf(szTemp, "socket Number = %d", socketID);
		    //MessageBox(hWnd, szTemp, "Server", MB_OK);
	    //}
	    std::string strMyName = ((Menu*)program)->strName;

	    // 프로그램의 흐름이 서버쪽으로 간다
	    delete program; program = new Server(strMyName, ip, socketID);

	    int nServerFlag = program->Main(hWnd, message);
	    switch(nServerFlag) 
	    {
	    case 1:	// Server로 게임을 시작한다
		{
		    // 다이렉트엑스 모드 시작
		    DirectX::DirectDraw* draw = new DirectX::DirectDraw(hWnd, 800, 600);

		    LocalPlayer* p1 = new LocalPlayer();
		    Player* p2 = 0;
		    int nChoice = ((Server*)program)->nOpponentType;
		    switch (nChoice) {
			case 0: // NONE
			    p2 = 0;
			    break;
			case 1: // Another Keyboard
			    p2 = new AsdxPlayer(p1);
			    break;
			case 2: // Computer
			    p2 = new ComputerPlayer(0);
			    break;
			case 3: // Network
			    {
				SOCKET clientSocketID = ((Server*)program)->m_nClientSocketID;
				p2 = new NetworkPlayer(ip, clientSocketID);
			    }
			    break;
			default:
				p2=0;
		    }
		    delete program; program=0;
		    //if (nChoice == 1) {
			// 좌우를 바꿔준다.
			program = new Game(p2, p1, p1);
		    //}
		    //else {
			//program = new Game(p1, p2, p1);
		    //}
		    program->Main(hWnd, message);
		    delete p1; p1=0; if (p2) {delete p2; p2=0;}
		    // 다이렉트 모드는 모든 스프라이트가 없어져야 제대로 끝난다
		    delete draw;    draw=0;
		    delete program; program = new Menu();


		    MoveWindow(hWnd, 0, 0, 737,480, true);
		} break;
	    default: // Server를 종료하고 메뉴를 부른다
		{
			delete program; program = new Menu();
		} break;

	    }
	    delete ip; ip=0;
	}
	break;
	case 2:	// 메뉴에서 나온 결과 -> 클라이언트로 접속하라
	{
	    IP* ip = new IP(hWnd);

	    SOCKET socketID = ip->TCP(0, 3598, ((Menu*)program)->strIP.c_str());
	    if ((int)socketID < 0) {
		MessageBox(hWnd, "Can't connect to Server", "Warning", MB_OK);
		Client::bConnected = false;
	    }
	    else 
	    {
		std::string strMyName = ((Menu*)program)->strName;
		delete program; program = new Client(strMyName, ip, socketID);

		int nClientFlag = program->Main(hWnd, message);
		switch(nClientFlag) 
		{
		case 1:
		    {
			DirectX::DirectDraw* draw = new DirectX::DirectDraw(hWnd, 800, 600);
			LocalPlayer* localPlayer = new LocalPlayer();
			delete program; program = new Terminal(localPlayer, ip, socketID);
			program->Main(hWnd, message);
			delete localPlayer; localPlayer=0;
			delete draw; draw=0;
			delete program; program = new Menu();


			MoveWindow(hWnd, 0, 0, 737,480, true);
		    } break;
		default:
		    {
			delete program; program = new Menu();
		    } break;
		}
	    }
	    delete ip; ip=0;
	}
	break;
	default:
	    { }
	}

	nFlag = program->Main(hWnd, message);
    }

    delete program;
    return message.wParam;
}
