#include "Keyboard.h"

DirectX::Keyboard &
DirectX::Keyboard::
operator ()(void)
{
    HRESULT hResult;
    BYTE state[256];

    if (device == 0)
	return * this;
    while (true)
    {
	if ((hResult = device->GetDeviceState(256 * sizeof(BYTE), state)) == DIERR_INPUTLOST)
	{
	    if ((hResult = device->Acquire()) != DI_OK)
		break;
	}
	else
	    break;
    }
    if (hResult == DI_OK)
	for (unsigned int i(0); i < 256; ++i)
	{
	    down[i] = (state[i] & 0x80) != 0 && !Keyboard::state[i];
	    Keyboard::state[i] = (state[i] & 0x80) != 0;
	}
    else
        for (unsigned int i(0); i < 256; ++i)
	    down[i] = false;

    return * this;
}

DirectX::Keyboard::
Keyboard(HWND hWnd, HINSTANCE hInstance)
    : DirectInput(hWnd, hInstance), State(state), Down(down)
{
    ZeroMemory(state, 256 * sizeof(bool));
    ZeroMemory(down, 256 * sizeof(bool));
    if (input == 0 || hWnd == 0)
	return;
    if (input->CreateDevice(GUID_SysKeyboard, & device, 0) != DI_OK)
    {
	MessageBox(hWnd, "CreateDevice", "Keyboard", MB_OK);
	return;
    }
    if (device->SetDataFormat(& c_dfDIKeyboard) != DI_OK)
    {
	device->Unacquire();
	device->Release();
	device = 0;
	MessageBox(hWnd, "SetDataFormat", "Keyboard", MB_OK);
	return;
    }
    if (device->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
    {
	device->Unacquire();
	device->Release();
	device = 0;
	MessageBox(hWnd, "SetCooperativeLevel", "Keyboard", MB_OK);
	return;
    }
    device->Acquire();
    pointer.insert(this);
}

DirectX::Keyboard::
~Keyboard(void)
{
}