#ifndef _DirectDraw_h_
#define _DirectDraw_h_

#include <ddraw.h>
#include "ISO.h"
#include <set>

namespace DirectX
{
    class DirectDraw;
};

class DirectX::DirectDraw
{
private:
    static unsigned int counter;

    virtual HRESULT restore(void);
protected:
    static std::set <DirectDraw *> pointer;
    static LPDIRECTDRAW	draw;
    static LPDIRECTDRAWSURFACE background, foreground;
    static LPDIRECTDRAWPALETTE palette;
    static DWORD RBitMask, GBitMask, BBitMask;
    static int Rshift, Gshift, Bshift;
    static int Rsize, Gsize, Bsize;
    static int width, height;

    static bool recover(void);
public:
    static const int & Width;
    static const int & Height;

    static void flip(void);
    static void flipToGDI(void);

    DirectDraw & operator ()(const char *const filename = "");

    DirectDraw(HWND hWnd, DWORD width = 640, DWORD height = 480, DWORD bits_per_pixel = 16);
    virtual ~DirectDraw(void);
};

#endif//_DirectDraw_h_
