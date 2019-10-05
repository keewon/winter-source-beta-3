#include "DirectX.h"

bool DirectX::Program::paused(true);

LRESULT
DirectX::Program::
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int
DirectX::Program::
main(MSG & message)
{
    result = 0;
    loop = true;

    while (loop)
    {
	if (PeekMessage(& message, NULL, 0, 0, PM_REMOVE))
	{
	    if (message.message == WM_QUIT)
		break;
	    else
	    {
		TranslateMessage(& message);
		DispatchMessage(& message);
	    }
	} 
	else 
	    if (paused)
		WaitMessage();
	    else
	    {
		Keyboard();
		Mouse();
		transit();
		result = main();
	    }
    }

    return result;
}

DirectX::Program::
Program(HWND hWnd): Keyboard(hWnd), Mouse(hWnd)
{
}

DirectX::Program::
~Program(void)
{
}
