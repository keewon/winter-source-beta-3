#include "Game.h"

DirectX::Program * program = 0;

LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ACTIVATE:
	DirectX::DirectInput::synchronize(DirectX::Program::paused = wParam == WA_INACTIVE);
	break;
    case WM_SETCURSOR:
	SetCursor(0);
	break;
    case WM_DESTROY:
	PostQuitMessage(0);
	break;
    }
    if (program != 0)
	return program->WndProc(hWnd, message, wParam, lParam);
    return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND
AppInit(HINSTANCE hInstance, int CmdShow)
{
    HWND hWnd;
    WNDCLASS wc;

    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = LoadIcon(0, MAKEINTRESOURCE(IDI_APPLICATION));
    wc.lpszMenuName = 0;
    wc.lpszClassName = "DBEntertainment";
    wc.hbrBackground = 0;
    wc.hInstance = hInstance;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    if (!RegisterClass(& wc))
	return 0;
    hWnd = CreateWindow("DBentertainment", "짱똘", WS_POPUP, CW_USEDEFAULT
	, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
    if (hWnd == 0)
	return 0;

    ShowWindow(hWnd, CmdShow);
    UpdateWindow(hWnd);
    SetFocus(hWnd);

    return hWnd;
}

int PASCAL
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int CmdShow)
{
    HWND hWnd(AppInit(hInstance, CmdShow));
    MSG message;
    DirectX::DirectDraw draw(hWnd);
    DirectX::DirectInput input(hWnd, hInstance);

    program = new Game(hWnd);
    program->main(message);
    delete program;
    program = 0;

    return message.wParam;
}
