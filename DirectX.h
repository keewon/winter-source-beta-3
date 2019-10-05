#ifndef _DirectX_h_
#define _DirectX_h_

#include "../Windows.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Sprite.h"
#include "streambuf.h"

namespace DirectX
{
    class Program;
}

class DirectX::Program: public Windows::Program
{
private:
    int result;
protected:
    DirectX::Keyboard Keyboard;
    DirectX::Mouse Mouse;
    bool loop;

    virtual void transit(void) = 0;
    virtual int main(void) = 0;
public:
    static bool paused;

    virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual int main(MSG & message);

    Program(HWND hWnd);
    virtual ~Program(void);
};

#endif//_DirectX_h_
