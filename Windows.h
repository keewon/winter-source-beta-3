#ifndef _Windows_h_
#define _Windows_h_

#include <windows.h>
#include "ISO.h"

namespace Windows
{
    class Program;
};

class Windows::Program: public ISO::Program
{
public:
    virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

    virtual ~Program(void) { }
};

#endif//_Windows_h_