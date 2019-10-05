#ifndef _Sprite_h_
#define _Sprite_h_

#include "DirectDraw.h"

namespace DirectX
{
    class Sprite;
};

class DirectX::Sprite: public DirectX::DirectDraw
{
private:
    static RECT rcView;

    LPDIRECTDRAWSURFACE surface;
    COLORREF colorkey;
    RECT clip;
    char * BitMaPFileName;
    int width, height;
    int x, y;

    HRESULT restore(void);
public:
    const int & Width, & Height;

    static void view(const int & x, const int & y);

    Sprite & operator ()(const char *const filename);
    Sprite & operator ()(const int & x, const int & y);
    Sprite & operator ()(const RECT & rect);
    Sprite & operator ()(const int & left, const int & top, const & right, const & bottom);
    Sprite & operator ()(const DWORD & Transparent = DDBLTFAST_SRCCOLORKEY, const int & foreAlphaBlend = 256, const int & backAlphaBlend = 0); // DDBLTFAST_DESTCOLORKEY DDBLTFAST_NOCOLORKEY DDBLTFAST_SRCCOLORKEY
    Sprite & operator <<(const COLORREF & rgb);

    Sprite(const char * const filename = "");
    virtual ~Sprite(void);
};

#endif//_Sprite_h_
