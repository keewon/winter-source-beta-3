#ifndef _Main_h_
#define _Main_h_

#include "DirectX/DirectX.h"

class Main: public DirectX::Program
{
private:
    DirectX::Sprite background;
    DirectX::Sprite cursor;
    DirectX::ostream console;
    int x, y;
    UINT IDevent;
    bool signal;
    int timer;

    virtual void transit(void);
    virtual int main(void);
public:
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    int main(MSG & message);

    Main(HWND hWnd, DirectX::Input & input, UINT elapse = 50, UINT IDevent = 1);
    virtual ~Main(void);
};

#endif//_Main_h_