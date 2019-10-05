#include "DirectDraw.h"

unsigned int DirectX::DirectDraw::counter(0);
std::set <DirectX::DirectDraw *> DirectX::DirectDraw::pointer;
LPDIRECTDRAW DirectX::DirectDraw::draw(0);
LPDIRECTDRAWSURFACE DirectX::DirectDraw::foreground(0), DirectX::DirectDraw::background(0);
LPDIRECTDRAWPALETTE DirectX::DirectDraw::palette(0);
int DirectX::DirectDraw::width(0), DirectX::DirectDraw::height(0);
DWORD DirectX::DirectDraw::RBitMask, DirectX::DirectDraw::GBitMask, DirectX::DirectDraw::BBitMask;
int DirectX::DirectDraw::Rshift(0), DirectX::DirectDraw::Gshift(0), DirectX::DirectDraw::Bshift(0);
int DirectX::DirectDraw::Rsize(0), DirectX::DirectDraw::Gsize(0), DirectX::DirectDraw::Bsize(0);
const int & DirectX::DirectDraw::Width = DirectX::DirectDraw::width;
const int & DirectX::DirectDraw::Height = DirectX::DirectDraw::height;

HRESULT
DirectX::DirectDraw::
restore(void)
{
    return DD_OK;
}

bool
DirectX::DirectDraw::
recover(void)
{
    bool result = true;

    for (std::set <DirectDraw *>::iterator i(pointer.begin()); i != pointer.end(); ++i)
	result &= ((* i)->restore() == DD_OK);
    if (background != 0)
	result &= (background->Restore() == DD_OK);
    if (foreground != 0)
	result &= (foreground->Restore() == DD_OK);

    return result;
}

void
DirectX::DirectDraw::
flip(void)
{
    HRESULT hResult;
    bool flag(draw != 0);

    while (flag)
    {
	if ((hResult = foreground->Flip(0, 0)) == DD_OK)
	    break;
	if (hResult == DDERR_SURFACELOST)
	    if (!recover())
		break;
	if (hResult != DDERR_WASSTILLDRAWING)
	    break;
    }
}

void
DirectX::DirectDraw::
flipToGDI(void)
{
    LPDIRECTDRAW2 draw2;

    if (draw == 0)
	return;
    draw->QueryInterface(IID_IDirectDraw2, (void * *)& draw2);
    draw->FlipToGDISurface();
    draw->Release();
}

DirectX::DirectDraw &
DirectX::DirectDraw::
operator ()(const char *const filename)
{
    HRSRC handle;
    PALETTEENTRY entry[256];
    RGBQUAD * rgb;
    HFILE hFile;
    int n;

    if (palette != 0)
    {
        palette->Release();
	palette = 0;
    }
    for (unsigned int i(0); i < 256; ++i)
    {
	entry[i].peRed   = (BYTE) (((i >> 5) & 0x07) * 255 / 7);
	entry[i].peGreen = (BYTE) (((i >> 2) & 0x07) * 255 / 7);
	entry[i].peBlue  = (BYTE) (((i >> 0) & 0x03) * 255 / 3);
	entry[i].peFlags = (BYTE) 0;
    }
    if (filename && ((handle = FindResource(0, filename, RT_BITMAP)) != 0))
    {
	LPBITMAPINFOHEADER info((LPBITMAPINFOHEADER) LockResource(LoadResource(0, handle)));

	if (info == 0)
	    return * this;
	rgb = (RGBQUAD *)((DWORD) info + info->biSize);

	if (info == 0 || info->biSize < sizeof(BITMAPINFOHEADER))
	    n = 0;
	else
	    if (info->biBitCount > 8)
		n = 0;
	    else
		if (info->biClrUsed == 0)
		    n = 1 << info->biBitCount;
		else
		    n = info->biClrUsed;

	for(unsigned int i(0); i < n; ++i)
	{
	    entry[i].peRed   = rgb[i].rgbRed;
	    entry[i].peGreen = rgb[i].rgbGreen;
	    entry[i].peBlue  = rgb[i].rgbBlue;
	    entry[i].peFlags = 0;
	}
    }
    else if (filename && (hFile = _lopen(filename, OF_READ)) != -1)
    {
	BITMAPFILEHEADER file;
	BITMAPINFOHEADER info;

	_lread(hFile, & file, sizeof(BITMAPFILEHEADER));
	_lread(hFile, & info, sizeof(BITMAPINFOHEADER));
	_lread(hFile, entry, 256 * sizeof(PALETTEENTRY));
	_lclose(hFile);

	if (info.biSize != sizeof(BITMAPINFOHEADER))
	    n = 0;
	else
	    if (info.biBitCount > 8)
		n = 0;
	    else
		if (info.biClrUsed == 0)
		    n = 1 << info.biBitCount;
		else
		    n = info.biClrUsed;

	for(unsigned int i(0); i < n; ++i)
	{
	    BYTE r = entry[i].peRed;
	    entry[i].peRed  = entry[i].peBlue;
	    entry[i].peBlue = r;
	}
    }
    draw->CreatePalette(DDPCAPS_8BIT, entry, & palette, 0);
    if (palette != 0)
        foreground->SetPalette(palette);
 
    return * this;
}

DirectX::DirectDraw::
DirectDraw(HWND hWnd, DWORD width, DWORD height, DWORD bits_per_pixel)
{
    DDSURFACEDESC DDSurfaceDesc;
    DDSCAPS DDScaps;

    ++counter;
    if (draw != 0 || hWnd == 0)
	return;
    if (DirectDrawCreate(0, & draw, 0) != DD_OK)
    {
	MessageBox(hWnd, "DirectDrawCreate", "DirectDraw", MB_OK);
	return;
    }
    if (draw->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) != DD_OK)
    {
	draw->Release();
	draw = 0;
	MessageBox(hWnd, "SetCooperativeLevel", "DirectDraw", MB_OK);
	return;
    }
    if (draw->SetDisplayMode(width, height, bits_per_pixel) != DD_OK)
    {
	draw->Release();
	draw = 0;
	MessageBox(hWnd, "SetDisplayMode", "DirectDraw", MB_OK);
	return;
    }
    DDSurfaceDesc.dwSize = sizeof(DDSurfaceDesc);
    DDSurfaceDesc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    DDSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    DDSurfaceDesc.dwBackBufferCount = 1;
    if (draw->CreateSurface(& DDSurfaceDesc, & foreground, 0) != DD_OK)
    {
	MessageBox(hWnd, "CreateSurface", "DirectDraw", MB_OK);
	return;
    }
    DDScaps.dwCaps = DDSCAPS_BACKBUFFER;
    if (foreground->GetAttachedSurface(& DDScaps, & background) != DD_OK)
    {
	foreground->Release();
	foreground = 0;
	draw->Release();
	draw = 0;
	MessageBox(hWnd, "GetAttachedSurface", "DirectDraw", MB_OK);
	return;
    }
    while (background->Lock(0, & DDSurfaceDesc, 0, 0) != DD_OK);
    RBitMask = DDSurfaceDesc.ddpfPixelFormat.dwRBitMask;
    GBitMask = DDSurfaceDesc.ddpfPixelFormat.dwGBitMask;
    BBitMask = DDSurfaceDesc.ddpfPixelFormat.dwBBitMask;
    Rshift = Gshift = Bshift = 0;
    Rsize = Gsize = Bsize = 8;
    for (DWORD i(RBitMask); i != 0; i >>= 1)
	if ((i & 0x01) == 0x01)
	    --Rsize;
	else
	    ++Rshift;
    for (DWORD i(GBitMask); i != 0; i >>= 1)
	if ((i & 0x01) == 0x01)
	    --Gsize;
	else
	    ++Gshift;
    for (DWORD i(BBitMask); i != 0; i >>= 1)
	if ((i & 0x01) == 0x01)
	    --Bsize;
	else
	    ++Bshift;
    background->Unlock(0);
    DirectDraw::width = width;
    DirectDraw::height = height;
    return;
}

DirectX::DirectDraw::
~DirectDraw(void)
{
    pointer.erase(this);
    if (--counter != 0 || draw == 0)
	return;
    if (foreground != 0)
    {
        foreground->Release();
	foreground = 0;
    }
    if (palette != 0)
    {
        palette->Release();
	palette = 0;
    }
    draw->Release();
    draw = 0;
    width = height = 0;
}
