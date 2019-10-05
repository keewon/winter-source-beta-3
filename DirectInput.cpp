#include "DirectInput.h"
 
unsigned int DirectX::DirectInput::counter(0);
std::set <DirectX::DirectInput *> DirectX::DirectInput::pointer;
bool DirectX::DirectInput::paused(true);
LPDIRECTINPUT DirectX::DirectInput::input(0);

void
DirectX::DirectInput::
synchronize(void)
{
    if (paused)
	device->Unacquire();
    else
	device->Acquire();
}

void
DirectX::DirectInput::
synchronize(const bool & paused)
{
    DirectInput::paused = paused;
    for (std::set <DirectInput *>::iterator i(pointer.begin()); i != pointer.end(); ++i)
	(* i)->synchronize();
    return;
}

DirectX::DirectInput::
DirectInput(HWND hWnd, HINSTANCE hInstance): device(0)
{
    ++counter;

    if (input != 0 || hWnd == 0 || hInstance == 0)
	return;
    if (DirectInputCreate(hInstance, DIRECTINPUT_VERSION, & input, 0) != DI_OK)
	MessageBox(hWnd, "DirectInputCreate", "DirectInput", MB_OK);
}

DirectX::DirectInput::
~DirectInput(void)
{
    pointer.erase(this);
    if (device != 0)
    {
	device->Unacquire();
	device->Release();
    }
    if (--counter != 0 || input == 0)
	return;
    input->Release();
    input = 0;
}