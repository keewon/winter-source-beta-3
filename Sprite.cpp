#include "Sprite.h"
#include <fstream>
#include <string.h>

RECT DirectX::Sprite::rcView = {0, 0, 0, 0};

HRESULT
DirectX::Sprite::
restore(void)
{
    HRESULT hResult;
    HBITMAP hbm;
    HDC hdcImage;
    HDC hdc;
    BITMAP bm;
    DDSURFACEDESC desc;

    if (surface == 0)
	return E_FAIL;
    if ((hResult = surface->Restore()) != DD_OK)
	return hResult;
    hbm = (HBITMAP) LoadImage(GetModuleHandle(0), BitMaPFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    if (hbm == 0)
	hbm = (HBITMAP) LoadImage(0, BitMaPFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hbm == 0)
	return E_FAIL;

    surface->Restore();
    hdcImage = CreateCompatibleDC(0);
    SelectObject(hdcImage, hbm);
    GetObject(hbm, sizeof(BITMAP), & bm);
    desc.dwSize = sizeof(DDSURFACEDESC);
    desc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    surface->GetSurfaceDesc(& desc);
    if ((hResult = surface->GetDC(& hdc)) == DD_OK)
    {
	StretchBlt(hdc, 0, 0, desc.dwWidth, desc.dwHeight, hdcImage, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	surface->ReleaseDC(hdc);
    }
    DeleteDC(hdcImage);
    DeleteObject(hbm);
    (* this) << colorkey;

    return hResult;
}

// 시점을 바꿈
void
DirectX::Sprite::
view(const int & x, const int & y)
{
    rcView.left = x - (DirectDraw::width / 2);
    rcView.top = y - (DirectDraw::height / 2);
    rcView.right = x + (DirectDraw::width / 2);
    rcView.bottom = y + (DirectDraw::height / 2);
}

DirectX::Sprite &
DirectX::Sprite::
operator ()(const char *const filename)
{
    HBITMAP hbm;
    HDC hdcImage;
    HDC hdc;
    BITMAP bm;
    DDSURFACEDESC desc;

    if (surface != 0)
    {
	surface->Release();
	surface = 0;
	if (BitMaPFileName != 0)
	{
	    delete [] BitMaPFileName;
	    BitMaPFileName = 0;
	}
        clip.left = clip.top = clip.right = clip.bottom = width = height = 0;
        pointer.erase(this);
    }
    if (strcmp(filename, "") == 0)
	return * this;
    else
    {
	std::ifstream temp(filename);

	if (temp.is_open() == 0)
	    return * this;
    }
    hbm = (HBITMAP) LoadImage(GetModuleHandle(0), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    if (hbm == 0)
	hbm = (HBITMAP) LoadImage(0, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hbm == 0)
	return * this;
    GetObject(hbm, sizeof(BITMAP), & bm);
    ZeroMemory(& desc, sizeof(DDSURFACEDESC));
    desc.dwSize = sizeof(DDSURFACEDESC);
    desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    desc.dwWidth = bm.bmWidth;
    desc.dwHeight = bm.bmHeight;
    if (draw->CreateSurface(& desc, & surface, 0) != DD_OK)
    {
	if (surface != 0)
	{
	    surface->Release();
	    surface = 0;
	}
	return * this;
    }

    surface->Restore();
    hdcImage = CreateCompatibleDC(0);
    SelectObject(hdcImage, hbm);
    GetObject(hbm, sizeof(BITMAP), & bm);
    desc.dwSize = sizeof(DDSURFACEDESC);
    desc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    surface->GetSurfaceDesc(& desc);
    if (surface->GetDC(& hdc) == DD_OK)
    {
	StretchBlt(hdc, 0, 0, desc.dwWidth, desc.dwHeight, hdcImage, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	surface->ReleaseDC(hdc);
    }
    DeleteDC(hdcImage);
    DeleteObject(hbm);
    BitMaPFileName = new char[strlen(filename) + 1];
    strcpy(BitMaPFileName, filename);
    clip.left = clip.top = 0;
    clip.right = width = bm.bmWidth;
    clip.bottom = height = bm.bmHeight;
    pointer.insert(this);

    return (* this) << RGB(0, 0, 0);
}

DirectX::Sprite &
DirectX::Sprite::
operator ()(const int & x, const int & y)
{
    Sprite::x = x;
    Sprite::y = y;

    return * this;
}

DirectX::Sprite &
DirectX::Sprite::
operator ()(const RECT & rect)
{
    clip = rect;
    if (clip.right < clip.left)
    {
	int temp(clip.right);

	clip.right = clip.left;
	clip.left = temp;
    }
    if (clip.bottom < clip.top)
    {
	int temp(clip.bottom);

	clip.bottom = clip.top;
	clip.top = temp;
    }
    return * this;
}

DirectX::Sprite &
DirectX::Sprite::
operator ()(const int & left, const int & top, const & right, const & bottom)
{
    RECT rect;

    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;

    return (* this)(rect);
}

DirectX::Sprite &
DirectX::Sprite::
operator ()(const DWORD & Transparent, const int & foreAlphaBlend, const int & backAlphaBlend)
{
    RECT rcRect;
    HRESULT hResult ;
    int x = Sprite::x - rcView.left, y = Sprite::y - rcView.top;

    if (draw == 0 || surface == 0)
	return * this;
    if (x < 0)
	x = 0;
    if (y < 0)
	y = 0;

    rcRect.left = rcView.left - Sprite::x + clip.left;
    rcRect.top = rcView.top - Sprite::y + clip.top;
    rcRect.right = rcView.right - Sprite::x + clip.left;
    rcRect.bottom = rcView.bottom - Sprite::y + clip.top;

    if (rcRect.left < clip.left)
	rcRect.left = clip.left;
    if (rcRect.top < clip.top)
	rcRect.top = clip.top;
    if (clip.right < rcRect.right)
	rcRect.right = clip.right;
    if (clip.bottom < rcRect.bottom)
	rcRect.bottom = clip.bottom;

    if (foreAlphaBlend == 256 && backAlphaBlend == 0)
    {
	while (true)
	{
	    hResult = background->BltFast(x, y, surface, & rcRect, Transparent);

	    if (hResult == DD_OK)
		break;
	    if (hResult == DDERR_SURFACELOST)
		if (!recover())
		    return * this;
	    if (hResult != DDERR_WASSTILLDRAWING)
		return * this;
	}

	if (hResult != DD_OK)
	    return * this;
    }
    else
    {
	DDSURFACEDESC DDSurfaceDesc, DDBackgroundDesc;
	WORD colorkey((GetRValue(Sprite::colorkey) >> Rsize << Rshift) | (GetGValue(Sprite::colorkey) >> Gsize << Gshift) | (GetBValue(Sprite::colorkey) >> Bsize << Bshift));
	int rowMax(DirectDraw::height < (rcRect.bottom - rcRect.top + y) ? DirectDraw::Height : rcRect.bottom - rcRect.top);
	int colMax(DirectDraw::width < (rcRect.right - rcRect.left + x) ? DirectDraw::width : rcRect.right - rcRect.left);

	DDSurfaceDesc.dwSize = DDBackgroundDesc.dwSize = sizeof(DDSURFACEDESC);
	surface->Lock(0, & DDSurfaceDesc, DDLOCK_WAIT, 0);
	background->Lock(0, & DDBackgroundDesc, DDLOCK_WAIT, 0);

	for (int Row((y < 0) ? -y : 0); Row < rowMax; ++Row)
	    for (int Column((x < 0) ? -x : 0); Column < colMax; ++Column)
	    {
		WORD sTemp = ((WORD *) DDSurfaceDesc.lpSurface)[DDSurfaceDesc.lPitch * (rcRect.top + Row) / 2 + rcRect.left + Column];

		if (sTemp == colorkey && Transparent == DDBLTFAST_SRCCOLORKEY)
		    continue;

		WORD dTemp = ((WORD *) DDBackgroundDesc.lpSurface)[DDBackgroundDesc.lPitch * (y + Row) / 2 + x + Column];
		int R((foreAlphaBlend * ((sTemp & RBitMask) >> Rshift << Rsize) + backAlphaBlend * ((dTemp & RBitMask) >> Rshift << Rsize)) / 256);
		int G((foreAlphaBlend * ((sTemp & GBitMask) >> Gshift << Gsize) + backAlphaBlend * ((dTemp & GBitMask) >> Gshift << Gsize)) / 256);
		int B((foreAlphaBlend * ((sTemp & BBitMask) >> Bshift << Bsize) + backAlphaBlend * ((dTemp & BBitMask) >> Bshift << Bsize)) / 256);

		if (R < 0)
		    R = 0;
		if (255 < R)
		    R = 255;
		if (G < 0)
		    G = 0;
		if (255 < G)
		    G = 255;
		if (B < 0)
		    B = 0;
		if (255 < B)
		    B = 255;

		((WORD *) DDBackgroundDesc.lpSurface)[DDBackgroundDesc.lPitch * (y + Row) / 2 + x + Column] = (R >> Rsize << Rshift) | (G >> Gsize << Gshift) | (B >> Bsize << Bshift);
	    }
	background->Unlock(0);
	surface->Unlock(0);
    }
    return * this;
}

DirectX::Sprite &
DirectX::Sprite::
operator <<(const COLORREF & rgb)
{
    HRESULT hResult;
    HDC hdc;
    DDCOLORKEY colorkey;
    DDSURFACEDESC desc;
    COLORREF rgbT;

    if (surface == 0)
	return *this;
    if (rgb != CLR_INVALID && surface->GetDC(&hdc) == DD_OK)
    {
	rgbT = GetPixel(hdc, 0, 0);
	SetPixel(hdc, 0, 0, rgb);
	surface->ReleaseDC(hdc);
    }
    desc.dwSize = sizeof(DDSURFACEDESC);
    colorkey.dwColorSpaceLowValue = CLR_INVALID;
    while ((hResult = surface->Lock(0, & desc, 0, 0)) == DDERR_WASSTILLDRAWING);
    if (hResult == DD_OK)
    {
	colorkey.dwColorSpaceLowValue = * (DWORD *)desc.lpSurface;
        if (desc.ddpfPixelFormat.dwRGBBitCount < 32)
            colorkey.dwColorSpaceLowValue &= (1 << desc.ddpfPixelFormat.dwRGBBitCount) - 1;
	surface->Unlock(0);
    }
    if (rgb != CLR_INVALID && surface->GetDC(&hdc) == DD_OK)
    {
	SetPixel(hdc, 0, 0, rgbT);
	surface->ReleaseDC(hdc);
    }
    colorkey.dwColorSpaceHighValue = colorkey.dwColorSpaceLowValue;
    surface->SetColorKey(DDCKEY_SRCBLT, & colorkey);
    Sprite::colorkey = rgb;

    return * this;
}

DirectX::Sprite::
Sprite(const char * const filename)
    : DirectDraw(0), BitMaPFileName(0), width(0), height(0), x(0), y(0)
    , surface(0), Width(width), Height(height)
{
    clip.left = clip.top = clip.right = clip.bottom = 0;

    (* this)(filename);
}

DirectX::Sprite::
~Sprite(void)
{
    if (BitMaPFileName != 0)
	delete [] BitMaPFileName;
    if (surface != 0)
	surface->Release();
}
